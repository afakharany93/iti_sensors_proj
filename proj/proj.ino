#include <Wire.h>
#include <GY80.h>
#include "filter.h"


#define ALPHA_ACC_LPF   0.5

GY80 sensor = GY80(); //create GY80 instance

//acc filters
Filter acc_x_LPF = Filter(ALPHA_ACC_LPF);
Filter acc_y_LPF = Filter(ALPHA_ACC_LPF);
Filter acc_z_LPF = Filter(ALPHA_ACC_LPF);

//acc filter
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
    fYg = acc_y_LPF.apply_filter(val.a_y);
    fZg = acc_z_LPF.apply_filter(val.a_z);


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



