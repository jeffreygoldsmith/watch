#include "Watch.h"

/*
   Name   : Jeffrey Goldsmith
   Teacher: Mr. D'Arcy
   Date   : 29 March 2016
   Course : TEI3M

   Description: Binary wrist watch...
*/

Time Clock;
Matrix Display;
Button PushButtons;

void setup()
{
  Display.Init();
  PushButtons.Init();
  Clock.Sync();
}

void loop()
{
  PushButtons.TakeInput();
  Display.DisplayTime();
  Clock.UpdateTime();
  Clock.ChangeTime();
}

