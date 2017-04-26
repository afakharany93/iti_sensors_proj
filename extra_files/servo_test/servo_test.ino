#include <Servo.h>

Servo Pitch_servo;  // create servo object to control a servo
Servo myservo;  // create servo object to control a servo

void setup()
{
    
    Pitch_servo.attach(10);  // attaches the servo on pin 9 to the servo object
     myservo.attach(9);  // attaches the servo on pin 9 to the servo object
    
}

void loop()
{
  Pitch_servo.write(90);
  myservo.write(90); 
  delay(200);
	
}

