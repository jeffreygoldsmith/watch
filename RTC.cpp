#include <RTClib.h>


//
// Class to control time and RTC.
//


//
// RTC::RTC() -- Class constructor
//
RTC::RTC() {}

bool isEditingModeEnabled(Row rows[])
{
  bool isEditingModeEnabled = false;

  for (int i = 0; i < sizeof(rows); i++)
  {
    if (rows[i].selected)
      return true;
  }

  return false;
}


//
// Function to initialize RTC.
//
void RTC::Sync()
{
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Flash current time onto RTC
}


//
// Time::Read() -- Update time based on readings from RTC
//
void RTC::Read()
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
// RTC::ChangeTime() -- Change time based on button presses
//
void RTC::ChangeTime()
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