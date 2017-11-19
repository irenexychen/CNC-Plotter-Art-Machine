#include <Stepper.h>
#include <Servo.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>


const int stepsPerRevolution = 20;
Servo penServo;//initiates servo
int servoPin = 6;//pin servo is connected to

Stepper xAxis(stepsPerRevolution, 2, 3, 10, 11); //pins x axis motor are connected to
Stepper yAxis(stepsPerRevolution, 4, 5, 8, 9); //pins y axis motor are connected to

bool drawBool = true;

bool ledstate = false;

int xstep;
int ystep;
int xold;
int yold;     //how often it alternates between x and y
int altYRatio;

void penUp();
void penDown();
void makeLoop(int n, int altYStep);

void setup() {
  penServo.attach(servoPin);//attach servo to arduino
  Serial.begin(9600);
  pinMode(13, OUTPUT);  //makes built-in LED flash
}

void loop() {
  int coords[1][2];
  int i = 0;

  delay (200);
  int coord;
  xAxis.setSpeed(100);
  yAxis.setSpeed(100);

  while (coord != '!') //end character in txt file
  {
    Serial.print("@GetNext"); //gives order to imageProcessing.py
    pinMode(13, ledstate);
    ledstate = !ledstate;

    //python sends in next two coords
    coords[1][0] = blockingRead(); //delay reading 1 byte
    coords[1][1] = blockingRead(); //reads y

    //python must stop sending here
    //ask python to sleep so that the cnc can have enough time to draw
    Serial.print("@Sleep");

    //calculate difference
    xstep = coords[1][0] - xold;
    ystep = coords[1][1] - yold;

    if (fabs(xstep) > 5 || fabs(ystep) > 5  ) {
      //move to location
      penUp(); 
      drawX(xstep);
      drawY(ystep);
    } else { 
      //draw the shit
      penDown();
      drawX(xstep);
      drawY(ystep);
    }
    xold = coords[1][0];
    yold = coords[1][1];

  }
}
penUp();
}


void drawX(int n) {
  Serial.print("Drawing!!X");
  Serial.print(n);
  for (int i = 0; i < n; i++) {
    xAxis.step(1);
    delay(20);
  }
}

char blockingRead() {
  while (!Serial.available()) {
    delay(100);
  }
  return Serial.parseInt();
}

void drawY(int n) {
  Serial.print("Drawing!!Y");
  Serial.print(n);
  for (int i = 0; i < n; i++) {
    yAxis.step(1);
    delay(20);
  }
}

void penUp() {
  penServo.write(70);//pulls pen up
  delay(3000); //pause for 3 seconds
}

void penDown() {
  penServo.write(0);//pulls pen up
  delay(3000); //pause for 3 seconds
}
