// #include "Button.h"

// //
// // Class to deal with an individual button
// //

// //
// // Button::Button() -- Class initializer
// //
// Button::Button(int pin, void (*press)(), void (*hold)())
// {
//   this->pin = pin;
//   this->press = press;
//   this->hold = hold;
// }


// //
// // Button::Init() -- Initialize button pin
// //
// void Button::Init()
// {
//   pinMode(pin, INPUT);
// }


// //
// // Button:Poll() -- Poll button for presses
// //
// void Button::Poll()
// {
//   value = digitalRead(pin);

//   // If the button has either just been pressed or released
//   if (value && !previousValue)
//   {
//     // The button was released
//     if (isButtonBeingPressed)
//     {
//       if (millis() - pressStartTime >= HOLD_TIME)
//       {
//         hold();
//       } else {
//         press();
//       }

//       isButtonBeingPressed = false;
//     } else {
//       // The button has been pressed
//       isButtonBeingPressed = true;
//       pressStartTime = millis();
//     }
//   }

//   previousValue = digitalRead(pin);
// }