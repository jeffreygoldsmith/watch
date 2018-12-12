#include "Input.h"


//
// Class to control buttons
//


//
// Input::Input() -- Class constructor
//
Input::Input() {}


//
// Input::Init() -- Class initializer
//
void Input::Init(TimeRows *timeRows, RTC *rtc, int buttonOnePin, int buttonTwoPin)
{
  this->timeRows = timeRows;
  this->rtc = rtc;
  buttonOne->Init(this, buttonOnePin, changeRow, toggleSet);
  buttonTwo->Init(this, buttonTwoPin, increaseTimeValue, changeRow);
}


//
// Input::TakeInput() -- Take readings from input sources
//
void Input::TakeInput()
{
  buttonOne->Poll();
  buttonTwo->Poll();
}


//
// Input::toggleSet() -- Toggle set mode of watch
//
void Input::toggleSet(Input* input)
{
  TimeRows *timeRows = input->timeRows;
  if (timeRows->isEditingModeEnabled())
  {
    timeRows->deselectRows();
    tm tm;
    tm.h = timeRows->rows[0].timeValue;
    tm.m = timeRows->rows[1].timeValue;
    tm.s = timeRows->rows[2].timeValue;
    input->rtc->SetTime(tm);
  } else {
    timeRows->rows[0].isSelectedForEditing = true;
  }
}


//
// Input::changeRow() -- Change current row selected
//
void Input::changeRow(Input* input)
{
  TimeRows *timeRows = input->timeRows;

  if (!timeRows->isEditingModeEnabled())
    return;

  int selectedRowIndex = input->timeRows->indexOfSelectedRow();
  timeRows->rows[selectedRowIndex].isSelectedForEditing = false;
  timeRows->rows[selectedRowIndex + 1 % timeRows->rows.size()].isSelectedForEditing = true;
}


//
// Input::increaseTimeValue() -- Increase time value of currently selected row
//
void Input::increaseTimeValue(Input* input)
{
  TimeRows *timeRows = input->timeRows;

  if (!timeRows->isEditingModeEnabled())
    return;

  int selectedRowIndex = timeRows->indexOfSelectedRow();
  timeRows->rows[selectedRowIndex].timeValue = timeRows->rows[selectedRowIndex].timeValue + 1 % timeRows->rows[selectedRowIndex].maxTimeValue;
}