#ifndef INPUT_H
#define INPUT_H

#include "Arduino.h"
#include "Row.h"

class Button
{
  public:
    Button();
    void Init(Row);
    void TakeInput();
    void ChangeTime();

    Row row;
    unsigned long counter;
    bool button1Prev;
    bool button2Prev;

  protected:
    byte BUTTONPIN_1;
    byte BUTTONPIN_2;
};

#endif // INPUT_H