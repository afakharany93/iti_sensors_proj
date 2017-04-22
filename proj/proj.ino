#include <Wire.h>
#include <GY80.h>
#include "filter.h"

GY80 sensor = GY80(); //create GY80 instance
Filter acc_x_LPF = Filter(0.5);

//acc filter
const float alpha = 0.5;
double fXg = 0;
double fYg = 0;
double fZg = 0;



void setup()
{
    // initialize serial communication at 115200 bits per second:
    Serial.begin(115200);
    sensor.begin();       //initialize sensors
}


void loop()
{
    GY80_raw val = sensor.read_raw();       //get values from all sensors
    

    //Low Pass Filter to smooth out data
    fXg = acc_x_LPF.apply_filter(val.a_x);
    fYg = val.a_y * alpha + (fYg * (1.0 - alpha));
    fZg = val.a_z * alpha + (fZg * (1.0 - alpha));


    // print out values
    Serial.print("Acc:");                         //accelerometer values
    Serial.print(fXg,3);
    Serial.print(',');
    Serial.print(fYg,3);
    Serial.print(',');
    Serial.print(fZg,3);
    Serial.print(' ');
    Serial.print("Gyro:");                        //gyroscope values
    Serial.print(val.g_x,1);
    Serial.print(',');
    Serial.print(val.g_y,1);
    Serial.print(',');
    Serial.print(val.g_z,1);
    Serial.print(' ');
    Serial.print("P:");                           //pressure values
    Serial.print(val.p,5);
    Serial.print(' ');
    Serial.print("T:");                           //temperature values
    Serial.println(val.t,1);


    delay(250);        // delay in between reads for stability
}



