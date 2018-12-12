#include "RTC.h"


//
// Class to control time and RTC.
//


unsigned long unixTimePrev;


//
// RTC::RTC() -- Class constructor
//
RTC::RTC() {}


//
// Function to initialize RTC.
//
void RTC::Init(TimeRows *timeRows)
{
  this->timeRows = timeRows;
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Flash current time onto RTC
}


//
// Time::Read() -- Update time based on readings from RTC
//
void RTC::Read()
{
  DateTime now = rtc.now(); // Take reading from RTC and update current time

  if (!timeRows->isEditingModeEnabled())
  {
    if (now.unixtime() - unixTimePrev == 1) // Check for second transition
    {
      tm decodedTime = Decode(now.unixtime()); // Compute and decode current time

      timeRows->rows[0].timeValue = decodedTime.h; // Set row time values
      timeRows->rows[1].timeValue = decodedTime.m;
      timeRows->rows[2].timeValue = decodedTime.s;
    }

    unixTimePrev = now.unixtime(); // Set lagging value of unix time
  }
}


//
// RTC::SetTime() -- Update RTC time from a tm struct
//
void RTC::SetTime(tm tm)
{
  DateTime now = rtc.now();
  
  rtc.adjust(DateTime(now.year(), now.month(), now.day(), tm.h, tm.m, tm.s)); // Adjust RTC time to time set by user
}