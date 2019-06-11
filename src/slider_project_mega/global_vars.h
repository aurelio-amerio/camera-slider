#ifndef global_vars_lib
#define global_vars_lib

#include "definitions.h"
#include "includes.h"

SoftwareSerial SwSerial(TXD, RXD); // TXD, RXD
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "24ee45c3838a41a091d79864ad77702c"; //online
//char auth[] = "b480210566ac409084bb68e27b3de193"; //local

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "ACasa_2GHz";
char pass[] = "topoc1ccioArduino2017";

ESP8266 wifi(&EspSerial);

SoftwareSerial SerialBLE(TXD, RXD); // TXD, RXD


AccelStepper stepper(AccelStepper::DRIVER, PUL, DIR); //pulse digital

int mode = 0;
int rightEnd = 0;       //posizione finale
int leftEnd = 0;        //posizione iniziale
int expositionTime = 1; //in seconds, can be more but not less

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
uint64_t captureVideoSecondsTotal = 0;

int captureProgress = 0;
int captureStart = 0;
int captureRunning = 0;
int captureResetPosition = 0;
int capturePause = 0;
bool captureResettingFlag = 0;


int captureResetHoldTimeStart = 0;
int captureResetHoldTime = 0;

WidgetLCD captureLCD1(captureLCD1Pin);
WidgetLCD captureLCD2(captureLCD2Pin);

int stabilization_seconds = 1; //secondi da stare fermi prima di fare una foto

#endif
