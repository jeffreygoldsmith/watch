#include <RTClib.h>


//
// Class to control time and RTC.
//


unsigned long unixTimePrev;


//
// RTC::RTC() -- Class constructor
//
RTC::RTC() 
{
  this.rows = rows;
}


//
// Function to initialize RTC.
//
void RTC::Init(Row rows)
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

  if (!this.isEditingModeEnabled())
  {
    if (now.unixtime() - unixTimePrev == 1) // Check for second transition
    {
      tm = Decode(now.unixtime()); // Compute and decode current time

      this.rows[0].timeValue = tm.h; // Set row time values
      this.rows[1].timeValue = tm.m;
      this.rows[2].timeValue = tm.s;
    }

    unixTimePrev = now.unixtime(); // Set lagging value of unix time
  }
}


//
// RTC::SetTime() -- Update RTC time from a tm struct
//
void RTC::SetTIme(tm tm)
{
  rtc.adjust(DateTime(tm.y, tm.mon, tm.d, tm.h, tm.m, tm.s)); // Adjust RTC time to time set by user
}


//
// Function to check to see
//
bool RTC::isEditingModeEnabled()
{
  bool isEditingModeEnabled = false;

  for (int i = 0; i < sizeof(this.rows); i++)
  {
    if (this.rows[i].selected)
      return true;
  }

  return false;
}