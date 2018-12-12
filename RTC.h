#ifndef RTC_H
#define RTC_H

#include "Arduino.h"
#include "RTClib.h"
#include "Time.h"
#include "TimeRows.h"

class RTC
{
  public:
    RTC();
    void Init(TimeRows *timeRows);
    void Read();
    void SetTime(tm);

    TimeRows *timeRows;

	protected:
		RTC_DS1307 rtc;
};

#endif // RTC_H