#include <Stepper.h>
#include <Servo.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

//Hardware Initialization
const int stepsPerRevolution = 20;
Servo penServo;//initiates servo
int servoPin = 6;//pin servo is connected to

Stepper xAxis(stepsPerRevolution, 2, 3, 10, 11); //pins x axis motor are connected to
Stepper yAxis(stepsPerRevolution, 4, 5, 8, 9); //pins y axis motor are connected to

//Dynamic Memory Initialization
bool drawBool = true;
bool ledstate = false;

int xstep;
int ystep;
int xold;
int yold;

//Function Declarations
void penUp();
void penDown();
void drawX(int n);
void drawY(int n);
int blockingRead();
int convertInputToInt();

void setup()
{
  penServo.attach(servoPin);//attach servo to arduino
  Serial.begin(9600);
  pinMode(13, OUTPUT);  //makes built-in LED flash
}

void loop()
{
  while (drawBool == true)
  {
    int currentX[50];
    int currentY[50];
    delay (3000);

    int numCoords = ;  //Serial.parseInt();
    //Serial.print("FROM ARDUINO!! READ the numCoords\n");
    xAxis.setSpeed(100);
    yAxis.setSpeed(100);

    //sets up position, move to first coord
    //Serial.print("FIRST\n");
    Serial.print("@GetNext"); //anything that is a command shouldn't have a \n??
    Serial.flush();
    delay(250);
    currentX[0] = blockingRead();
    currentY[0] = blockingRead();

    penUp();
    drawX((int)currentX[0]);
    drawY((int)currentY[0]);
    penDown();

    xold = currentX[0];
    yold = currentY[0];

    //clear current, fresh memory to use in loop
    currentX[0] = 0;
    currentY[0] = 0;

    for (int i = 1; i < numCoords; i++)
    {
      Serial.print("@GetNext"); //gives order to imageProcessing.py
      Serial.flush();
      delay(250);
      //python sends in next 49 coords
      for (int k = 0; k < 50; k++) {
        currentX[k] = blockingRead();
        //delay(2000);
        currentY[k] = blockingRead(); //reads y

      }

      for (int k = 0; k < 50; k++) {
        //calculate difference
        xstep = currentX[k] - xold;
        ystep = currentY[k] - yold;

        if (fabs(xstep) > 10 || fabs(ystep) > 10 ) {
          //move to location
          penUp();
          drawX((int)xstep);
          drawY((int)ystep);
          penDown();
        } else {
          drawX((int)xstep);
          drawY((int)ystep);
        }
        xold = currentX[k];
        yold = currentY[k];
      }
    }
    penUp();
    drawBool = false;
  }
}


int blockingRead()
{
  //Serial.print("INSIDE BLOCKING READ\n");
  //Serial.flush();
  while (!Serial.available())
  {
    //Serial.print("FML SERIAL IS NOT AVAILABLE??? WHERE'S MAH NEXT COORDINATE\n");
    //Serial.flush();
    delay(100);
  }
  return convertInputToInt();
}


void drawX(int n)
{
  if (n > 0) {
    /*Serial.print("Drawing positive x");
      Serial.print(n);
      Serial.print("\n");
      Serial.flush();
    */
    for (int i = 0; i < (int)n; i++)
    {
      xAxis.step(1);
      delay(10);
    }
  } else if (n < 0) {
    /*Serial.print("Drawing negative x");
      Serial.print(n);
      Serial.print("\n");
      Serial.flush();
    */
    for (int i = 0; i > (int)n; i--)
    {
      xAxis.step(-1);
      delay(10);
    }
  } else {
    //Serial.print("No x step\n");
  }
}


void drawY(int n)
{
  if (n > 0) {
    //    Serial.print("Drawing positive y");
    //    Serial.print(n);
    //    Serial.print("\n");
    //    Serial.flush();
    for (int i = 0; i < (int)n; i++)
    {
      yAxis.step(1);
      delay(10);
    }
  } else if (n < 0) {
    //    Serial.print("Drawing negative y");
    //    Serial.print(n);
    //    Serial.print("\n");
    //    Serial.flush();
    for (int i = 0; i > (int)n; i--)
    {
      yAxis.step(-1);
      delay(10);
    }
  } else {
    //Serial.print("No y step\n");
  }
}

int convertInputToInt() {
  //  Serial.print("inside convertInputToInt function");
  //  Serial.flush();
  String resultS;
  int resultI;
  char tempChar = Serial.read();
  String inputString = "";
  //assume serial.available
  int i = 0;
  while (tempChar != ' ' && tempChar != '\n') {
    inputString += (char)tempChar;
    tempChar = Serial.read();
  }
  for (int i = 0; i < inputString.length(); i++)
  {
    if (inputString[i] == ".")
    {
      break;
    }
    else if (inputString[i] >= 48 && inputString[i] <= 57) //is a numerical val
    {
      resultS += inputString;
    }
  }
  resultI = resultS.toInt();
  return resultI;
}


void penUp()
{
  penServo.write(70);//pulls pen up
  delay(1000); //pause for 3 seconds
}

void penDown()
{
  penServo.write(0);//pulls pen up
  delay(1000); //pause for 3 seconds
}
