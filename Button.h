#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"
#include "Input.h"

#define HOLD_TIME 1000

class Input;

class Button
{
  public:
    Button();

    void Init(Input *input, int pin, void (*press)(Input *input), void (*hold)(Input *input));
    void Poll();

  protected:
    Input *input;
    void (*press)(Input *input);
    void (*hold)(Input *input);
    int pin;
    int value;
    int previousValue;
    bool isButtonBeingPressed;
    unsigned long pressStartTime;
};

#endif // BUTTON_H