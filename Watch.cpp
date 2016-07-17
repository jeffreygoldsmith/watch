//
// Include Libraries.
//
#include "Watch.h"
#include <RTClib.h>
#include <assert.h>
#include <Chaplex.h>

#define PINS 5


struct tm tm;


byte ctrlpins[] = { 13, 12, 11, 10, 9 }; //Arduino pins controlling charlieplexed leds
static const byte startingPos[] = { 0, 4, 10 };
static const byte bitLength[] = { 6, 6, 4 };     // Bit length of time measurements
static const byte BUTTONPIN_1 = 14; // Button 1 input pin
static const byte BUTTONPIN_2 = 15; // Button 2 input pin

unsigned long pressMill1;
unsigned long pressMill2;
bool isSet = false;
bool isSetPrev;
bool isFirstSet = true;
bool isFirstSleep = true;
bool isFirstRT = true;
long mill = 0;

unsigned long unixPrev;


RTC_DS1307 rtc; // Create new RTC object
Chaplex myCharlie(ctrlpins, PINS); // Create new chaplex object


DateTime now = rtc.now(); // Take reading from RTC and update current time
charlieLed myLeds[]  =  { { 1, 0 }, { 2, 0 }, { 3, 0 }, { 4, 0 },
                          { 0, 1 }, { 2, 1 }, { 3, 1 }, { 4, 1 },
                          { 0, 2 }, { 1, 2 }, { 3, 2 }, { 4, 2 },
                          { 0, 3 }, { 1, 3 }, { 2, 3 }, { 4, 3 },
                          { 0, 4 }, { 1, 4 }, { 2, 4 } };


//
// Function to take time and display value on Display.
//
void bitTime(int t, byte tLength, byte startingPosition)
{
  for (int i = 0; i < tLength; i++)
  {
    bool bitBool = bitRead(t, i); // Check each bit in t to be high or low
    myCharlie.ledWrite(myLeds[i + startingPosition], bitBool); // If bit is high set LED to be high, else set low
  }
  myCharlie.outRow(); // Update Display
}


//
// Function to display flashing time value on Display.
//
void flash(int t, byte tLength, byte startingPosition)
{
  if (isFirstRT) // Check if first run through
  {
    mill = millis(); // Record current millisecond value
    isFirstRT = false; // Set first run through flag to false
  }

  if (millis() - mill <= 1000) // Until 1 second has passed keep LEDs on
  {
    bitTime(t, tLength, startingPosition); // Set state of LEDs to on
  } else {
    if (millis() >= mill + 2000) // Until two seconds total have passed keep first run through flag to false
      isFirstRT = true;

    bitTime(0, tLength, startingPosition); // Set state of LEDs to off
  }
}


//
// Function to detect amount of time a button has been pressed.
//
void pressTime(byte pin, unsigned long *pressTime)
{
  if (!digitalRead(pin))
  {
    if (!*pressTime)
      *pressTime = millis();
  } else {
    *pressTime = 0;
  }
}


//
// Function to convert timestamp to seconds since 1 January 1970.
//
unsigned long Encode(int y, int mon, int d, int h, int m, int s)
{
  unsigned long dy;

  assert(y >= 1970 && y <= 2038);
  assert(mon >= 1 && mon <= 12);
  assert(d >= 1 && d <= 31);
  assert(h >= 0 && h < 60);
  assert(m >= 0 && m < 60);
  assert(s >= 0 && s < 60);

  static int days[] = // Days prior to start of month (origin-1)
  {
    0, -1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333
  };

  dy = y - 1968; // Normalize year to leap year preceding epoch start
  dy = (dy - 2) * 365 + (dy >> 2); // Multiply by 365.25, giving days in years
  d -= !(y & 0x03) && mon <= 2; // Reduce day if prior to leap point in leap year
  dy = s + 60 * (m + 60 * (h + 24 * (dy + days[mon] + d)));

  return dy;
}


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
// Class to control information displayed on LED Display.
//

//
// Display::Display() -- Class constructor
//
Display::Display() {}


//
// Display::Init() -- Initialization method
//
void Display::Init() 
{ 
  Chaplex myCharlie(ctrlpins, PINS); // Create new chaplex object
  Serial.println("test");
}


//
// Display::DisplayTime() -- Update LED Display with current time
//
void Display::DisplayTime()
{
  //(tm.h == 0 || tm.h == 12) ? bitTime(12, bitLength[2], startingPosition[2]) : bitTime(tm.h % 12, bitLength[2], startingPosition[2]);
  //bitTime(tm.m, bitLength[1], startingPosition[1]);
  flash(tm.s, bitLength[0], startingPos[0]);
}


//
// Class to control time and RTC.
//

//
// Time::Time() -- Class constructor
//
Time::Time() {}


//
// Function to initialize RTC.
//
void Time::Sync()
{
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Flash current time onto RTC
}


//
// Time::UpdateTime() -- Update time based on readings from RTC
//
void Time::UpdateTime()
{
  DateTime now = rtc.now(); // Take reading from RTC and update current time

  if (!isSet)
  {
    if (now.unixtime() - unixPrev == 1) // Check for second transition
      tm = Decode(now.unixtime()); // Compute and decode current time

    unixPrev = now.unixtime(); // Set lagging value of unix time
  }
  Serial.println(now.unixtime());
}


//
// Time::ChangeTime() -- Change time based on button presses
//
void Time::ChangeTime()
{
  Serial.begin(9600);
  if (isSet) // Check if watch is in set mode
  {
    Serial.println("Set mode active.");
  } else {
    if (!isSet && isSetPrev) // Check for set mode transition
    {
      DateTime newDate = (tm.y, tm.mon, tm.d, tm.wd, tm.h, tm.m, tm.s); // Adjust RTC time to time set by user
      rtc.adjust(newDate);
    }
  }
  isSetPrev = isSet; // Set lagging variable for set
}


//
// Class to control buttons
//

//
// Button::Button() -- Class constructor
//
Button::Button() {}


//
// Button::Init() -- Initialization method
//
void Button::Init()
{
  Serial.begin(9600);
  pinMode(BUTTONPIN_1, INPUT);
  pinMode(BUTTONPIN_2, INPUT);
}


//
// Button::TakeInput() -- Take readings from buttons
//
void Button::TakeInput()
{
  Serial.begin(9600);
  //
  // Check for button presses.
  //
  if (digitalRead(A0) && !button1Prev && isFirstSet)
  {
    if (millis() - pressMill1 >= 1000)
    {
      isSet != isSet;
      Serial.println("Hold 1");
    } else {
      Serial.println("Press 1");
    }
    isFirstSet = false;
  } else {
    isFirstSet = true;
  }

  if (digitalRead(A1) && !button2Prev && isFirstSleep)
  {
    if (millis() - pressMill2 >= 1000)
    {
      Serial.println("Hold 2");
    } else {
      Serial.println("Press 2");
    }
    isFirstSleep = false;
  } else {
    isFirstSleep = true;
  }

  pressTime(A0, &pressMill1);
  pressTime(A1, &pressMill2);

  button1Prev = digitalRead(A0); // Update lagging values
  button2Prev = digitalRead(A1);
}
