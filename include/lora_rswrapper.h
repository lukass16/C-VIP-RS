#pragma once
#include <LoRa.h>
#include <SPI.h>
#include <lcd_rswrapper.h>


// https://randomnerdtutorials.com/ttgo-lora32-sx1276-arduino-ide/
// https://github.com/sandeepmistry/arduino-LoRa/blob/master/examples/LoRaDuplex/LoRaDuplex.ino


namespace lora {

    #define SCK 5
    #define MISO 19
    #define MOSI 27
    #define SS 18
    #define RST 14
    #define DIO0 26


    String outgoing;              // outgoing message
    
    //defining received message buffer
    char received[80];

    int syncWord = 0xF3;

    byte _localAddress = 0xFF;     // address of this device
    byte _destination = 0xBB;      // destination to send to
    long lastSendTime = 0;        // last send time
    int interval = 2000;          // interval between sends
    boolean _canPrintHeaderMessage = false;

    void sendMessage(String outgoing, int lora_message_id);
    String onReceive(int packetSize); //added math!
    //*NEW
    int getPacketSize();

    
    void setup(double frequency = 868E6, boolean canPrintHeaderMessage = false){
        
        _canPrintHeaderMessage = canPrintHeaderMessage;

        //SPI LoRa pins
        SPI.begin(SCK, MISO, MOSI, SS);
        //setup LoRa transceiver module
        LoRa.setPins(SS, RST, DIO0);
            
        if (!LoRa.begin(frequency)) {
            Serial.println("Starting LoRa failed!");
            while (1);
        }

        //setting paramaters
        LoRa.setSyncWord(0xF3);
        LoRa.setTxPower(10);
        LoRa.setSpreadingFactor(10);
        LoRa.setCodingRate4(6);
        LoRa.setSignalBandwidth(62.5E3);
        Serial.println("Lora connected");
        Serial.println(LoRa.available());
    }

    long freqError()
    {
        return LoRa.packetFrequencyError();
    }

    //!careful
    int getPacketSize()
    {
        return LoRa.parsePacket();
    }

    int getPacketRssi()
    {
        return LoRa.packetRssi();
    }

    float getPacketSNR()
    {
        return LoRa.packetSnr();
    }

     //*NEW
    String readMessage(){
        int packetSize = LoRa.parsePacket();
        String message = "";
        if(packetSize){
            // read packet header bytes:
            int recipient = LoRa.read();          // recipient address
            byte sender = LoRa.read();            // sender address
            byte incomingMsgId = LoRa.read();     // incoming msg ID
            byte incomingLength = LoRa.read();    // incoming msg length
 
            if (recipient != _localAddress && recipient != 0xBB) {
                Serial.println("This message is not for me, recipient: " + String(recipient));
                return "NULL";                            
            }   
            while(LoRa.available()){
                message += (char)LoRa.read();
            }
            Serial.print("Message: " + message);
        }
        else{
            return "NULL";
        }
        Serial.println(" RSSI: " + String(LoRa.packetRssi()));
        return message;
    }

}