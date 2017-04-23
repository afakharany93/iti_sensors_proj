#include <Wire.h>
#include <GY80.h>

GY80 sensor = GY80(); //create GY80 instance
float tau=0.075;
float a=0.0;
float x_angleC;
float y_angleC;
float pitch=0;
float roll=0;
float looptime=millis();
float looptime_new=0;

void Complementary_Pitch(float newAngle, float newRate,int looptime) 
{
    float dtC = float(looptime)/1000.0;
    a=tau/(tau+dtC);
    x_angleC= a* (x_angleC + newRate * dtC) + (1-a) * (newAngle);

}


 void Complementary_roll(float newAngle, float newRate,int looptime) 
{
    float dtC = float(looptime)/1000.0;
    a=tau/(tau+dtC);
    y_angleC= a* (y_angleC + newRate * dtC) + (1-a) * (newAngle);

}

void setup()
{
    // initialize serial communication at 115200 bits per second:
    Serial.begin(115200);
    sensor.begin();    //initialize sensors
}

void loop()
{
    GY80_scaled val = sensor.read_scaled();       //get values from all sensors
    // print out values
    pitch = (atan2(val.a_x, sqrt(val.a_y*val.a_y + val.a_z*val.a_z))*180.0)/M_PI;
    roll = (atan2(-val.a_y, val.a_z)*180.0)/M_PI;

     
     Complementary_Pitch(pitch,val.g_y,looptime_new);       //accelerometer values
     Complementary_roll(roll,val.g_x,looptime_new);
     
     Serial.print(",acc x =");
     Serial.print(val.a_x); 
     Serial.print(",acc y =");
     Serial.print(val.a_y); 
     Serial.print(",acc z =");
     Serial.print(val.a_z); 

     Serial.print(",gyro x =");
     Serial.print(val.g_x);
     Serial.print(",gyro y =");
     Serial.print(val.g_y);

     Serial.print(",Pitch="); 
     Serial.print(x_angleC);  
     Serial.print(",roll=");        //accelerometer values
     Serial.println(y_angleC);
     looptime_new=looptime-looptime_new;
     looptime=millis();

    delay(300);        // delay in between reads for stability
}

    

