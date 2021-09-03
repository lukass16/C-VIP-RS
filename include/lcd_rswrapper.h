#pragma once
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <string>
#include <sstream>

//https://github.com/johnrickman/LiquidCrystal_I2C
//https://docs.google.com/document/d/1Fbtc8zCtQmf6O5GGj64akNSewvqAAdK0V5oWDq2YLxc/edit
//?Should test this library as it is not actively maintained by the creators anymore


namespace lcd {

    LiquidCrystal_I2C lcd(0x27, 20, 4);

    void setup()
    {
        lcd.init();
        lcd.backlight();
        lcd.setCursor(1, 0); //Pirmais skaitlis ir kolonna, otrais skatilis ir rinda
        lcd.print("Waiting for GPS AV"); 
        lcd.setCursor(8, 2);
        lcd.print("lock"); //TODO parbaudit, vai var printēt arī objektus, kas nav string
    }
    //TODO izdomat ka pectam sho izdest

    void writeLat(double lat)
    {
        lcd.setCursor(0, 0);
        lcd.print("Lat:");
        lcd.setCursor(4, 0);
        lcd.print("       "); //*Deletes previous data (7)
        lcd.print(lat, 4);
    }

    void writeLon(double lon)
    {
        lcd.setCursor(0, 1);
        lcd.print("Lon:");
        lcd.setCursor(4, 1);
        lcd.print("       "); //*Deletes previous data (7)
        lcd.print(lon, 4);
    }

    void writeDistance(double dist)
    {
        lcd.setCursor(0, 2);
        lcd.print("Dist:");
        lcd.setCursor(5, 2);
        lcd.print("     "); //*Deletes previous data (5)
        lcd.print(dist, 0);
        lcd.setCursor(8, 2);  //pieņemu, ka distace < 1000m
        lcd.print("m");
    }

    void writeHeading(double course)
    {
        lcd.setCursor(0, 3);
        lcd.print("HDNG:");
        lcd.setCursor(5, 3);
        lcd.print("   "); //*Deletes previous data (3)
        lcd.print(course, 0);
    }

    void writeAltitude(double alt)
    {
        lcd.setCursor(11, 2);
        lcd.print("Alt:");
        lcd.setCursor(15, 2);
        lcd.print("    "); //*Deletes previous data (4)
        lcd.print(alt, 0);
        lcd.setCursor(19, 2);  
        lcd.print("m");
    }

    void writeGpsStatus(bool status)
    {
        lcd.setCursor(19, 0);
        if(status)
        {
            lcd.print("*");
        }
        else
        {
            lcd.print("O");
        }
    }

    void writeSpeed(float spd)
    {
        lcd.setCursor(10, 3);
        lcd.print("SPD:");
        lcd.setCursor(14, 3);
        lcd.print("      "); //*Deletes previous data (6)
        lcd.print(spd, 1);
    }

    void writeCounter(int counter)
    {
        lcd.setCursor(12, 0);
        lcd.print("C:");
        lcd.setCursor(14, 0);
        lcd.print(counter);
    }

    void writeAll(double lat, double lon, double dist, double course, double alt, float spd, bool status, int counter)
    {
        writeLat(lat);
        writeLon(lon);
        writeDistance(dist);
        writeHeading(course);
        writeAltitude(alt);
        writeSpeed(spd);
        writeGpsStatus(status);
        writeCounter(counter);
    }

    void test()
    {
        lcd.setCursor(0, 0);
        lcd.print("Testing, testing"); 
    } 
}

