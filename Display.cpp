#include "Display.h"
#include <Chaplex.h>

#define CONTROL_PIN_LENGTH 5
#define FLASH_DELAY 750

byte controlPins[] = { 2, 3, 4, 5, 6 };
Chaplex ledDisplay(controlPins, CONTROL_PIN_LENGTH);
charlieLed leds[]  =  { { 1, 0 }, { 2, 0 }, { 3, 0 }, { 4, 0 },
                          { 0, 1 }, { 2, 1 }, { 3, 1 }, { 4, 1 },
                          { 0, 2 }, { 1, 2 }, { 3, 2 }, { 4, 2 },
                          { 0, 3 }, { 1, 3 }, { 2, 3 }, { 4, 3 },
                          { 0, 4 }, { 1, 4 }, { 2, 4 } };

//
// Class to control information displayed on LED Display.
//


//
// Display::Display() -- Class constructor
//
Display::Display() {}


//
// Display::Init() -- Class initializer
//
void Display::Init(TimeRows *timeRows)
{
  this->timeRows = timeRows;
}


//
// Display::UpdateDisplay() -- Update LED Display with current time
//
void Display::UpdateDisplay()
{
  for (int i = 0; i < timeRows->rows.size(); i++)
    DisplayRow(timeRows->rows[i]);
}


//
// Display::DisplayRow() -- Update single LED row time value
//
void Display::DisplayRow(Row row)
{
  if (row.isSelectedForEditing)
    flash(row);
  else
    solid(row);
}


//
// Function to take time and display value on Display.
//
void Display::solid(Row row)
{
  for (int i = 0; i < row.bitLength; i++)
  {
    bool bitBool = bitRead(row.timeValue, i); // Check each bit in t to be high or low
    ledDisplay.ledWrite(leds[i + row.ledArrayIndex], bitBool); // If bit is high set LED to be high, else set low
  }
  ledDisplay.outRow(); // Update Display
}


//
// Function to display flashing time value on Display.
//
void Display::flash(Row row)
{
  if (isFirstFlash) // Check if first run through
  {
    mill = millis(); // Record current millisecond value
    isFirstFlash = false; // Set first run through flag to false
  }

  if (millis() - mill <= FLASH_DELAY) // Until 1 second has passed keep LEDs on
  {
    solid(row); // Set state of LEDs to on
  } else {
    if (millis() >= mill + FLASH_DELAY * 2) // Until two seconds total have passed keep first run through flag to false
      isFirstFlash = true;

    solid(row); // Set state of LEDs to off
  }
}