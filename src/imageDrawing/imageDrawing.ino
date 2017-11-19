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
    int coords[1][2];
    delay (3000);

    //int numCoords = Serial.parseInt();
    //Serial.print("FROM ARDUINO!! READ the numCoords\n");
    xAxis.setSpeed(100);
    yAxis.setSpeed(100);

    //sets up position, move to first coord
    Serial.print("FIRST\n");
    Serial.print("@GetNext"); //anything that is a command shouldn't have a \n??
    delay(4000);
    xold = (int)blockingRead();
    yold = blockingRead();
    Serial.print("WHY THE FUCK ARE YOU NOT SENDINGG TO PYTHON???\n");
    Serial.print(xold);
    Serial.print(yold);
    penUp();
    drawX((int)xold);
    drawY((int)yold);
    penDown();

    for (int i = 1; i < 828; i++)
    {
      Serial.print("@GetNext"); //gives order to imageProcessing.py
      pinMode(13, ledstate);
      ledstate = !ledstate;

      //python sends in next two coords
      coords[1][0] = (int)convertInputToInt(); //delay reading 1 byte
      coords[1][1] = (int)convertInputToInt(); //reads y

      //python must stop sending here
      //ask python to sleep so that the cnc can have enough time to draw
      Serial.print("@Sleep\n");

      //calculate difference
      xstep = coords[1][0] - xold;
      ystep = coords[1][1] - yold;

      if (fabs(xstep) > 7 || fabs(ystep) > 7 )
      {
        //move to location
        penUp();
        drawX((int)xstep);
        drawY((int)ystep);
        penDown();
      } else {
        //draw the shit
        drawX((int)xstep);
        drawY((int)ystep);
        //TODO: check if pendown/up commands match/don't break for all combs
      }
      xold = coords[1][0];
      yold = coords[1][1];

    }
    penUp();
    drawBool = false;
  }
}

int blockingRead()
{
  Serial.print("INSIDE BLOCKING READ");
  while (!Serial.available())
  {
    delay(100);
  }
  return convertInputToInt();
  //returns unsigned int, is cast to signed outside
}

void drawX(int n)
{
  Serial.print("Drawing!!X");
  Serial.print(n);
  Serial.print("\n");
  for (int i = 0; i < (int)n; i++)
  {
    xAxis.step(1);
    delay(20);
  }
}

void drawY(int n)
{
  Serial.print("Drawing!!Y");
  Serial.print(n);
  Serial.print("\n");
  for (int i = 0; i < n; i++)
  {
    yAxis.step(1);
    delay(20);
  }
}

int convertInputToInt() {
  Serial.print("inside convertInputToInt function");
  String resultS;
  int resultI;
  char tempChar = Serial.read();
  String inputString = "";
  //assume serial.available
  int i = 0;

  inputString = (char)tempChar;
  Serial.print(inputString);

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
  Serial.print("input that was just read");
  Serial.print(resultI);
  return resultI;
}
void penUp()
{
  penServo.write(70);//pulls pen up
  delay(3000); //pause for 3 seconds
}

void penDown()
{
  penServo.write(0);//pulls pen up
  delay(3000); //pause for 3 seconds
}
