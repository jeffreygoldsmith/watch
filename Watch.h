#ifndef WATCH_H
#define WATCH_H

#include "Arduino.h"


struct tm
{
  int y;    // Year
  int mon;  // Month
  int d;    // Day
  int wd;   // Weekday
  int h;    // Hour
  int m;    // Minute
  int s;    // Second
};

class Button
{
  public:
    Button();
    void TakeInput();
    void ChangeTime();
    unsigned long counter;

  private:
    byte BUTTONPIN_1;
    byte BUTTONPIN_2;
    bool button1; 
    bool button2; 
    bool button1Prev; 
    bool button2Prev;
}

class Time
{
  public:
    Time();
    void Sync();
    void UpdateTime();
    void ChangeTime();
};

#endif // WATCH_H
