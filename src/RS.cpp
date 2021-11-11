#include <Arduino.h>
#include "gps_rswrapper.h"
#include "lcd_rswrapper.h"
#include <LoRa.h>
#include <SPI.h>
#include <lcd_rswrapper.h>
#include <lora_rswrapper.h>


//defining receivable sensor variables
double lat = 0;
double lng = 0;
double alt = 0;
int sats_r = 0;

float magx = 0;
float magy = 0;
float magz = 0;

float temp = 0;
float pres = 0;
float bar_alt = 0;
float vert_velocity = 0;

int bat_status = 0;

//Packet stuff

int counter = 0;
int badPackets = 0;
double successRate = 0;
float receivedRSSI = 0;
float receivedSNR = 0;
int receivedSize = 0;
int mathCounter = 0;
long freqErrorRaw= 0;
String freqErrorConverted = "";

//ScreenSwitching related variables
int LeftSwitch = 25;
int RightSwitch = 26;
short LeftState = 0;
short RightState = 0;
int prevLeftState = 0;
int prevRightState = 0;
bool buttonPressed = 0;

//Screen update logic
int currentScreen = 1;
int prevSats = -1;
int pageCounter = 1;
int prevDisplayedCounter = 0;
int MathCounter = 0; //EXP
bool ScreenSwitched = 0;

//defining necessary functional variables
double prevDistance = 0;
double distance = 0;
double course = 0;
double sats = 0;
bool gpsValid = 0;

//defining deserialization function
void deserializeData(char buffer[]){
  sscanf(buffer, "%lf,%lf,%lf,%d,%f,%f,%f,%f,%f,%f,%d,%d", &lat, &lng, &alt, &sats_r, &magx, &magy, &magz, &temp, &bar_alt, &vert_velocity, &bat_status, &counter); //for deserialization double ahs to vbe specified as %lf
}

//defining message string and deserialization buffer
String message = "";
char buffer[80] = "";

void setup()
{
  //Testing one by one
  Serial.begin(115200);
  Serial.println("Serial connected");

  //GPS
  Serial.println("Setup GPS");
  gps::setup();

  //LoRa
  Serial.println("Setup LoRa");
  lora::setup();

  //LCD
  Serial.println("Setup LCD");
  lcd::setup();

  //Slēdži
  pinMode(LeftSwitch, INPUT);
  pinMode(RightSwitch, INPUT);

}

void loop()
{
  //GPS nolasīšanas/barošanas funkcija
  
  gps::readGps();

  
  //RSSI update
  receivedRSSI = lora::getPacketRssi();
  receivedSNR = lora::getPacketSNR();

  char bufferO [10];
  freqErrorRaw = lora::freqError();
  double freqErrorDouble = freqErrorRaw;
  sprintf(bufferO, "%.1e", freqErrorDouble);
  freqErrorConverted = bufferO;
  
  
  //Slēdžu nolasīšana
  LeftState = digitalRead(LeftSwitch);
  RightState = digitalRead(RightSwitch);

  
  //BUTTON LOGIC
  if(LeftState == 1 && prevLeftState == 0 && pageCounter <=1)
  {
    pageCounter++;
    prevLeftState = 1;
    buttonPressed = 1;
  }

  if(RightState == 1 && prevRightState == 0 && pageCounter >= 1)
  {
    pageCounter = pageCounter-1;
    prevRightState = 1;
    buttonPressed = 1;
  }

  buttonPressed = 0;
  prevLeftState = digitalRead(LeftSwitch);
  prevRightState = digitalRead(RightSwitch);
  
  //loop tiek lasits LoRa

  message = lora::readMessage();
  message.toCharArray(buffer, 80);
  deserializeData(buffer);

  //RS GPS aprēķini un funkcijas
  if(lat != 0 && lng != 0)
  {
    distance = gps::getDistance(lat, lng);
    course = gps::getCourseTo(lat, lng);
  }
  sats = gps::getSatellites();
  gpsValid = gps::gpsValid();
  

//SLIKTO PAKEŠU APRĒĶINS
//Ja divu secīgu pakešu atšķirība ir lielāka par viens, aprēķina izkritušo paketi
//Ja pienāk nulles pakete pēc tā, ka ir saņemta pakete lielāka par 0, tā tiek uztverta kā koruptēta

  if(MathCounter != counter)
  {
    if(counter>=1)
    {
    badPackets = badPackets + (counter-(MathCounter+1));
    successRate = 100-((int)((double)badPackets/counter*100));   
    MathCounter = counter;
    Serial.print("New packet, badPackets = ");
    Serial.print(badPackets);
    Serial.print(" success ratio = ");
    Serial.println(successRate);
    }
  }

//Ja pg counter == 2 un ekrāns pārslēgts vai mainās distance vai pienāk jauna pakete, tad atsvaidzina
//Ja pg counter == 1 un ekrāns pārslēgts vai mainās satelītu skaits, tad atsvaidzina
//Ja pg counter == 0 un ekrāns pārslēgts vai mainās pakešu skaits, tad atsvaidzina

  if(pageCounter == 2 && (currentScreen != 2 || prevDisplayedCounter !=  counter || prevDistance != distance))
  {
        if(counter>=1)
        {
        lcd::writeAll(lat, lng, distance, course, bar_alt, vert_velocity, sats_r, counter);
        prevDisplayedCounter = counter;
        prevDistance = distance;
        currentScreen = 2;
        }
    } 
      else if(pageCounter == 1 && (currentScreen != 1 || prevSats != sats))
      {
        lcd::GPSSetup(sats);
        prevSats = sats;
        currentScreen = 1;
      }
      else if(pageCounter == 0 && (currentScreen != 0 || prevDisplayedCounter != counter))
        {
          lcd::LoRaSetup(MathCounter, badPackets, successRate, receivedRSSI, receivedSNR, freqErrorConverted);
          prevDisplayedCounter = counter;
          currentScreen = 0;
        }
        
        
}

  

