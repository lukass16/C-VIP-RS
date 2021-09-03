#pragma once
#include <TinyGPS++.h>
#include <SoftwareSerial.h>


//TODO izchekot kas notiek ar gps - kaa saglabat vecas


namespace gps {

    #define RXPIN 15 //P4 on lopy is set to RX pin (important to remember about crossover connection)
    #define TXPIN 4  //P3
    SoftwareSerial gpsSerial;
    SoftwareSerialConfig ssc = SWSERIAL_8N1; // 8bits-no_parity-1_stop_bit  https://github.com/plerup/espsoftwareserial/
    TinyGPSPlus gps;

    boolean hasData = false;

    //*NEW
    // sens_data::GpsData lastData;  //Last data so that values of zero don't get sent when gps doesn't have lock on
    //*NEW

    void setup(uint gpsRate = 9600)
    {
        gpsSerial.begin(gpsRate, ssc, RXPIN, TXPIN);
        Serial.println("Init GPS: " + String(gpsRate));
    }

    void readGps()
    {
        hasData = false;

        while (gpsSerial.available())
        {
            gps.encode(gpsSerial.read());
            hasData = true;
        }

        // Serial.print("LAT=");
        // Serial.println(gps.location.lat(), 6);
        // Serial.print("LONG=");
        // Serial.println(gps.location.lng(), 6);
        // Serial.print("ALT=");
        // Serial.println(gps.altitude.meters());
    }

    double lastLatitude() {
        return gps.location.lat();  //TODO ar 6 skaitljiem?
    }

    double lastLongitude() {
        return gps.location.lng();
    }

    double lastAltitude() {
        return gps.altitude.meters();
    }

    //*NEW

    int getSatellites()
    {
        return gps.satellites.value();
    }

    bool gpsValid()
    {
        return gps.location.isValid();
    }

    double getDistance(double latRocket, double longRocket)
    {
        //? Necessary to check if gps data is incorrect?
        return gps.distanceBetween(gps.location.lat(), gps.location.lng(), latRocket, longRocket);
    }

    double getCourseTo(double latRocket, double longRocket)
    {
        //? Necessary to check if gps data is incorrect?
        return gps.courseTo(gps.location.lat(), gps.location.lng(), latRocket, longRocket);
    }

    //*NEW
    // sens_data::GpsData getGpsState()
    // {
    //     sens_data::GpsData gd;
    //     if(gps.location.isValid())
    //     {
    //         //*NEW
    //         //adding last good values
    //         lastData.lat = lastLatitude();
    //         lastData.lng = lastLongitude();
    //         lastData.alt = lastAltitude();
    //         gd.lat = lastLatitude();
    //         gd.lng = lastLongitude();
    //         gd.alt = lastAltitude();
    //         return gd;
    //         //*NEW
    //     }
    //     else
    //     {
    //         return lastData;
    //     }
        
    // }
}