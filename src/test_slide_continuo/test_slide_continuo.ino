#define BLYNK_PRINT Serial

#define TXD 5
#define RXD 6

#include <AccelStepper.h>
#include <SoftwareSerial.h>

//SoftwareSerial SwSerial(TXD, RXD); // TXD, RXD

#include <BlynkSimpleSerialBLE.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "24ee45c3838a41a091d79864ad77702c";

SoftwareSerial SerialBLE(TXD, RXD); // TXD, RXD

#define PUL 10 //define Pulse pin
#define DIR 9 //define Direction pin
#define ENA 8 //define Enable Pin

int btn1=0;
int btn2=0;

int currentPositionVal = 0;
int targetPositionVal = 0;
int maxSpeedVal = 5000;
int maxAccelerationVal = 5000;
int stepVal=500;

AccelStepper stepper(AccelStepper::DRIVER, PUL, DIR); //pulse digital

BlynkTimer timer;

// a function to be executed periodically
void repeatMe() {
    Serial.print("Posizione corrente: ");
    Serial.println(stepper.currentPosition());
}


void setup()
{
  // Debug console
  Serial.begin(9600);

  SerialBLE.begin(9600);
  Blynk.begin(SerialBLE, auth);

  Serial.println("Waiting for connections...");

  
  //setto il pin Enabler
  stepper.setEnablePin(ENA);
  
  pinMode(btn1Pin, INPUT);
  pinMode(btn2Pin, INPUT);

  Serial.begin(9600);

  stepper.setMaxSpeed(maxSpeedVal);
  stepper.setAcceleration(maxAccelerationVal);
  //stepper.moveTo(targetPositionVal);
  
  //inizializzo la posizione
  currentPositionVal=stepper.currentPosition();

  //inizializzo il timer
  timer.setInterval(1000, repeatMe);
}

void loop()
{
  btn1=digitalRead(btn1Pin);
  btn2=digitalRead(btn2Pin);
  if((btn1==1||btn2==1)/*&&stepper.distanceToGo()==0*/)
  {
    if(btn1==1&&btn2==1)
    {
      stepper.stop();
    }
    else if(btn1==1)
    {
      stepper.move(stepVal);
    }
     else if(btn2==1)
    {
      stepper.move(-stepVal);
    }
    else
    {
      stepper.stop();
    }
    
  }
  Blynk.run();
  stepper.run();
  //timer.run();
}
