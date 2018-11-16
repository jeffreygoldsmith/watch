#ifndef INPUT_H
#define INPUT_H

#include "Arduino.h"
#include "Button.h"
#include "RTC.h"
#include "Vector.h"
#include "Row.h"

class Input
{
  public:
    Input();
    void Init(Vector<Row>, RTC&, int, int);
    void toggleSet();
    void changeRow();
    void increaseTimeValue();
    void TakeInput();

    Vector<Row> rows;
    RTC rtc;
    Button buttonOne;
    Button buttonTwo;
};

#endif // INPUT_H