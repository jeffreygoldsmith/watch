#include "Arduino.h"
#include "RTClib.h"
#include "Time.h"
#include "Row.h"
#include <ArduinoSTL.h>

#ifndef RTC_H
#define RTC_H


using namespace std;

class RTC
{
  public:
    RTC(vector<Row>);
    void Init();
    void Read();
    void SetTime(tm);

    vector<Row> rows;

	protected:
		RTC_DS1307 rtc;
};

#endif // RTC_H