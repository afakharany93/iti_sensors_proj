#include <math.h>
#include <Wire.h>
#include <GY80.h>
#include "filter.h"


#define ALPHA_ACC_LPF   0.9

GY80 sensor = GY80(); //create GY80 instance

//acc filters
Filter acc_x_LPF = Filter(ALPHA_ACC_LPF);
Filter acc_y_LPF = Filter(ALPHA_ACC_LPF);
Filter acc_z_LPF = Filter(ALPHA_ACC_LPF);

//acc filter
double filtered_acc_x = 0;
double filtered_acc_y = 0;
double filtered_acc_z = 0;

double pitch=0;
double roll=0;

void setup()
{
    // initialize serial communication at 115200 bits per second:
    Serial.begin(9600);
    sensor.begin();       //initialize sensors
    sensor.a_set_scale(GY80_a_scale_4);
}


void loop()
{
    
    //angles calculation from acceleration
    pitch = (atan2(filtered_acc_x, sqrt(filtered_acc_y*filtered_acc_y + filtered_acc_z*filtered_acc_z))*180.0)/M_PI;
    roll = (atan2(filtered_acc_y, sqrt(filtered_acc_x*filtered_acc_x + filtered_acc_z*filtered_acc_z))*180.0)/M_PI;

    GY80_raw val = sensor.read_raw();       //get values from all sensors
    

    //Low Pass Filter to smooth out data
    filtered_acc_x = acc_x_LPF.apply_filter(val.a_x);
    filtered_acc_y = acc_y_LPF.apply_filter(val.a_y);
    filtered_acc_z = acc_z_LPF.apply_filter(val.a_z);

    
    //roll = (atan2(-filtered_acc_y, filtered_acc_z)*180.0)/M_PI;


    // print out values
    Serial.print("Acc:");                         //accelerometer values
    Serial.print(filtered_acc_x,3);
    Serial.print(',');
    Serial.print(filtered_acc_y,3);
    Serial.print(',');
    Serial.print(filtered_acc_z,3);
    Serial.print(' ');

    Serial.print("  Gyro:");                        //gyroscope values
    Serial.print(val.g_x);
    Serial.print(',');
    Serial.print(val.g_y);
    Serial.print(',');
    Serial.print(val.g_z);


    Serial.print("  angles:");                        //gyroscope values
    Serial.print("  pitch: ");
    Serial.print(pitch);
    Serial.print(',');
    Serial.print("  Roll: ");
    Serial.print(roll);

    Serial.print("\n");
    // Serial.print(' ');
    // Serial.print("P:");                           //pressure values
    // Serial.print(val.p,5);
    // Serial.print(' ');
    // Serial.print("T:");                           //temperature values
    // Serial.println(val.t);


    delay(200);        // delay in between reads for stability
}



