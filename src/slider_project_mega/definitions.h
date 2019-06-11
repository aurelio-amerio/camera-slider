#ifndef pinDefinitions
#define pinsDefinitions

//
#define seconds() (millis()/1000)
#define BLYNK_PRINT Serial

// Hardware Serial on Mega, Leonardo, Micro...
#define EspSerial Serial3

// Your ESP8266 baud rate:
#define ESP8266_BAUD 115200

#define TXD 11
#define RXD 12

#define PUL 10 //define Pulse pin
#define DIR 9 //define Direction pin
#define ENA 8 //define Enable Pin

#define menueMode V0
#define menueStepPin V1
#define menueExpositionTimePin V2

//Pin per la calibrazione
#define calibSpeedSliderPin V5 //il pin dello slider per la velocità nella calibrazione degli estremi
#define calibLeftEndPin V6
#define calibRightEndPin V7

#define calibLeftDirPin V8
#define calibRightDirPin V9
#define calibLCDPin V10

//pin per lo stop motion
#define captureLCD1Pin V20
#define captureProgressPin V21
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

#endif
