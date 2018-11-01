//
// Include Libraries.
//
#include "Watch.h"
#include <RTClib.h>
#include <assert.h>


#define PINS 5


struct tm tm;




byte ctrlpins[] = { 13, 12, 11, 10, 9 }; //Arduino pins controlling charlieplexed leds
static const byte startingPos[] = { 0, 6, 14 };
static const byte setLookup[][2] = { { 1, 2 }, { 0 ,2 }, { 0, 1 } };
static const byte bitLength[] = { 6, 6, 4 };     // Bit length of time measurements
static const byte timeOverflowAmount[] = { 60, 60, 24 }; // Amount before each time measurement overflows
static const int FLASH_DELAY = 750;
static const byte BUTTONPIN_1 = 14; // Button 1 input pin
static const byte BUTTONPIN_2 = 15; // Button 2 input pin

byte tempTime[3]; // Temporary time values for set mode
byte row = 0; // Current selected for set mode

unsigned long pressMill1;
unsigned long pressMill2;
bool isSet = false; // Boolean to indicate set mode
bool isSetPrev; // Lagging variable for isSet
bool isFirstSet = true; // First run through check for set mode
bool isFirstB1 = true; // First run through check for button one
bool isFirstB2 = true; // First run through check for button two
bool isFirstFlash = true; // First run through check for flash function
long mill = 0;

unsigned long unixPrev;

Chaplex myCharlie(ctrlpins, PINS); // Create new chaplex object


