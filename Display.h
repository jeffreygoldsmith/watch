#ifndef DISPLAY_H
#define DISPLAY_H

#include "Arduino.h"
#include "Row.h"

class Display
{
  public:
    Display();
    void Init(Row);
    void UpdateDisplay();
    void DisplayRow(Row);

    Row row;

  protected:
    void displaySolid(Row);
    void displayFlash(Row);
    bool isWatchInSetMode;
};

#endif // DISPLAY_H