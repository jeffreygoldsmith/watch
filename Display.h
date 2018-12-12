#ifndef DISPLAY_H
#define DISPLAY_H

#include "Arduino.h"
#include <Vector.h>
#include "Row.h"

using namespace std;

class Display
{
  public:
    Display();
    void Init(Vector<Row*>);
    void UpdateDisplay();
    void DisplayRow(Row);

    Vector<Row*> rows;

  protected:
    void solid(Row);
    void flash(Row);
    bool isFirstFlash;
    unsigned long mill;
    bool isWatchInSetMode;
};

#endif // DISPLAY_H