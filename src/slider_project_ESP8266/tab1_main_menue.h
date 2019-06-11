#ifndef tab1_main_menue
#define tab1_main_menue

#include "definitions.h"
#include "includes.h"
#include "global_vars.h"
#include "tab2_calib.h"
#include "tab3_stop_motion.h"

//main menue
BLYNK_WRITE(menueMode) //seleziona Modalità
{
  switch (param.asInt())
  {
  case 1: //Calibrazione
    mode = 1;
    calibration_color_enable();
    calibLCD.clear();
    calibLCD.print(0, 0, "Select an end");
    calibLCD.print(0, 1, "to calibrate");
    break;
  case 2: //Stop Motion
    mode = 2;
    calibration_color_disable();
    captureResetHoldTime = 0;
    captureLCD1.clear();
    captureLCD2.clear();
    printTotalTime();
    printVideoTime();
    break;
  case 3: // Item 3
    mode = 3;
    calibration_color_disable();
    break;
  default:
    Serial.println("Selezionare Modalità");
  }
  //pulisco gli schermi quando cambio modalità
  /*calibLCD.clear();
    captureLCD1.clear();
    captureLCD2.clear();*/
}

BLYNK_WRITE(menueStepPin) //seleziona step/giro
{
  switch (param.asInt())
  {
  case 1: // Item 1
    stepsRev = 200;
    speedMult = 1;
    break;
  case 2: // Item 2
    stepsRev = 1600;
    speedMult = 8;
    break;
  case 3: // Item 3
    stepsRev = 3200;
    speedMult = 16;
    break;
  case 4: // Item 4
    stepsRev = 6400;
    speedMult = 32;
    break;
  default:
    Serial.println("Selezionare Steps");
  }
  maxSpeedVal = baseSpeed * speedMult;
  maxAccelerationVal = baseSpeed * speedMult;
  calibStepVal = maxSpeedVal * 10;
  stepper.setMaxSpeed(maxSpeedVal);
  stepper.setAcceleration(maxAccelerationVal);
}

BLYNK_WRITE(menueExpositionTimePin)
{
  expositionTime = param.asInt();
}

#endif