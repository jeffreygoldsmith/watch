#ifndef RTC_H
#define RTC_H

#include "Arduino.h"
#include "Time.h"

class RTC
{
  public:
    RTC(Row);
    void Init();
    void UpdateTime();
    void SetTime(tm);

    Row rows;

	protected:
    bool isEditingModeEnabled();

		rtc RTC_DS1307
};

#endif // RTC_H