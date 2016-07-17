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


class Display
{
  public:
    Display();
    void Init();
    void DisplayTime();
};


class Time
{
  public:
    Time();
    void Sync();
    void UpdateTime();
    void ChangeTime();
};


class Button
{
  public:
    Button();
    void Init();
    void TakeInput();
    void ChangeTime();
    unsigned long counter;
    bool button1Prev;
    bool button2Prev;

  protected:
    byte BUTTONPIN_1;
    byte BUTTONPIN_2;
};


#endif // WATCH_H
