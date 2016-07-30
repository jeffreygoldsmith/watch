#include "Watch.h"

/*
   Name   : Jeffrey Goldsmith
   Teacher: Mr. D'Arcy
   Date   : 29 March 2016
   Course : TEI3M

   Description: Binary wrist watch...
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

