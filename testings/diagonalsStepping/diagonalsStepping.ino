#include <Stepper.h>
#include <Servo.h>
#include <stdbool.h>

const int stepsPerRevolution = 20; 
Servo penServo;//initiates servo
int servoPin = 6;//pin servo is connected to

Stepper xAxis(stepsPerRevolution, 2,3,10,11); //pins x axis motor are connected to
Stepper yAxis(stepsPerRevolution, 4,5,8,9); //pins y axis motor are connected to

bool draw = true;
 
void setup() 
{
	penServo.attach(servoPin);//attach servo to arduino

}
 
void loop() 
{
	xAxis.setSpeed(100);
	yAxis.setSpeed(100);
	//max 250 steps for dvd/cd stepper motors 
 
	while (draw == true){
		penServo.write(0);//pulls pen down      
		delay(5000); //pause for 5 seconds
	 
		for (int i = 0; i < 100; i++){
			xAxis.step(1);
			delay(1);
			yAxis.step(1);
			delay(1);
		}
	 
		for (int i = 0; i < 100; i++){
			xAxis.step(-1);
			delay(1);
			yAxis.step(1);
			delay(1);
		}
		
		for (int i = 0; i < 100; i++){
			xAxis.step(-1);
			delay(1);
			yAxis.step(-1);
		  delay(1);
		}

		for (int i = 0; i < 100; i++){
			xAxis.step(1);
			delay(1);
			yAxis.step(-1);
			delay(1);
		}
		
		penServo.write(70);//pulls pen up      
		delay(5000); //pause for 5 seconds
		
		draw = false;
	}
	
}
