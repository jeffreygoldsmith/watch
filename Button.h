#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"

#define HOLD_TIME 1000

class Button
{
  public:
    Button();
    void Init(int, void (*)(), void (*)());
    void Poll();

  protected:
    void (*press)();              // A handler to be called if the button is pressed
    void (*hold)();               // A handler to be called if the button is held down for one second
    int pin;                      // The physical pin corresponding to the button
    bool previousButtonState;     // A lagging value of the button readout
    unsigned long pressStartTime; // A count of how long the button has been held down for in milliseconds
};

#endif // BUTTON_H