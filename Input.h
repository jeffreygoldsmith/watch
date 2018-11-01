#ifndef INPUT_H
#define INPUT_H

#include "Arduino.h"
#include "Button.h"
#include "RTC.h"
#include "Row.h"

class Input
{
  public:
    Input(Row[], RTC, int, int);
    void Init();
    void toggleSet();
    void changeRow();
    void increaseTimeValue();
    void TakeInput();

    Row rows[];
    Button buttonOne;
    Button buttonTwo;
};

#endif // INPUT_H