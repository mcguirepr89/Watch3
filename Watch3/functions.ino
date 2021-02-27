////Interrupt State Changer
void stateChange() {
  {
    volatile static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    if (interrupt_time - last_interrupt_time > 80 )  // ignores interupts for 80 milliseconds
    {
      state = old + 1;
    }
    last_interrupt_time = interrupt_time;
  }
}

////Interrupt State Changer
void displayStateChange() {
  {
    volatile static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    if (interrupt_time - last_interrupt_time > 80 )  // ignores interupts for 80 milliseconds
    {
      displayPower = displayOld + 1;
      displayOld = displayPower;
    }
    last_interrupt_time = interrupt_time;
  }
}

//// Watch Winder State Handler
void watchWinderStateHandler() {
  switch (state) {
    case 1:
      old = state;
      stepperSpeed = 1;
      break;

    case 2:
      old = state;
      stepperSpeed = 3;
      break;

    case 4:
      old = state;
      stepperSpeed = 6;
      break;
      
    case 3:
      old = state;
      stepperSpeed = 9;
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
