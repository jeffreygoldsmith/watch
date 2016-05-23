//
// Include Libraries.
//
#include "Watch.h"
#include <RTClib.h>
#include <assert.h>
#include <SparkFun_LED_8x7.h>
#include <Chaplex.h>


struct tm tm;


static const byte row[] = { 7, 6, 5, 4, 3, 2, 1 };           // Row number of time measurements
static const byte bitLength[] = { 6, 6, 5, 3, 5, 4, 7 };     // Bit length of time measurements
static const byte BUTTONPIN_1 = 14; // Button 1 input pin
static const byte BUTTONPIN_2 = 15; // Button 2 input pin

byte MATRIX_PINS[] = {2, 3, 4, 5, 6, 7, 8, 9};  // Pins for LEDs

unsigned long pressMill1;
unsigned long pressMill2;
bool isFirstSet = true;
bool isFirstSleep = true;
bool isSet = false;

unsigned long unixPrev;


RTC_DS1307 rtc; // Create new RTC object


//
// Function to take time and display value on matrix.
//
void bitTime(int t, byte tLength, byte y)
{
  for (int i = 0; i < tLength; i++)
  {
    bool bitBool = bitRead(t, i); // Check each bit in t to be high or low
    Plex.pixel(y, i, bitBool); // If bit is high set LED to be high, else set low
  }
  Plex.display(); // Update matrix
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
// CTS::Encode() -- Convert timestamp to seconds since 1 January 1970
//
unsigned long Encode(int y, int mon, int d, int h, int m, int s)
{
      int dy;
 
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
 
      return s + 60 * (m + 60 * (h + 24 * (dy + days[mon] + d)));
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
// Class to control information displayed on LED matrix.
//

//
// Matrix::Matrix() -- Class constructor
//
Matrix::Matrix() {}


//
// Matrix::Init() -- Initialization method
//
void Matrix::Init()
{
  Plex.init(MATRIX_PINS); // Initialize charlieplexed matrix
  Plex.clear(); // Set all LEDs to off
  Plex.display(); // Update matrix
}


//
// Matrix::DisplayTime() -- Update LED matrix with current time
//
void Matrix::DisplayTime()
{
  bitTime(tm.y % 100, bitLength[6], row[6]); // Display time on matrix
  bitTime(tm.mon, bitLength[5], row[5]);
  bitTime(tm.d, bitLength[4], row[4]);
  bitTime(tm.wd, bitLength[3], row[3]);
  (tm.h == 0 || tm.h == 12) ? bitTime(12, bitLength[2], row[2]) : bitTime(tm.h % 12, bitLength[2], row[2]);
  bitTime(tm.m, bitLength[1], row[1]);
  bitTime(tm.s, bitLength[0], row[0]);
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
  DateTime now = rtc.now(); // Take reading from RTC and update current time
  unixPrev = now.unixtime(); // Set lagging value of unix time to be initial unix time
}


//
// Time::UpdateTime() -- Update time based on readings from RTC
//
void Time::UpdateTime()
{
  DateTime now = rtc.now(); // Take reading from RTC and update current time

  if (now.unixtime() - unixPrev == 1) // Check for second transition
    tm = Decode(now.unixtime()); // Compute and decode current time

  unixPrev = now.unixtime(); // Set lagging value of unix time
}


//
// Time::ChangeTime() -- Change time based on button presses
//
void Time::ChangeTime()
{
  if (isSet)
  {
    Serial.println("Set mode active.");
  }
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
  //
  // Check for button presses.
  //
  if (digitalRead(A0) && !button1Prev && isFirstSet)
  {
    if (millis() - pressMill1 >= 1000)
    {
      isSet = !isSet;
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
