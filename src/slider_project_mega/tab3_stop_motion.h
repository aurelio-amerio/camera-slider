#ifndef tab3_stop_motion
#define tab3_stop_motion

#include "definitions.h"
#include "includes.h"
#include "global_vars.h"

//stop motion

//prototypes
void printTotalTime();
void printVideoTime();
void resetPositionLoop();

//capture

void printTotalTime()
{
  String textToPrint = captureHours + String("h ") + captureMinutes + String("min          ");
  captureLCD1.print(0, 0, "Total time:");
  //captureLCD1.print(0, 1, "              ");
  captureLCD1.print(0, 1, textToPrint);
  //Serial.println(textToPrint);
}
BLYNK_WRITE(captureTotalHoursPin)
{
  Serial.println(param.asInt());
  if (mode == 2)
  {
    captureHours = param.asInt();
    captureSeconds = captureHours * 3600 + captureMinutes * 60;
    printTotalTime();
  }
}

BLYNK_WRITE(captureTotalMinutesPin)
{
  Serial.println(param.asInt());
  if (mode == 2)
  {
    captureMinutes = param.asInt();
    captureSeconds = captureHours * 3600 + captureMinutes * 60;
    printTotalTime();
  }
}

//video

void printVideoTime()
{
  Blynk.setProperty(captureLCD2Pin, "color", BLYNK_YELLOW);
  String textToPrint = captureVideoMinutes + String("min ") + captureVideoSeconds + String("s          ");
  captureLCD2.print(0, 0, "Video time:");
  //captureLCD1.print(0, 1, "              ");
  captureLCD2.print(0, 1, textToPrint);
  //Serial.println(textToPrint);
}

BLYNK_WRITE(captureVideoMinutesPin)
{
  if (mode == 2)
  {
    captureVideoMinutes = param.asInt();
    captureVideoSecondsTotal = captureVideoMinutes * 60 + captureVideoSeconds;
    printVideoTime();
  }
}
BLYNK_WRITE(captureVideoSecondsPin)
{
  if (mode == 2)
  {
    captureVideoSeconds = param.asInt();
    captureVideoSecondsTotal = captureVideoMinutes * 60 + captureVideoSeconds;
    printVideoTime();
  }
}

//buttons
BLYNK_WRITE(captureStartPin)
{
  if (mode == 2)
  {
    captureStart = param.asInt();
  }

  if (param.asInt() == 0)
  {
    captureRunning = 0;
  }
}

BLYNK_WRITE(capturePausePin)
{
  if (mode == 2)
  {
    capturePause = param.asInt();
  }
}

void reset_position()
{
  captureResettingFlag = 1;
  captureResetHoldTime = 0;
  stepper.moveTo(leftEnd);
  //Serial.print("Resetting position");
  Blynk.setProperty(captureLCD2Pin, "color", BLYNK_RED);
  captureLCD2.clear();
  captureLCD2.print(0, 0, "Repositioning.");
  captureLCD2.print(0, 1, "Please wait...");
}

BLYNK_WRITE(captureResetPositionPin)
{

  if (mode == 2)
  {
    if (captureResetPosition == 0 && param.asInt() == 1)
    {
      Serial.println("start time");
      captureResetPosition = 1;
      captureResetHoldTimeStart = millis();
      captureResetHoldTime = 0;
    }
    else
    {
      captureResetPosition = param.asInt();
    }
    if (captureResetPosition == 1 && captureResettingFlag == 0)
    {
      captureLCD2.clear();
      Blynk.setProperty(captureLCD2Pin, "color", BLYNK_RED);
      captureLCD2.print(0, 0, "Hold button");
      captureLCD2.print(0, 1, "5s to reset");
    }
  }
}

void resetPositionLoop()
{
  //funzioni per resettare la posizione
  if (captureResetPosition == 1)
  {
    //Serial.println("sto premento il pulsante");
    captureResetHoldTime = millis() - captureResetHoldTimeStart;

    if (captureResetHoldTime > 5500)
    {
      captureResetHoldTime = 0;
    }
  }
  //dovrei mettere queste funzioni in un timer, forse

  if (captureResettingFlag == 0 && captureResetHoldTime >= 5000 /*&& stepper.currentPosition() != leftEnd && stepper.distanceToGo() == 0*/)
  {
    reset_position();
  }

  if (captureResettingFlag == 1 && stepper.currentPosition() == leftEnd && stepper.distanceToGo() == 0)
  {
    captureResettingFlag = 0;
    captureLCD2.clear();
    printVideoTime();
    printTotalTime();
  }
}

class StopMotionMove
{
public:
  //COSTRUCTOR
  StopMotionMove(int captureFPS = captureFPS, uint64_t captureSeconds = captureSeconds, uint64_t captureVideoSecondsTotal = captureVideoSecondsTotal,
                 int stabilization_seconds = stabilization_seconds)
  {
    initialize();
  }

  void initialize()
  {
    stepper.moveTo(leftEnd);
    init_movement = true;
    takeFirstPhoto = true;
    number_of_shots = captureVideoSecondsTotal * captureFPS;
    dt = captureSeconds / number_of_shots;
    movement_dt = dt - stabilization_seconds;
    exposition_dt = expositionTime;
    dx = abs((leftEnd - rightEnd) / number_of_shots);
    int minSpeedTmp = dx / (movement_dt - 2 - exposition_dt); //il 2 è per tenere conto dell'accelerazione, se è il caso aumentare

    if (minSpeedTmp > maxSpeedVal)
    {
      init_succesfull = 0;
      Serial.print("errore, non posso fare tutte queste foto in così poco tempo!");
      Blynk.virtualWrite(captureStartPin, 0);
      captureStart = 0;
    }
    else
    {
      init_succesfull = 1;
      if (minSpeedTmp > maxSpeedVal / 2)
      {
        stepper.setMaxSpeed(minSpeedTmp);
        stepper.setAcceleration(1.5 * minSpeedTmp);
      }
      else
      {
        stepper.setMaxSpeed(maxSpeedVal / 2);
        stepper.setAcceleration(1.5 * maxSpeedVal / 2);
      }
    }
  }

  void shoot_photo()
  {
    //do something
    Serial.println("scatto foto");
    travelledDistance += dx;
    progressPercentage = travelledDistance / abs(leftEnd - rightEnd);
  }

  int distanceToGo()
  {
    return stepper.distanceToGo();
  }

  void run()
  {
    if (init_succesfull == 1 && current_position != rightEnd)
    {
      if (stepper.distanceToGo() == 0)
      {

        current_position = stepper.currentPosition();

        if (init_movement == true)
        {
          initial_time = seconds();
          init_movement = 0;
          passed_time = 0;
          shoot = true;

          if (takeFirstPhoto == true)
          {
            shoot_photo();
            takeFirstPhoto = false;
          }

          delay(exposition_dt);
          stepper.move(dx);
        }
        else
        {
          passed_time = seconds() - initial_time;
        }

        if (passed_time >= dt && shoot == true && stepper.distanceToGo() == 0)
        {
          shoot_photo();
          init_movement = 1;
          shoot = false;
        }
      }
      else
      {
        //wait for the stepper to reach the right position
      }
    }
    else if (init_succesfull == 1 && current_position == rightEnd)
    {
      Serial.print("\nho raggiunto la fine dello stop motion");
    }
    else
    {
      initialize();
    }
    //progress timer
    
      if (millis() - initUpdateTime > 500)
      {
        initUpdateTime = millis();
        update_progress();
      }
    
  }

private:
  bool init_succesfull;
  bool init_movement = true;
  bool shoot = true;
  int current_position;
  long int initial_time;
  int passed_time;
  int dt;
  int movement_dt;
  int exposition_dt;
  int dx;
  bool takeFirstPhoto = true;
  long int number_of_shots;
  double progressPercentage = 0;
  int travelledDistance = 0;
  long int initUpdateTime = 0;

  void update_progress()
  {
    Blynk.virtualWrite(captureProgressPin, progressPercentage);
  }

  void move_to_next_step()
  {
    stepper.move(dx);
  }
};

//end of stop motion

#endif