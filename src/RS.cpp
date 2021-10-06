#include <Arduino.h>
#include <cstdio>
#include "gps_rswrapper.h"
#include "lcd_rswrapper.h"
#include "lora_rswrapper.h"
#include <ArduinoJson.h>

using namespace std;

String incoming;
StaticJsonDocument<256> document;

//defining receivable sensor variables
double lat = 0;
double lng = 0;
double alt = 0;

float magx = 0;
float magy = 0;
float magz = 0;

float temp = 0;
float pres = 0;
float bar_alt = 0;
float speed = 0;

int counter = 0;

//defining necessary functional variables
double distance = 0;
double course = 0;
bool gpsValid = 0;

void deserializeData(String ser){
  char buffer[100];
  sscanf(buffer, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%d", &lat, &lng, &alt, &magx, &magy, &magz, &temp, &pres, &bar_alt, &counter);
}

void setup()
{
  //Testing one by one
  Serial.begin(115200);
  Serial.println("Serial connected");

  //*test
  lcd::setup();
  delay(1000000);

  // //GPS
  // Serial.println("Setup GPS");
  // gps::setup();

  //LoRa
  Serial.println("Setup LoRa");
  lora::setup();

  // //LCD
  // Serial.println("Setup LCD");
  // lcd::setup();
}

void loop()
{
  //TODO - atkariba no packet counter var ieviest vai ko mainam lcd - proti ja nav jauna pakete nekas nenotiek jauns
  //loop tiek lasits LoRa
  //jsonString = lora::onReceive(lora::getPacketSize());
  incoming = lora::readMessage();

  if (incoming != "NULL")
  {
    deserializeData(incoming);
  }

  //GPS
  lat = document["lat"];
  lng = document["lng"];
  alt = document["alt"];

  // MAGNETOMETER
  magx = document["x"];
  magy = document["y"];
  magz = document["z"];

  // BAROMETER
  temp = document["temp"];
  pres = document["pres"];
  bar_alt = document["bar_alt"];
  speed = document["vert_vel"];

  // COUNTER
  counter = document["counter"];


  // //*FUNKCIJAS
  // //- Gps heading
  // course = gps::getCourseTo(lat, lng);
  // //- Gps distance
  // distance = gps::getDistance(lat, lng);
  // //- Target coord
  Serial.print("lat: ");
  Serial.println(lat);
  Serial.print("lng: ");
  Serial.println(lng);

  //test
  Serial.println("mag: " + String(alt));
  //test

  //- Mes esam pieslegusies - pasam RS ?isValid()
  //gpsValid = gps::gpsValid();
  //- augstums (var pieslegt funkciju parslegties no gps un barometra augstumu)
  Serial.print("Barometer altitude: ");
  Serial.println(bar_alt);
  Serial.print("GPS altitude: ");
  Serial.println(alt);
  //- vertical speed
  //TODO add vertical speed to C VIP ROCKET code

  //TODO - test lcd displaying capability
  //lcd::writeAll(lat, lng, distance, course, alt, speed, gpsValid, counter);

  //? lapas-pogas


  //TESTING
  Serial.println("magx: " + String(magx));
  Serial.println("magy: " + String(magy));
  Serial.println("magz: " + String(magz));

  Serial.println("counter: " + String(counter));

  Serial.println("");
  delay(2000);
}