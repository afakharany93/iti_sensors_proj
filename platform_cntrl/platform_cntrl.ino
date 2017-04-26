#include <Wire.h>
#include <GY80.h>
#include <Servo.h>
#include "fuzzy.h"
#include "filter.h"

#define sensitivty 3
#define RANGE 0
#define ALPHA   0.5
Filter Pitch_LPF = Filter(ALPHA);
Filter Roll_LPF = Filter(ALPHA);

Servo Pitch_servo;  // create servo object to control a servo
Servo Roll_servo;  // create servo object to control a servo

GY80 sensor = GY80(); //create GY80 instance


/*
creating an instance of the fuzzy controller.
the constructor takes in first argument is the number of fuzzy sets of the both error and change in error,
the second argument is the max of the input range,
third argument is the minimum of the input range,
forth argument is the maximum of the output range,
fifth argument is the minimum of the output range
*/
fuzzy Pitch_cntrlr(3,180/sensitivty,-180/sensitivty,180/sensitivty,0);
fuzzy Roll_cntrlr(3,180/sensitivty,-180/sensitivty,180/sensitivty,0);   

//double tau=0.075;
double tau=0.03;
double a=0.0;
double x_angleC;
double y_angleC;
double pitch=0;
double roll=0;
double looptime=millis();
double looptime_new=0;

double filtered_pitch=0;
double filtered_roll=0;

int ch_op_pitch_servo = 0;
int pitch_servo_val= 0;

int ch_op_roll_servo = 0;
int roll_servo_val= 0;

void Complementary_Pitch(double newAngle, double newRate,int looptime) 
{
    double dtC = double(looptime)/1000.0;
    a=tau/(tau+dtC);
    x_angleC= a* (x_angleC + newRate * dtC) + (1-a) * (newAngle);

}


 void Complementary_roll(double newAngle, double newRate,int looptime) 
{
    double dtC = double(looptime)/1000.0;
    a=tau/(tau+dtC);
    y_angleC= a* (y_angleC + newRate * dtC) + (1-a) * (newAngle);

}

void setup()
{
    // initialize serial communication at 115200 bits per second:
    Serial.begin(9600);
    
    //sensor.g_set_scale(GY80_g_scale_250);

    Pitch_servo.attach(9);  // attaches the servo on pin 9 to the servo object
    Roll_servo.attach(10);
  /*
    Pitch_servo.write(80);
     Roll_servo.write(95);
     delay(500);
     */
    sensor.begin();    //initialize sensors  
   
}

void loop()
{
    static int P_old_value=0;
    static int R_old_value=0;
    GY80_scaled val = sensor.read_scaled();       //get values from all sensors
    // print out values
    pitch = (atan2(val.a_x, sqrt(val.a_y*val.a_y + val.a_z*val.a_z))*180.0)/M_PI;
    roll = (atan2(val.a_y, sqrt(val.a_x*val.a_x + val.a_z*val.a_z))*180.0)/M_PI;
     
    //roll = (atan2(-val.a_y, val.a_z)*180.0)/M_PI;

     
     Complementary_Pitch(pitch,val.g_y,looptime_new);       //accelerometer values
     Complementary_roll(roll,val.g_x,looptime_new);
     
     
     filtered_pitch = Pitch_LPF.apply_filter(x_angleC);
     filtered_roll = Roll_LPF.apply_filter(y_angleC);
     
     //control
     ch_op_pitch_servo = Pitch_cntrlr.fuzzy_controller(filtered_pitch , 0); //arguments to the fuzzy controller (input, setpoint)
     pitch_servo_val += ch_op_pitch_servo;
     if(pitch_servo_val > 180)
     {
        pitch_servo_val = 180;
     }
     else if(pitch_servo_val < 0)
     {
        pitch_servo_val = 0;
     }

     ch_op_roll_servo = Roll_cntrlr.fuzzy_controller(filtered_roll , 0); //arguments to the fuzzy controller (input, setpoint)
     roll_servo_val += ch_op_roll_servo;
     if(roll_servo_val > 180)
     {
        roll_servo_val = 180;
     }
     else if(roll_servo_val < 0)
     {
        roll_servo_val = 0;
     }

    // pitch_servo_val = map(pitch_servo_val, 0, 180, 0, 170);     // scale it to use it with the servo (value between 0 and 180)
    //  roll_servo_val = map(roll_servo_val, 0, 180, 5, 175);  
     if(abs(P_old_value-pitch_servo_val)>RANGE)
     {
     Pitch_servo.write(pitch_servo_val);
     P_old_value=pitch_servo_val;
     }
     if(abs(R_old_value-roll_servo_val)>RANGE)
     {
     Roll_servo.write(roll_servo_val);
     R_old_value=roll_servo_val;
     }


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
     Serial.print(filtered_pitch);  
     Serial.print(",roll=");        //accelerometer values
     Serial.println(filtered_roll);
     looptime_new=looptime-looptime_new;
     looptime=millis();

    delay(10);        // delay in between reads for stability
}

    

