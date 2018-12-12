#ifndef DISPLAY_H
#define DISPLAY_H

#include "Arduino.h"
#include <Vector.h>
#include "TimeRows.h"

using namespace std;

class Display
{
  public:
    Display();
    void Init(TimeRows *timeRows);
    void UpdateDisplay();
    void DisplayRow(Row);

    TimeRows *timeRows;

  protected:
    void solid(Row);
    void flash(Row);
    bool isFirstFlash;
    unsigned long mill;
    bool isWatchInSetMode;
};

#endif // DISPLAY_H