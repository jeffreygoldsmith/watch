#ifndef RTC_H
#define RTC_H

#include "Arduino.h"
#include "RTClib.h"
#include "Time.h"
#include <Vector.h>
#include "Row.h"

class RTC
{
  public:
    RTC();
    void Init(Vector<Row*>);
    void Read();
    void SetTime(tm);

    Vector<Row*> rows;

	protected:
		RTC_DS1307 rtc;
};

#endif // RTC_H