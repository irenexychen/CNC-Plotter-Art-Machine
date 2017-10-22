#include <Stepper.h>
#include <Servo.h>
const int stepsPerRevolution = 20; 
Servo penServo;//initiates servo

int servoPin = 6;//pin servo is connected to
Stepper xAxis(stepsPerRevolution, 2,3,10,11); //pins x axis motor are connected to
Stepper yAxis(stepsPerRevolution, 4,5,8,9); //pins y axis motor are connected to
 
void setup() 
{
  penServo.attach(servoPin);//attach servo to arduino
}
 
void loop() 
{
  xAxis.setSpeed(100);
  yAxis.setSpeed(100);
 //max 250 steps for dvd/cd stepper motors 
 
  penServo.write(0);//pulls pen down      
  delay(5000); //pause for 5 seconds
 
  xAxis.step(160);//move x axis on way
  delay(100);

  yAxis.step(160);//move y axis one way
  delay(100);
 
  xAxis.step(-160); //the other way 
  delay(100);
 
  yAxis.step(-160);//the other way
  delay(100);
 //draws a square
 
   penServo.write(70);//pulls pen up      
  delay(5000); //pause for 5 seconds
}