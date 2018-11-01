#include "Button.h"

//
// Class to deal with an individual button
//

//
// Button::Button() -- Class initializer
//
void Button::Button(int pin, void (*press)(), void (*hold)())
{
  this.pin = pin;
  this.press = press;
  this.hold = hold;
}


//
// Button::Init() -- Initialize button pin
//
void Button::Init()
{
  pinMode(this.pin, INPUT);
}


//
// Button:Poll() -- Poll button for presses
//
void Button::Poll()
{
  this.value = digitalRead(this.pin);

  // If the button has either just been pressed or released
  if (this.value && !this.previousValue)
  {
    // The button was released
    if (this.isButtonBeingPressed)
    {
      if (millis() - this.pressStartTime >= HOLD_TIME)
      {
        this.hold();
      } else {
        this.press();
      }

      this.isButtonBeingPressed = false;
    } else {
      // The button has been pressed
      this.isButtonBeingPressed = true;
      this.pressStartTime = millis();
    }
  }

  this.previousValue = digitalRead(this.pin);
}