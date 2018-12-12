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
void Button::Init(Input *input, int pin, void (*press)(Input *input), void (*hold)(Input *input))
{
  this->input = input;
  this->pin = pin;
  this->press = press;
  this->hold = hold;
  pinMode(pin, INPUT);
}


//
// Button:Poll() -- Poll button for presses
//
void Button::Poll()
{
  value = digitalRead(pin);

  // If the button has either just been pressed or released
  if (value && !previousValue)
  {
    // The button was released
    if (isButtonBeingPressed)
    {
      if (millis() - pressStartTime >= HOLD_TIME)
      {
        hold(this->input);
      } else {
        press(this->input);
      }

      isButtonBeingPressed = false;
    } else {
      // The button has been pressed
      isButtonBeingPressed = true;
      pressStartTime = millis();
    }
  }

  previousValue = digitalRead(pin);
}