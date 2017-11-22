//////NEWWWWWW////////
#include <Stepper.h>
#include <Servo.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

const int stepsPerRevolution = 20;
const int servoPin = 6;//pin servo is connected to

//Hardware Initialization
Servo penServo;//initiates servo
Stepper xAxis(stepsPerRevolution, 2, 3, 10, 11); //pins x axis motor are connected to
Stepper yAxis(stepsPerRevolution, 4, 5, 8, 9); //pins y axis motor are connected to

//Dynamic Memory Initialization
bool drawBool = true;
int xstep;
int ystep;
int xold;
int yold;

//Function Declarations
void penUp();
void penDown();
void drawX(int n);
void drawY(int n);
void blockingRead(int currentX[], int currentY[]);
void convertSerialStrInputToInt(int currentX[], int currentY[]);

void setup()
{
  xAxis.setSpeed(100);
  yAxis.setSpeed(100);
  penServo.attach(servoPin);//attach servo to arduino
  Serial.begin(9600);
}

void loop()
{
  while (drawBool == true)
  {
    int currentX[49];
    int currentY[49];
    delay (3000);

    int numCoords = 1608;

    xold = 0;
    yold = 0;

    int iters = 5;
    for (int i = 0; i < numCoords; i = i + 9)
      /* Outer for-loop keeps track of number of coordinates to go through, which overlaps with inner for-loop
             which reads in 49 coordinates at the same time. Hence to merge the for-loops, the i counter of the outer
             for-loop must be iterated along with the inner for-loop.
      */
    {
      Serial.print("\n");
      Serial.print("@GetNext"); //gives order to imageProcessing.py
      Serial.flush();

      //python sends in next 49 coords
      blockingRead(currentX, currentY);

    
  delay(5000);

      for (int i = 0; i < 5; i++) {
        Serial.print("current move ");
        Serial.print(currentX[i]);
        Serial.print(" ");
        Serial.print(currentY[i]);
        Serial.print("\n");

      }
              Serial.flush();

        delay(5000);

      for (int k = 0; k < iters; k++)
      {
        if (currentX[k] == -1) //end of file (not 49 full coords), blocked read returns -1 instead of a coordinate
        {
          iters = k;
          break;
        }
      }



      for (int k = 0; k < iters; k++)
      {


        //calculate difference
        xstep = currentX[k] - xold;
        ystep = currentY[k] - yold;

        if (fabs(xstep) > 10 || fabs(ystep) > 10 ) //if points are too far apart from eachother to be continuous
        { //move to location
          penUp();
          drawX((int)xstep);
          drawY((int)ystep);
          penDown();
        }
        else
        { //actually draw line
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

void penUp()
{
  penServo.write(70);//pulls pen up
  delay(1000);
}

void penDown()
{
  penServo.write(0);//pulls pen up
  delay(1000);
}

void drawX(int stepsToMove)
{
  int steps;
  if (stepsToMove > 0)
  {
    steps = 1;
  }
  else
  {
    stepsToMove *= -1;
    steps = -1;
  }
  for (int i = 0; i < (int)stepsToMove; i++)
  {
    xAxis.step(steps);
    delay(10);
  }
}


void drawY(int stepsToMove)
{
  int steps;
  if (stepsToMove > 0)
  {
    steps = 1;
  }
  else
  {
    stepsToMove *= -1;
    steps = -1;
  }
  for (int i = 0; i < (int)stepsToMove; i++)
  {
    yAxis.step(1);
    delay(10);
  }
}

void blockingRead(int currentX[], int currentY[] )
{
  while (!Serial.available())
  {
    delay(100);
  }
  convertSerialStrInputToInt(currentX, currentY);
}

void convertSerialStrInputToInt(int currentX[], int currentY[])
{ //only called when serial.available

  Serial.print("inside convertInputToInt function");
  Serial.flush();
  String resultStr;
  int resultInt;
  String inputString = "";

  String tempCharX = "";
  char tempCharY;
  int l;
  //Serial.setTimeout(1500);
  
  int i = 0;
  int h = 0;
  while (Serial.available())
  { 
    tempCharX = Serial.readString();
    h++;
  }

  
  ///for (int i = 0; i < 50; i++)
  //{
  Serial.print("do you even go here???\n");


  Serial.print(tempCharX);
  Serial.flush();
  Serial.print("testing char: ");
  Serial.print(tempCharX[0]);

  Serial.print("\n");
  Serial.flush();
  l = tempCharX.length() - 1;


     Serial.print("length char: ");
  Serial.print(l);

  Serial.print("\n");
  
  for (int n = 0; n < l; n++)
  {

    if (tempCharX[n] == '!') //end of entire coordinates to be sent from python
    {
      resultInt = -1;
      currentX[i] = resultInt;
      break;
    }
    Serial.print(n);
    Serial.print("getting X\n");

    Serial.flush();
    while (tempCharX[n] != ' ') //python inputs coordinates in a string separated by spaces & on diff lines
    {
      inputString += (char)tempCharX[n];
      n++;
    }
    n++;


    for (int j = 0; j < inputString.length(); j++)
    {
      if (inputString[j] == '.') //python puts integers as floats into input fmor serial.read(), we just want it as an int
      {
        break;
      }
      else if (inputString[j] >= 48 && inputString[j] <= 57) //is an integer 0 to 9
      {
        resultStr += inputString[j];
      }
    }
    resultInt = resultStr.toInt();
    resultStr = "";
    inputString = "";
    currentX[i] = resultInt;


    while (tempCharX[n] != ' ') //python inputs coordinates in a string separated by spaces & on diff lines
    {
      inputString += (char)tempCharX[n];
      n++;
    }
    Serial.print(inputString);
    Serial.print("\n");
    Serial.flush();

    for (int j = 0; j < inputString.length(); j++)
    {
      if (inputString[j] == '.') //python puts integers as floats into input fmor serial.read(), we just want it as an int
      {
        Serial.print("found dot!!!");
            Serial.print("\n");
    Serial.flush();
        break;
      }
      else if (inputString[j] >= 48 && inputString[j] <= 57) //is an integer 0 to 9
      {
        resultStr += inputString[j];
      }
    }
    Serial.print("result string is :");
    Serial.print(resultStr);
    Serial.print("\n");
    Serial.flush();
    resultInt = resultStr.toInt();
    resultStr = "";
    inputString = "";
    
    currentY[i] = resultInt;

    i++;
    //}

  }
  Serial.print("got all 50\n");


  for (int i = 0; i < 5; i++) {
    Serial.print("current point ");
    Serial.print(currentX[i]);
    Serial.print(" ");
    Serial.print(currentY[i]);
    Serial.print("\n");
  }

  Serial.flush();
}
