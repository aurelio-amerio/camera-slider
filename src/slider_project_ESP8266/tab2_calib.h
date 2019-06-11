#ifndef tab2_calib
#define tab2_calib

#include "definitions.h"
#include "includes.h"
#include "global_vars.h"



//prototypes

void calibration_color_enable();
void calibration_color_disable();
void doCalibLoop(int positionLeft);


class Tab2_Stop_Motion
{
public:
Tab2_Stop_Motion()
{}
};
// print calibration info
void calib_print_info() {

  Serial.print("Posizione corrente: ");
  Serial.println(stepper.currentPosition());
  //Serial.print("quanto manca: ");
  //Serial.println(stepper.distanceToGo());
  Serial.print("right end: ");
  Serial.println(rightEnd);
  Serial.print("left end: ");
  Serial.println(leftEnd);

}

//CALIBRAZIONE
BLYNK_WRITE(calibSpeedSliderPin)
{
  if (mode == 1)
  {
    calibSpeedPercentage = param.asInt();
    calibCurrentSpeedVal = floor(baseSpeed * 0.01 * calibSpeedPercentage * speedMult);
    Serial.println(calibCurrentSpeedVal);
  }
}

//Which end am I calibrating?
BLYNK_WRITE(calibLeftEndPin)
{
  if (mode == 1)
  {
    calibMovingLeftEnd = param.asInt();
    calibPrint = 0;
    //Serial.print(calibMovingLeftEnd);
  }
}
BLYNK_WRITE(calibRightEndPin)
{
  if (mode == 1)
  {
    calibMovingRightEnd = param.asInt();
    calibPrint = 0;
  }
}

//Which dir should I go?
BLYNK_WRITE(calibLeftDirPin)
{
  if (mode == 1)
  {
    calibLeftDir = param.asInt();
  }
}

BLYNK_WRITE(calibRightDirPin)
{
  if (mode == 1)
  {
    calibRightDir = param.asInt();
  }
}
//funzioni per la calibrazione
//funzioni che cambiano i colori dei pulsanti di calibrazione
void calibration_color_enable()
{
  Blynk.setProperty(calibLeftDirPin, "color", BLYNK_YELLOW);
  Blynk.setProperty(calibRightDirPin, "color", BLYNK_YELLOW);
  Blynk.setProperty(calibLeftEndPin, "color", BLYNK_GREEN);
  Blynk.setProperty(calibRightEndPin, "color", BLYNK_GREEN);
  Blynk.setProperty(calibSpeedSliderPin, "color", BLYNK_GREEN);
  Blynk.setProperty(calibLCDPin, "color", BLYNK_GREEN);
  calibLCD.clear();
  //calibLCD.print(0, 0, "Select an end");
  //calibLCD.print(0, 1, "to calibrate");
}

void calibration_color_disable()
{
  Blynk.setProperty(calibLeftDirPin, "color", BLYNK_GRAY);
  Blynk.setProperty(calibRightDirPin, "color", BLYNK_GRAY);
  Blynk.setProperty(calibLeftEndPin, "color", BLYNK_GRAY);
  Blynk.setProperty(calibRightEndPin, "color", BLYNK_GRAY);
  Blynk.setProperty(calibSpeedSliderPin, "color", BLYNK_GRAY);
  Blynk.setProperty(calibLCDPin, "color", BLYNK_GRAY);

  calibLCD.clear();
  calibLCD.print(0, 0, "Tab");
  calibLCD.print(0, 1, "Disabled");
}

//loop da mettere nel main
void doCalibLoop(int distanceLeft)
{
  //select which end to calibrate
  if (calibMovingLeftEnd == 1 && calibMovingRightEnd == 0)
  {
    leftEnd = stepper.currentPosition();
    if (calibPrint == 0)
    {
      calibPrint = 1;
      calibLCD.clear();
      calibLCD.print(0, 0, "Calibrating");
      calibLCD.print(0, 1, "Left End");
    }
  }
  else if (calibMovingLeftEnd == 0 && calibMovingRightEnd == 1)
  {
    rightEnd = stepper.currentPosition();
    if (calibPrint == 0)
    {
      calibPrint = 1;
      calibLCD.clear();
      calibLCD.print(0, 0, "Calibrating");
      calibLCD.print(0, 1, "Right End");
    }
  }
  else
  {
    if (calibPrint == 0)
    {
      calibPrint = 1;
      calibLCD.clear();
      calibLCD.print(0, 0, "Select an end");
      calibLCD.print(0, 1, "to calibrate");
    }

  }

  //move according to button pression
  if (calibLeftDir == 1 && calibRightDir == 0)
  {
    if (distanceLeft == 0)
      stepper.move(calibStepVal);
    stepper.setSpeed(calibCurrentSpeedVal);
  }
  else if (calibLeftDir == 0 && calibRightDir == 1)
  {
    if (distanceLeft == 0)
      stepper.move(-calibStepVal);
    stepper.setSpeed(-calibCurrentSpeedVal);
  }
  else
  {
    stepper.move(0);
  }
}

//end of calibration

#endif