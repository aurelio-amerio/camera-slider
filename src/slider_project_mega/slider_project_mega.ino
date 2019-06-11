#include "definitions.h"
#include "includes.h"
#include "global_vars.h"
#include "tab1_main_menue.h"
#include "tab2_calib.h"
#include "tab3_stop_motion.h"

BlynkTimer calibTimer;
StopMotionMove stopMotion;
//inizio del codice blynk

void setup()
{
  // Debug console
  Serial.begin(9600);

  //SerialBLE.begin(9600);
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  
  //Blynk.begin(SerialBLE, auth);

  //Serial.println("Waiting for connections...");

   Blynk.begin(auth, wifi, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, wifi, ssid, pass, "blynk-cloud.com", 8442);
  //Blynk.begin(auth, wifi, ssid, pass, IPAddress(192,168,1,100), 8442);


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
    if (captureRunning == 0 && captureStart == 1)
    {
      stopMotion.initialize();
      captureRunning = 1;
    }
    else if (captureRunning == 1 && captureStart == 1)
    {
      stopMotion.run();
    }
    else
    {
      //sto fermo, non devo fare nulla
    }
    stepper.run();
  }
  calibTimer.run();
  //connessione
  Blynk.run();
}
