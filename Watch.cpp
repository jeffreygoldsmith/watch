//
// Include Libraries.
//
#include "Arduino.h"
#include "Watch.h"
#include <Sodaq_DS3231.h>


struct tm tm;


static const byte BUTTONPIN_1 = 13; // Button 1 input pin
static const byte BUTTONPIN_2 = 12; // Button 2 input pin

long counter1 = 0;
long counter2 = 0;
long counter1Prev;
long counter2Prev;

unsigned long pressMillis;
bool firstPress;
bool firstData;

unsigned long unixPrev;

//bool button1; // Latest button 1 value //! MAYBE NOT NEEDED -- DECLARED IN .H FILE?
//bool button2; // Latest button 2 value
//bool button1Prev; // Lagging button 1 value
//bool button2Prev; // Lagging button 2 value


//
// Function to take time_t variable and decode into individual measurements of time.
//
struct tm Decode(unsigned long ts)
{
  int d, m, y;
  struct tm tm;

  tm.s = ts % 60; // Seconds
  ts /= 60;

  tm.m = ts % 60; // Minutes
  ts /= 60;

  tm.h = ts % 24; // Hours
  ts /= 24;

  ts = ts * 100 + 36525 * 2 - 5950; // Move start of epoch to 1 March 1968 and scale
  y = (ts + 75) / 36525; // Years since start of epoch
  d = (175 + ts - y * 36525) / 100 * 10; // Days since start of year (times 10)
  m = (d - 6) / 306; // Months since start of year

  tm.y = 1968 + y + (m >= 10); // Year
  tm.mon = 1 + (m + 2) % 12; // Month
  tm.d = (4 + d - m * 306) / 10; // Day
  tm.wd = 1 + (5 + ts / 100) % 7; // Day of week (Sunday = 1)

  return tm;
}


//
// Class to control buttons
//

//
// Button::Button() -- Class constructor
//
Button::Button()
{
  pinMode(BUTTONPIN_1, INPUT);
  pinMode(BUTTONPIN_2, INPUT);
}

//
// Button::TakeInput() -- Take readings from buttons
//
void Button::TakeInput()
{
  button1 = digitalRead(BUTTONPIN_1); // Take in new readings from buttons
  button2 = digitalRead(BUTTONPIN_2);

  if (button1 == false && button1Prev == true) // Check if first button state has changed
    counter1++; // Increment counter

  if (button2 == false && button2Prev == true) // Check if second button state has changed
    counter2++; // Increment counter

  counter1Prev = counter;
  button1Prev = button1; // Update lagging values
  button2Prev = button2;
}


//
// Class to control time and RTC.
//

//
// Time::Time() -- Class constructor
//
Time::Time()
{
  rtc.begin(); // Initialize RTC
}


//
// Function to initialize RTC.
//
void Time::Sync()
{
  DateTime now = rtc.now(); // Take reading from RTC and update current time
  unixPrev = now.getEpoch(); // Set lagging value of unix time to be initial unix time
}


//
// Time::UpdateTime() -- Update time based on readings from RTC
//
void Time::UpdateTime()
{
  DateTime now = rtc.now(); // Take reading from RTC and update current time

  if (now.getEpoch() - unixPrev == 1) // Check for second transition
  {
    tm = Decode(now.getEpoch()); // Compute and decode current time
  }
  unixPrev = now.getEpoch(); // Set lagging value of unix time
}


//
// Time::ChangeTime() -- Change time based on button presses
//
void Time::ChangeTime()
{
  pressMillis = 0;

  while (digitalRead(BUTTONPIN_1) == false)
  {
    if (!pressMillis)
      pressMillis = millis();

    if (millis() - pressMillis <= 1000)
    {
      // Do things here
      break;
    }
  }
}

