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

String firstCoords;
String inputString;

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
    delay(3000);

    Serial.print("@GetNumCoords");
    Serial.flush();
    int numCoords = blockingRead();
    xAxis.setSpeed(100);
    yAxis.setSpeed(100);

    //sets up position, move to first coord
    Serial.print("@GetNext"); //anything that is a command shouldn't have a \n??
    Serial.flush();
    //delay(250);
    firstCoords = blockingRead();

    currentX[0] = convertInputToString(firstCoords);
    currentY[0] = convertInputToString(firstCoords);

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
      /*
        inputString stores a large batch of string at the same time,
        since communication between the imageProcessing program and the
        imageDrawing program is very time consuming (wait for getNext, read
        and write, for every single coordinate at the time)
      */

      Serial.print("@GetNext"); //gives order to imageProcessing.py
      Serial.flush();
      delay(250);
      inputString = blockingRead();
      //python sends in next 49 coords
      
      for (int k = 0; k < 50; k++) {
        if (inputString != "") {
          currentX[k] = convertInputToInt(inputString);
          currentY[k] = convertInputToInt(inputString);
          i++;
        } else {
          break; //only case when this happens is when there's less than 49 coords available, towards the end of the program
        }
      }
      
      for (int k = 0; k < 50; k++) {

        //TODO: if there's less than 50, i.e. on the last loop, make it exit properly
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

int blockingRead() //choice for which serial input
{
  while (!Serial.available())
  {
    delay(100);
  }
  return getSerialInput();
}

void drawX(int n)
{
  if (n > 0) {
    for (int i = 0; i < (int)n; i++)
    {
      xAxis.step(1);
      delay(10);
    }
  } else if (n < 0) {
    for (int i = 0; i > (int)n; i--)
    {
      xAxis.step(-1);
      delay(10);
    }
  } else {
    //Serial.print("No x step\n");
  }
}

String getSerialInput() { //reads until long
  char tempChar = "";
  tempChar = Serial.read();
  while (tempChar != ' ' && tempChar != '\n') {
    inputString += (char)tempChar;
    tempChar = Serial.read(); //if reads in
    if (tempChar = "!") { //seed some ! from inside python
      return inputString;
    }
  }
}

void drawY(int n)
{
  if (n > 0) {
    for (int i = 0; i < (int)n; i++)
    {
      yAxis.step(1);
      delay(10);
    }
  } else if (n < 0) {
    for (int i = 0; i > (int)n; i--)
    {
      yAxis.step(-1);
      delay(10);
    }
  } else {
    //Serial.print("No y step\n");
  }
}


int convertInputToInt(String inputString) { //splits string, converts to int
  String newInputString = "";
  bool found = false;
  int intCoord;
  String parsedCoord;
  for (int i = 0; i < inputString.length(); i++)
  {
    if (found == false) {
      if (inputString[i] == ".") {
        found = true;
      }
      else if (inputString[i] >= 48 && inputString[i] <= 57) //is a numerical val
      {
        parsedCoord += inputString;
      }
    } else { //found is true, transfer the rest into a new inputString
      if (newInputString == "") {
        //skip, clean out front, only save if it's a num
        if (inputString[i] >= 48 && inputString[i] <= 57) {
          newInputString += inputString;
        }
      } else {//just fill the new string up
        newInputString += inputString;
      }
    }
  }
  intCoord = parsedCoord.toInt();
  inputString = newInputString;
  return intCoord;
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
