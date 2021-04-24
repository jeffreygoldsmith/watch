#include "Button.h"

//
// Class to deal with an individual button
//

//
// Button::Button() -- Class initializer
//
Button::Button() {}


//
// Button::Init() -- Class initializer
//
void Button::Init(int pin, void (*press)(), void (*hold)())
{
  this->pin = pin;
  this->press = press;
  this->hold = hold;
  pinMode(pin, INPUT);
}


//
// Button:Poll() -- Poll button for presses
//
// Assumes that the button is configured in such a fashion that
// the button is HIGH when open and LOW when closed.
//
void Button::Poll()
{
  bool currentButtonValue = digitalRead(pin);

  // Because the button is in the HIGH state by default,
  // a press is seen by a current value of LOW and a previous value of HIGH.
  // Similarly, a release is seen by the opposite.
  if (!currentButtonValue && previousButtonValue) // Button press down
  {
    pressStartTime = millis();
  } else if (currentButtonValue && !previousButtonValue) { // Button release
    if (millis() - pressStartTime >= HOLD_TIME)
    {
      hold();
    } else {
      press();
    }
  }

  previousButtonValue = currentButtonValue;
}