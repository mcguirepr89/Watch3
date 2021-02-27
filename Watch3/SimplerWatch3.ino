#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTC.h>
#include <Stepper.h>

////OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1 //// Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

////Display Power State Handler
int displayOld = 1;
int displayPower = 1;

////Button
#define button 2

////Latch
#define latch 3

////Stepper motor
int stepperSpeed;
int stepsAround = 2038;
//Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper myStepper = Stepper(stepsAround, 4, 6, 5, 7);


////Watch Winding State Handler
int state;
int old;

////Real Time Clock
static DS3231 RTC;
int thisSecond;
int lastSecond;
int thisMinute;
int lastMinute;
int thisHour;
int lastHour;

void setup() {
  Serial.begin(9600);
  //RTC
  RTC.begin();

  //Button
  pinMode(button, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(button), stateChange, FALLING);

  //Latch
  pinMode(latch, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(latch), displayStateChange, CHANGE);

  //OLED
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  //display.setRotation(2);
}

void loop() {
  if (displayPower == 0) {                //// Detect OLED Power State
    Serial.println("Display Off");
  } else {
    if (displayPower == 1) {
      oledTime();
    } else {
      if (displayPower >= 2) {
        display.clearDisplay();
        display.display();
        displayPower = 0;
        displayOld = 0;
      }
    }
  }

  watchWinderStateHandler();           //// Set Watch Winder State

  if (stepperSpeed > 0) {              //// Move the motor
    myStepper.setSpeed(stepperSpeed);
    myStepper.step(stepsAround / 100);
  }

  thisMinute = RTC.getMinutes();      //// Change rotation direction every minute
  if ( thisMinute != lastMinute ) {
    lastMinute = thisMinute;
    stepsAround = stepsAround * -1;
    display.clearDisplay();
  }
}
