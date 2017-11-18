#include <Stepper.h>
#include <Servo.h>
#include <stdbool.h>
#include <stdio.h>

const int stepsPerRevolution = 20; 
Servo penServo;//initiates servo
int servoPin = 6;//pin servo is connected to

Stepper xAxis(stepsPerRevolution, 2,3,10,11); //pins x axis motor are connected to
Stepper yAxis(stepsPerRevolution, 4,5,8,9); //pins y axis motor are connected to

bool draw = true;



int numLines = 0;
int xstep;
int ystep;
int xold;
int yold;
int altXRatio; //how often it alternates between x and y
int altYRatio;
 
int penLiftThreshold;


void penUp();
void penDown();
void makeLoop(int n, int altYStep);


void setup(){
  penServo.attach(servoPin);//attach servo to arduino
  Serial.begin(9600);
  
}
 
void loop() {
  int coords[3000][2] = {{22,90},
{22,91},
{22,92},
{22,93},
{23,87},
{23,88},
{23,89},
{23,90},
{23,91},
{23,92},
{55,153},
{55,156},
{55,158},
{55,159},
{55,160},
{55,161},
{55,164},
{55,198},
{56,43},
{56,44},
{56,65},
{56,66},
{56,72},
{56,73},
{56,83},
{56,84},
{56,98},
{56,119},
{56,120},
{56,121},
{56,122},
{56,123},
{56,124},
{56,125},
{56,126},
{56,135},
{56,138},
{56,139},
{56,143},
{56,144},
{56,153},
{56,155},
{56,160},
{56,161},
{56,162},
{56,163},
{56,197},
{56,198},
{56,199},
{57,44},
{57,45},
{57,46},
{57,66},
{57,67},
{57,72},
{57,73},
{57,82},
{57,83},
{57,98},
{57,118},
{57,119},
{57,123},
{57,125},
{57,135},
{57,138},
{57,139},
{57,153},
{57,154},
{57,155},
{57,161},
{57,162},
{57,163},
{57,164},
{57,197},
{58,46},
{58,47},
{58,48},
{58,67},
{58,68},
{58,69},
{58,73},
{58,74},
{58,81},
{58,82},
{58,98},
{58,118},
{58,124},
{58,125},
{58,132},
{58,134},
{58,137},
{58,138},
{58,163},
{58,164},
{58,165},
{58,197},
{59,47},
{59,48},
{59,49},
{59,73},
{59,74},
{59,75},
{59,80},
{59,81},
{59,97},
{59,98},
{59,118},
{59,123},
{59,124},
{59,132},
{59,134},
{59,136},
{59,137},
{59,141},
{59,165},
{59,166},
{59,167},
{59,196},
{60,49},
{60,50},
{60,51},
{60,70},
{60,73},
{60,74},
{60,75},
{60,80},
{60,81},
{60,97},
{60,118},
{60,122},
{60,123},
{60,131},
{60,136},
{60,139},
{60,141},
{60,166},
{60,167},
{60,168},
{60,196},
{61,50},
{61,51},
{61,52},
{61,70},
{61,75},
{61,76},
{61,77},
{61,78},
{61,79},
{61,80},
{61,97},
{61,118},
{61,119},
{61,120},
{61,121},
{61,122},
{61,131},
{61,133},
{61,135},
{61,136},
{61,139},
{61,141},
{61,167},
{61,168},
{61,169},
{61,196},
{62,52},
{62,53},
{62,54},
{62,70},
{62,75},
{62,77},
{62,78},
{62,79},
{62,97},
{62,98},
{62,118},
{62,119},
{62,120},
{62,121},
{62,130},
{62,132},
{62,134},
{62,135},
{62,138},
{62,141},
{62,168},
{62,169},
{62,170},
{62,196},
{63,54},
{63,55},
{63,70},
{63,71},
{63,76},
{63,77},
{63,98},
{63,117},
{63,118},
{63,119},
{63,120},
{63,129},
{63,134},
{63,137},
{63,140},
{63,170},
{63,171},
{63,196},
{64,55},
{64,56},
{64,57},
{64,58},
{64,71},
{64,76},
{64,77},
{64,98},
{64,116},
{64,117},
{64,129},
{64,131},
{64,133},
{64,134},
{64,137},
{64,138},
{64,139},
{64,171},
{64,172},
{64,196},
{65,58},
{65,59},
{65,71},
{65,72},
{65,76},
{65,98},
{65,113},
{65,114},
{65,115},
{65,129},
{65,133},
{65,172},
{65,173},
{65,196},
{66,59},
{66,60},
{66,61},
{66,62},
{66,71},
{66,72},
{66,74},
{66,75},
{66,76},
{66,98},
{66,99},
{66,112},
{66,113},
{66,114},
{66,128},
{66,130},
{66,132},
{66,133},
{66,173},
{66,196},
{67,62},
{67,63},
{67,64},
{67,72},
{67,73},
{67,75},
{67,76},
{67,99},
{67,100},
{67,110},
{67,111},
{67,112},
{67,118},
{67,128},
{67,130},
{67,132},
{67,133},
{67,173},
{67,174},
{67,196},
{67,197},
{68,64},
{68,65},
{68,72},
{68,73},
{68,75},
{68,98},
{68,99},
{68,100},
{68,101},
{68,106},
{68,108},
{68,109},
{68,110},
{68,113},
{68,114}}; 
  delay (200);
  char coord;
  int line = 0;
  
  xAxis.setSpeed(100);
  yAxis.setSpeed(100);
  //max 250 steps for dvd/cd stepper motors 

  //penUp();
/*
   while (Serial.available() > 0){
    while (coord == '!'){ //end character in txt file
      coord = Serial.read();
      coords[line][0]= (int)coord;//x value
      
      if (coord == ' '){
        coords[line][1]= (int)coord;//y value
      } else if (coord == '\n'){
        numLines++;
        line++;
      } 
    }
    */
 
  while (draw == true){
    //Serial.print(6);
    //Serial.print((char)coords[0][1]);
    Serial.print("it's at least in the loop???? pls");
    
    penDown();//pulls pen down      
    delay(5000); //pause for 5 seconds
    xold = coords[0][0];
    yold = coords[0][1];
/*
    xAxis.step(xold);
    delay(1);
    yAxis.step(yold);
    delay(1);
    */
    Serial.print("right before the next for loop");
    for (int i = 1; i < 200; i++){
      xstep = coords[i][0] - xold;
      ystep = coords[i][1] - yold;
      Serial.print("Hi");
      if (xstep > 5 || ystep > 5){ //too far, make new line
        //penUp();
        altYRatio = ystep/xstep;
        makeLoop(xstep, altYRatio);
        //penDown();
      } else if (xstep > 0){ //close enough, it's a line, draw
        altYRatio = ystep/xstep;
        makeLoop(xstep, altYRatio);
      } else { //x is 0, can't divide by 
        makeLoop(1, ystep);
      }
      xold = coords[i][0];
      yold = coords[i][1];
    }
    
    penUp();
    draw = false;
    //}
  }
}

void makeLoop(int n, int altYStep){
  Serial.print("does it go here??");
  for (int i = 0; i < n; i++){
    xAxis.step(1);
    delay(1);
    yAxis.step(altYStep);
    delay(1);
  }
}

void penUp(){
  penServo.write(70);//pulls pen up 
  delay(3000); //pause for 3 seconds
}

void penDown(){
  penServo.write(0);//pulls pen up
  delay(3000); //pause for 3 seconds 
}


