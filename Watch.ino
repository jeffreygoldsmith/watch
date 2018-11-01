#include "Watch.h"
#include "Rows.h"

/*
   Name   : Jeffrey Goldsmith
   Date   : 29 March 2016

   Description: Binary wrist watch...
*/

#define HOUR_BIT_LENGTH 4
#define MINUTE_BIT_LENGTH 6
#define SECOND_BIT_LENGTH 6

#define HOUR_LED_ARRAY_INDEX 14
#define MINUTE_LED_ARRAY_INDEX 6
#define SECOND_LED_ARRAY_INDEX 0

Time Clock;
Display LEDS;
Button PushButtons;

Row hourRow = Row{0, HOUR_BIT_LENGTH, HOUR_LED_ARRAY_INDEX, false};
Row minuteRow = Row{0, MINUTE_BIT_LENGTH, MINUTE_LED_ARRAY_INDEX, false};
Row secondRow = Row{0, SECOND_BIT_LENGTH, SECOND_LED_ARRAY_INDEX, false};
Row rows[] = { hourRow, minuteRow, secondRow };

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
