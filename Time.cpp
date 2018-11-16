#include "Time.h"

//
// Function to convert timestamp to seconds since 1 January 1970.
//
unsigned long Encode(int y, int mon, int d, int h, int m, int s)
{
  unsigned long dy;

  assert(y >= 1970 && y <= 2038);
  assert(mon >= 1 && mon <= 12);
  assert(d >= 1 && d <= 31);
  assert(h >= 0 && h < 60);
  assert(m >= 0 && m < 60);
  assert(s >= 0 && s < 60);

  static int days[] = // Days prior to start of month (origin-1)
  {
    0, -1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333
  };

  dy = y - 1968; // Normalize year to leap year preceding epoch start
  dy = (dy - 2) * 365 + (dy >> 2); // Multiply by 365.25, giving days in years
  d -= !(y & 0x03) && mon <= 2; // Reduce day if prior to leap point in leap year
  dy = s + 60 * (m + 60 * (h + 24 * (dy + days[mon] + d)));

  return dy;
}


//
// Function to take time_t variable and decode into individual measurements of time.
//
struct tm Decode(unsigned long ts)
{
  int d, m, y;
  struct tm tm;

  tm.s = ts % 60; // Seconds
  ts /= 60;

  tm.m = ts % 60; // Minutes
  ts /= 60;

  tm.h = ts % 24; // Hours
  ts /= 24;

  ts = ts * 100 + 36525 * 2 - 5950; // Move start of epoch to 1 March 1968 and scale
  y = (ts + 75) / 36525; // Years since start of epoch
  d = (175 + ts - y * 36525) / 100 * 10; // Days since start of year (times 10)
  m = (d - 6) / 306; // Months since start of year

  tm.y = 1968 + y + (m >= 10); // Year
  tm.mon = 1 + (m + 2) % 12; // Month
  tm.d = (4 + d - m * 306) / 10; // Day
  tm.wd = 1 + (5 + ts / 100) % 7; // Day of week (Sunday = 1)

  return tm;
}