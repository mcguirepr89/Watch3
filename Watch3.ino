#include <Stepper.h>

const int stepsPerRevolution = 2038;

// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper myStepper = Stepper(stepsPerRevolution, 2, 4, 3, 5);

#define button 6

int state;
int old;
int buttonPoll;

void setup()
{
  Serial.begin(115200);
  pinMode(button, INPUT);
}

void loop()
{
  buttonPoll = digitalRead(button);
  if (buttonPoll == 1) {
    delay(50);
    buttonPoll = digitalRead(button);
    if (buttonPoll == 0) {
      state = old + 1;
      Serial.println("Button Pressed");
      Serial.print("State");
    }
  }
  switch (state) {
    case 1:
      myStepper.setSpeed(10);
      myStepper.step(-stepsPerRevolution / 2038);
      Serial.println(state);
      old = state;
      break;

    case 2:
      myStepper.setSpeed(5);
      myStepper.step(-stepsPerRevolution / 2038);
      Serial.println(state);
      old = state;
      break;

    case 3:
      myStepper.setSpeed(1);
      myStepper.step(-stepsPerRevolution / 2038);
      Serial.println(state);
      old = state;
      break;

    case 4:
      myStepper.setSpeed(10);
      myStepper.step(stepsPerRevolution / 2038);
      Serial.println(state);
      old = state;
      break;

    case 5:
      myStepper.setSpeed(5);
      myStepper.step(stepsPerRevolution / 2038);
      Serial.println(state);
      old = state;
      break;

    case 6:
      myStepper.setSpeed(1);
      myStepper.step(stepsPerRevolution / 2038);
      Serial.println(state);
      old = state;
      break;

    default:
      old = 0;
      break;
  }
}
