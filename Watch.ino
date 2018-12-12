#include "RTC.h"
#include "Input.h"
#include "Display.h"
#include "TimeRows.h"
#include <Vector.h>

using namespace std;

/*
 *  Name   : Jeffrey Goldsmith
 *  Date   : 29 March 2016
 *
 * Description: Binary wrist watch...
 */

#define BUTTON_PIN_1              A0
#define BUTTON_PIN_2              A1

#define HOUR_BIT_LENGTH           4
#define MINUTE_BIT_LENGTH         6
#define SECOND_BIT_LENGTH         6

#define HOUR_OVERFLOW             24
#define MINUTE_OVERFLOW           60
#define SECOND_OVERFLOW           60

#define HOUR_LED_ARRAY_INDEX      14
#define MINUTE_LED_ARRAY_INDEX    6
#define SECOND_LED_ARRAY_INDEX    0


Row hourRow = Row{0, HOUR_OVERFLOW, HOUR_BIT_LENGTH, HOUR_LED_ARRAY_INDEX, false};
Row minuteRow = Row{0, MINUTE_OVERFLOW, MINUTE_BIT_LENGTH, MINUTE_LED_ARRAY_INDEX, false};
Row secondRow = Row{0, SECOND_OVERFLOW, SECOND_BIT_LENGTH, SECOND_LED_ARRAY_INDEX, false};
Row rowArray[] = { hourRow, minuteRow, secondRow };
Vector<Row> rowVector(rowArray);
TimeRows rows(rowVector);

RTC rtc;
Input input;
Display display;

void setup()
{
  Serial.begin(9600);
  rtc.Init(&rows);
  input.Init(&rows, &rtc, BUTTON_PIN_1, BUTTON_PIN_2);
  display.Init(&rows);
}

void loop()
{
  input.TakeInput();
  rtc.Read();
  display.UpdateDisplay();
}
