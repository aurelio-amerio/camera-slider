#define BLYNK_PRINT Serial

#define TXD 11
#define RXD 12

#include <AccelStepper.h>
#include <SoftwareSerial.h>



SoftwareSerial SwSerial(TXD, RXD); // TXD, RXD

#include <BlynkSimpleSerialBLE.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "24ee45c3838a41a091d79864ad77702c"; //online
//char auth[] = "b480210566ac409084bb68e27b3de193"; //local




SoftwareSerial SerialBLE(TXD, RXD); // TXD, RXD

#define PUL 10 //define Pulse pin
#define DIR 9 //define Direction pin
#define ENA 8 //define Enable Pin

#define menueMode V0
#define menueStepPin V1

//Pin per la calibrazione
#define calibSpeedSliderPin V5 //il pin dello slider per la velocità nella calibrazione degli estremi
#define calibLeftEndPin V6
#define calibRightEndPin V7

#define calibLeftDirPin V8
#define calibRightDirPin V9
#define calibLCDPin V10

//pin per lo stop motion
#define captureLCD1Pin V20
#define captureProgressa V21
#define captureLCD2Pin V22
#define captureTotalHoursPin V23 //hours of capture duration
#define captureTotalMinutesPin V24 //minutes of capture duration
#define captureVideoMinutesPin V25 //hours, how long should the video last
#define captureVideoSecondsPin V26 //minutes

#define captureStartPin V27 //start capture pin
#define captureResetPositionPin V28 //reset to initial position
#define capturePausePin V29 //reset to initial position

//colors
#define BLYNK_GREEN     "#23C48E"
#define BLYNK_BLUE      "#04C0F8"
#define BLYNK_YELLOW    "#ED9D00"
#define BLYNK_RED       "#D3435C"
#define BLYNK_DARK_BLUE "#5F7CD8"
#define BLYNK_GRAY      "#8C8C8C"





AccelStepper stepper(AccelStepper::DRIVER, PUL, DIR); //pulse digital
BlynkTimer calibTimer;

int mode = 0;
int rightEnd = 0;
int leftEnd = 0;

int stepsRev = 200;
int speedMult = 1;
int baseSpeed = 312; //lo si può aumentare, questo è un valore di sicurezza
int maxSpeedVal = baseSpeed;
int maxAccelerationVal = baseSpeed;

//variabili per la calibrazione
int calibMovingLeftEnd = 0;
int calibMovingRightEnd = 0;
int calibLeftDir = 0;
int calibRightDir = 0;
int calibPrint = 0;
int calibSpeedPercentage = 50;
int currentPositionVal = 0;
int calibCurrentSpeedVal = baseSpeed;
int calibStepVal = maxSpeedVal;

WidgetLCD calibLCD(calibLCDPin);

//variabili per lo stop motion
int captureFPS = 25;
int captureHours = 0;
int captureMinutes = 0;
uint64_t captureSeconds = 0;
int captureVideoMinutes = 0;
int captureVideoSeconds = 0;
int captureVideoSecondsTotal = 0;
int captureShots = 0;
int captureProgress = 0;
int captureStart = 0;
int captureResetPosition = 0;
int capturePause = 0;
bool captureResettingFlag = 0;

int captureResetHoldTimeStart = 0;
int captureResetHoldTime = 0;

WidgetLCD captureLCD1(captureLCD1Pin);
WidgetLCD captureLCD2(captureLCD2Pin);

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


//inizio del codice blynk
//function prototype
void calibration_color_enable();
void calibration_color_disable();
void printTotalTime();
void printVideoTime();
void resetPositionLoop();
void doCalibLoop(int positionLeft);

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

//stop motion
/*int captureFPS = 25;
  int captureHours = 0;
  int captureMinutes = 0;
  int captureSeconds = 0;
  int captureVideoMinutes = 0;
  int captureVideoSeconds = 0;
  int captureShots = 0;
  int captureProgress = 0;
  int captureStart = 0;
  int captureResetPosition = 0;
  int capturePause = 0;
  bool captureResettingFlag = 0;*/


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
    captureVideoSecondsTotal = captureVideoMinutes * 60 + captureVideoSeconds ;
    printVideoTime();

  }
}
BLYNK_WRITE(captureVideoSecondsPin)
{
  if (mode == 2)
  {
    captureVideoSeconds = param.asInt();
    captureVideoSecondsTotal = captureVideoMinutes * 60 + captureVideoSeconds ;
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

  if (captureResettingFlag == 1 && stepper.currentPosition() == leftEnd && stepper.distanceToGo() == 0 )
  {
    captureResettingFlag = 0;
    captureLCD2.clear();
    printVideoTime();
    printTotalTime();

  }
}
//end of stop motion

void setup()
{
  // Debug console
  Serial.begin(9600);

  SerialBLE.begin(9600);
  Blynk.begin(SerialBLE, auth);

  Serial.println("Waiting for connections...");

  //setto il pin Enabler
  stepper.setEnablePin(ENA);
  //stepper.setMaxSpeed(maxSpeedVal);
  //stepper.setAcceleration(maxAccelerationVal);

  //inizializzo la posizione
  currentPositionVal = stepper.currentPosition();

  //inizializzo il calibTimer
  calibTimer.setInterval(3000L, calib_print_info);

  //inizializzo setup tab
  //Blynk.virtualWrite(calibSpeedSliderPin, 25); //set speed to 25%

  //inizializzo caputure tab
  //Blynk.virtualWrite(captureTotalHoursPin, 0);
  //Blynk.virtualWrite(captureTotalMinutesPin, 0);

  //pulisco gli schermi
  calibLCD.clear();
  captureLCD1.clear();
  captureLCD2.clear();

  //svuoto la tab di calibrazione
  calibration_color_disable();

}


void loop()
{
  int distanceLeft = stepper.distanceToGo();


  if (mode == 1)
  {
    doCalibLoop(distanceLeft);
    stepper.runSpeedToPosition();
  }

  if (mode == 2)
  {
    resetPositionLoop();
    stepper.run();

  }
  calibTimer.run();
  //connessione
  Blynk.run();
}
