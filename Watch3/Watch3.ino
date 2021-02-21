/* Watch3 Watch Winder for Automatic watches -- Patrick McGuire
February 21st, 2021 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTC.h>
#include <Stepper.h>
#include <ezButton.h>

////OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1 //// Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


////Display Power Handler
int displayOld;
int displayPower = 1;

////Watch Winding State Handler
int state;
int old;

////button

ezButton button(2);
const int SHORT_PRESS = 500; //// milliseconds
const int LONG_PRESS  = 500; //// milliseconds
const int REALLY_LONG_PRESS = 2000; //// 2 seconds
const int WINDER_OFF = 3200; //// 5 seconds
unsigned long pressedTime  = 0;
unsigned long releasedTime = 0;
bool isPressing = false;
bool isLongDetected = false;

////Stepper motor
int stepperSpeed;
const int stepsAround = 2038;
//Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper myStepper = Stepper(stepsAround, 4, 6, 5, 7);

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
  thisMinute = RTC.getMinutes();
  lastMinute = RTC.getMinutes();

  //Button
  button.setDebounceTime(50);

  //OLED
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); //// Don't proceed, loop forever
  }
  display.clearDisplay();
  //display.setRotation(2);
}



////////////////////////////////////////////////////////////
void loop() {
  button.loop();                       //// Sets Watch Winder & Display Power States
  if (button.isPressed())
    pressedTime = millis();

  if (button.isReleased()) {
    releasedTime = millis();

    long pressDuration = releasedTime - pressedTime;

    if ( pressDuration < SHORT_PRESS ) {
      Serial.println("A short press is detected");
      state = old + 1;
    }
    if ( pressDuration > LONG_PRESS && pressDuration < REALLY_LONG_PRESS) {
      Serial.println("A long press is detected");
      displayPower = displayOld + 1;
      displayOld = displayPower;
    }
    if ( pressDuration > REALLY_LONG_PRESS && pressDuration < WINDER_OFF) {
      Serial.println("A REALLY long press is detected");
      state = 4;
    } else {
      if ( pressDuration > WINDER_OFF ) {
        Serial.println("Power Down");
        state = 0;
        old = 0;
      }
    }
  }
  
  displayStateChanges();                  //// Display any state changes
  watchWinderStateHandler();              //// Act on any new watch winder states

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

  myStepper.setSpeed(stepperSpeed);       //// Stepper Motor Settings
  if (stepperSpeed > 0 ) {
    myStepper.step(20);
  }
  Serial.print("displayPower = ");        /// Debugging
  Serial.println(displayPower);
  Serial.print("state = ");
  Serial.println(state);
  Serial.print("stepperSpeed = ");
  Serial.println(stepperSpeed);
}
/////////////////////////////////////////////////////////////



//// Display State Changes
void displayStateChanges() {
  if (displayPower == 1) {
    if (state != old) {
      display.clearDisplay();
      display.setTextSize(3);             //// Normal 1:1 pixel scale
      display.setTextColor(SSD1306_WHITE);        //// Draw white text
      display.setCursor(64, 0);
      if (state > 4) {
        display.setCursor(34, 0);
        display.print("Off");
      } else {
        display.print(state);
      }
      display.display();
      delay(1200);
      display.clearDisplay();
    }
  }
}



//// Watch Winder State Handler
void watchWinderStateHandler() {
  switch (state) {
    case 1:
      old = state;
      stepperSpeed = 8;
      break;

    case 2:
      old = state;
      stepperSpeed = 6;
      break;

    case 3:
      old = state;
      stepperSpeed = 3;
      break;

    case 4:
      old = state;
      stepperSpeed = 1;
      break;

    default:
      old = 0;
      state = 0;
      stepperSpeed = 0;
      break;
  }
}

///////////Configuring the time output on the display
void oledTime() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  //////////////////// Date ////////////////////////////////
  switch (RTC.getWeek()) {            //// Day of the Week
    case 1:
      display.print("Sunday, ");
      break;
    case 2:
      display.print("Monday, ");
      break;
    case 3:
      display.print("Tuesday, ");
      break;
    case 4:
      display.print("Wednesday, ");
      break;
    case 5:
      display.print("Thursday, ");
      break;
    case 6:
      display.print("Friday, ");
      break;
    case 7:
      display.print("Saturday, ");
      break;
  }

  switch (RTC.getMonth()) {                //// Month
    case 1:
      display.print("Jan, ");
      break;
    case 2:
      display.print("Feb, ");
      break;
    case 3:
      display.print("March, ");
      break;
    case 4:
      display.print("April, ");
      break;
    case 5:
      display.print("May, ");
      break;
    case 6:
      display.print("June, ");
      break;
    case 7:
      display.print("July, ");
      break;
    case 8:
      display.print("Aug, ");
      break;
    case 9:
      display.print("Sept, ");
      break;
    case 10:
      display.print("Oct, ");
      break;
    case 11:
      display.print("Nov, ");
      break;
    case 12:
      display.print("Dec, ");
      break;
  }

  switch (RTC.getDay()) {                //// Date in calendar form
    case 1:
      display.print("1st");
      break;
    case 2:
      display.print("2nd");
      break;
    case 3:
      display.print("3rd");
      break;
    case 21:
      display.print("21st");
      break;
    case 22:
      display.print("22nd");
      break;
    case 23:
      display.print("23rd");
      break;
    case 31:
      display.print("31st");
      break;
    default:
      display.print(RTC.getDay());
      display.println("th");
      break;
  }
  display.println("");
  display.println("");

  ///////////////////////Time////////////////////////////////
  display.setTextSize(2);             //// Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);

  thisHour = RTC.getHours();          //// Hours space buffer
  if (thisHour < 10) {
    display.print(" ");
  }
  display.print(thisHour);            //// Print Hour
  display.print(":");

  thisMinute = RTC.getMinutes();      //// Clear whole screen every minute
  if ( thisMinute != lastMinute ) {
    lastMinute = thisMinute;
    display.clearDisplay();
  }

  if (thisMinute < 10) {              //// Minutes '0' buffer
    display.print("0");
  }
  display.print(thisMinute);          //// Print minutes
  display.print(":");

  thisSecond = RTC.getSeconds();      //// Clear the seconds only (not entire screen)
  if ( thisSecond != lastSecond ) {
    display.setTextColor(SSD1306_BLACK);
    if (lastSecond < 10) {
      display.print("0");
    }
    display.print(lastSecond);
    lastSecond = thisSecond;
  }
  if (thisSecond < 10) {              //// Seconds '0' buffer
    display.print("0");
  }
  display.print(thisSecond);

  if (RTC.getHourMode() == CLOCK_H12) //// Set 'am' or 'pm'
  {
    switch (RTC.getMeridiem()) {
      case HOUR_AM :
        display.print("am");
        break;
      case HOUR_PM :
        display.print("pm");

        break;
    }
  }
  display.display();                  //// Finally, display everything on the screen
}
