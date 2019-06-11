#ifndef global_vars_lib
#define global_vars_lib

#include "definitions.h"
#include "includes.h"

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
int captureHours = 1;
int captureMinutes = 1;
uint32_t captureSeconds = 1;
int captureVideoMinutes = 1;
int captureVideoSeconds = 1;
uint32_t captureVideoSecondsTotal = 1; //uint64_t

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
