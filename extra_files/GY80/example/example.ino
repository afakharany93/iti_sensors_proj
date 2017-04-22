#include <Wire.h>
#include <GY80.h>

GY80 sensor = GY80(); //create GY80 instance

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
    GY80_scaled val = sensor.read_scaled();       //get values from all sensors
    

    //Low Pass Filter to smooth out data
    fXg = val.a_x * alpha + (fXg * (1.0 - alpha));
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



