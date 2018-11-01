#ifndef RTC_H
#define RTC_H

#include "Arduino.h"
#include "Time.h"

class RTC
{
  public:
    RTC();
    void Init(Row);
    void Sync();
    void UpdateTime();
    void ChangeTime();

    Row row;

	protected:
		rtc RTC_DS1307
};

#endif // RTC_H