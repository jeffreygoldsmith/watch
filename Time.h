#ifndef TIME_H
#define TIME_H

#include <assert.h>


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

unsigned long Encode(int y, int mon, int d, int h, int m, int s);
struct tm Decode(unsigned long ts);

#endif // TIME_H