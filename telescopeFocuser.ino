#include "IRremote.h"
#include "telescopeFocuser.h"
#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
int previousSpeed = 50;
int previousDistance = 12;
bool allowPrevious = false;
// for your motor
IRrecv irrecv(RECEIVER);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(60);
  // initialize the serial port:
  Serial.begin(9600);
  irrecv.enableIRIn();
  turnOffStepper();
}

void turnOffStepper()
{
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
}

void moveStepper(int moveLength, int moveSpeed){
  myStepper.setSpeed(moveSpeed);
  myStepper.step(moveLength);
  turnOffStepper();
}


void loop() {
  int tmpValue;
  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    switch(results.value)
    {
      case 0xFF629D:
        previousSpeed = 150;
        previousDistance = 256;
        Serial.print("150,256\n");
        moveStepper(previousDistance, previousSpeed);
        allowPrevious = true;
        break;
      case 0xFFA857:
        previousSpeed = 150;
        previousDistance = -256;
        Serial.print("150,-256\n");
        moveStepper(previousDistance, previousSpeed);
        allowPrevious = true;
        break;
      case 0xFF22DD:
        previousSpeed = 30;
        previousDistance = -12;
        Serial.print("30,-12\n");
        moveStepper(2, previousSpeed);
        allowPrevious = true;
        break;
      case 0xFFC23D:
        previousSpeed = 30;
        previousDistance = 12;
        Serial.print("30,12\n");
        moveStepper(2, previousSpeed);
        allowPrevious = true;
        break;
      case 0xFFFFFFFF:
        Serial.print("previous");
        if (allowPrevious == true)
        {
          moveStepper(previousDistance, previousSpeed);
        }
        break;
      default:
        Serial.print("default\n");
        allowPrevious = false;
    }
    irrecv.resume(); // receive the next value
  }
}
