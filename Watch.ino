#include "Watch.h"

/*
   Name   : Jeffrey Goldsmith
   Date   : Started - 29 March 2016

   Description: Microcontroller code for a binary wrist watch
*/

Time Clock;
Display LEDS;
Button PushButtons;

void setup()
{
  Serial.begin(9600);
  LEDS.Init();
  Clock.Sync();
  PushButtons.Init();
}

void loop()
{
  PushButtons.TakeInput();
  Clock.UpdateTime();
  LEDS.DisplayTime();
  Clock.ChangeTime();
}

