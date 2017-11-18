#include <Stepper.h>
#include <Servo.h>
//#include "coordinates.txt"
const int stepsPerRevolution = 20; 
Servo penServo;//initiates servo

int servoPin = 6;//pin servo is connected to
Stepper xAxis(stepsPerRevolution, 2,3,10,11); //pins x axis motor are connected to
Stepper yAxis(stepsPerRevolution, 4,5,8,9); //pins y axis motor are connected to
int coords[3000][2];
int numLines = 0;

void setup() 
{
  penServo.attach(servoPin);//attach servo to arduino
  Serial.begin (9600);
}
 
void loop() 
{
  delay (200);
  char coord;
  int line = 0;
  
  while (Serial.available() > 0)
  {
   
    while (coord == '!') //end character in txt file
    {
      coord = Serial.read();
      coords[line][0]= (int)coord;//x value
      
      if (coord == ' ')
      {
        coords[line][1]= (int)coord;//y value

      }
      else if (coord == '\n')
      {
        numLines ++;
        line ++;
      }
      
    }
}
}
  
 
        
 


