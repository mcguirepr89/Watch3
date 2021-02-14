#include <Stepper.h>

const int stepsPerRevolution = 2038;

// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper myStepper = Stepper(stepsPerRevolution, 2, 4, 3, 5);

void setup() {
  // Nothing to do (Stepper Library sets pins as outputs)
}

void loop() {
  
  // Rotate CCW
  myStepper.setSpeed(1);
  myStepper.step(-stepsPerRevolution);
  delay(5);
// Rotate CW
  myStepper.setSpeed(1);
  myStepper.step(stepsPerRevolution);
  delay(5);
  
}
