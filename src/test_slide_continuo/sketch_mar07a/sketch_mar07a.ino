
#include <AccelStepper.h>
int PUL=10; //define Pulse pin
int DIR=9; //define Direction pin
int ENA=8; //define Enable Pin

AccelStepper stepper(ENA, PUL, DIR); // en pulse digital
void setup()
{  
   stepper.setMaxSpeed(-1000);
   stepper.moveTo(1000); 
   stepper.setSpeed(-1000); 
         
}
void loop()
{  
   stepper.run();
}
