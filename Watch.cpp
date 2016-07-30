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
static const byte startingPos[] = { 0, 6, 14 };
static const byte setLookup[][2] = { { 1, 2 }, { 0 ,2 }, { 0, 1 } };
static const byte bitLength[] = { 6, 6, 4 };     // Bit length of time measurements
static const byte timeOverflowAmount[] = { 60, 60, 24 }; // Amount before each time measurement overflows
static const int FLASH_DELAY = 750;
static const byte BUTTONPIN_1 = 14; // Button 1 input pin
static const byte BUTTONPIN_2 = 15; // Button 2 input pin

byte tempTime[3]; // Temporary time values for set mode
byte row = 0; // Current selected for set mode

unsigned long pressMill1;
unsigned long pressMill2;
bool isSet = false; // Boolean to indicate set mode
bool isSetPrev; // Lagging variable for isSet
bool isFirstSet = true; // First run through check for set mode
bool isFirstB1 = true; // First run through check for button one
bool isFirstB2 = true; // First run through check for button two
bool isFirstFlash = true; // First run through check for flash function
long mill = 0;

unsigned long unixPrev;


RTC_DS1307 rtc; // Create new RTC object
Chaplex myCharlie(ctrlpins, PINS); // Create new chaplex object


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
  if (isFirstFlash) // Check if first run through
  {
    mill = millis(); // Record current millisecond value
    isFirstFlash = false; // Set first run through flag to false
  }
  
  if (millis() - mill <= FLASH_DELAY) // Until 1 second has passed keep LEDs on
  {
    bitTime(t, tLength, startingPosition); // Set state of LEDs to on
  } else {
    if (millis() >= mill + FLASH_DELAY * 2) // Until two seconds total have passed keep first run through flag to false
      isFirstFlash = true;

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
void Display::Init() {}


//
// Display::DisplayTime() -- Update LED Display with current time
//
void Display::DisplayTime()
{
  
  if (!isSet)
  {
    (tm.h == 0 || tm.h == 12) ? bitTime(12, bitLength[2], startingPos[2]) : bitTime(tm.h % 12, bitLength[2], startingPos[2]);
    bitTime(tm.m, bitLength[1], startingPos[1]);
    bitTime(tm.s, bitLength[0], startingPos[0]);
  } else {
    if (row == 2 && tempTime[2] == 0) // Check if hour is 12
      flash(12, bitLength[2], startingPos[2]); // Account for 12th hour case

    bitTime(tempTime[setLookup[row][1]], bitLength[setLookup[row][1]], startingPos[setLookup[row][1]]);
    bitTime(tempTime[setLookup[row][0]], bitLength[setLookup[row][0]], startingPos[setLookup[row][0]]);
    flash(tempTime[row], bitLength[row], startingPos[row]);
  }
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
    {
      tm = Decode(now.unixtime()); // Compute and decode current time
      Serial.println(tempTime[2]);
      Serial.println(tm.h);
    }

    unixPrev = now.unixtime(); // Set lagging value of unix time
  }
}


//
// Time::ChangeTime() -- Change time based on button presses
//
void Time::ChangeTime()
{
  if (isSet) // Check if watch is in set mode
  {
    if (isFirstSet) // Check for first run through
    {
      isFirstSet = false;
      row = 0;
      
      tempTime[2] = tm.h % 12; // Set temporary time values
      tempTime[1] = tm.m;
      tempTime[0] = tm.s;
    }
  } else {
    if (!isSet && isSetPrev) // Check for set mode transition
    {
      isFirstSet = true;
      tm = Decode(Encode(tm.y, tm.mon, tm.d, tempTime[2], tempTime[1], tempTime[0]));
      
      rtc.adjust(DateTime(tm.y, tm.mon, tm.d, tm.h, tm.m, tm.s)); // Adjust RTC time to time set by user
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
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
}


//
// Button::TakeInput() -- Take readings from buttons
//
void Button::TakeInput()
{
  //
  // Check for button presses.
  //
  if (digitalRead(A0) && !button1Prev && isFirstB1)
  {
    if (millis() - pressMill1 >= 1000)
    {
      isSet = !isSet;
      Serial.println("Hold 1");
    } else {
      if (isSet)
      {
        row = (row + 1) % 3;
      }
      Serial.println("Press 1");
    }
    isFirstB1 = false;
  } else {
    isFirstB1 = true;
  }

  if (digitalRead(A1) && !button2Prev && isFirstB2)
  {
    if (millis() - pressMill2 >= 1000)
    {
      Serial.println("Hold 2");
    } else {
      if (isSet)
      {
        tempTime[row] = (tempTime[row] + 1) % timeOverflowAmount[row];
      }
      Serial.println("Press 2");
    }
    isFirstB2 = false;
  } else {
    isFirstB2 = true;
  }

  pressTime(A0, &pressMill1);
  pressTime(A1, &pressMill2);

  button1Prev = digitalRead(A0); // Update lagging values
  button2Prev = digitalRead(A1);
}
