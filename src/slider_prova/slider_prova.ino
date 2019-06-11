#include <SoftwareSerial.h>
#include <AccelStepper.h>

#define btnPin1 7
#define btnPin2 6




void setup()
{
  //pin mode
  pinMode(btnPin1, INPUT);
  pinMode(btnPin2, INPUT);

  // Debug console
  Serial.begin(9600);

  //myServo.attach(servoPin);
}
int pin1Status=0;
int pin2Status=0;
void loop()
{
  pin1Status = digitalRead(btnPin1);
  pin2Status = digitalRead(btnPin2);
  Serial.println(pin2Status);
 
}

