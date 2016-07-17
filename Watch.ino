#include "Watch.h"

/*
   Name   : Jeffrey Goldsmith
   Teacher: Mr. D'Arcy
   Date   : 29 March 2016
   Course : TEI3M

   Description: Binary wrist watch...
*/

Time RTC;
Display LEDS;
Button PushButtons;

void setup()
{
  Serial.begin(9600);
  Serial.println("test");
  LEDS.Init();
  RTC.Sync();
  //PushButtons.Init();
}

void loop()
{
  //PushButtons.TakeInput();
  RTC.UpdateTime();
  LEDS.DisplayTime();
  //Clock.ChangeTime();
}
