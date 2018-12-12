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
void Input::Init(Vector<Row*> rows, RTC *rtc, int buttonOnePin, int buttonTwoPin)
{
  this->rows = rows;
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
  if (isEditingModeEnabled(input->rows))
  {
    deselectRows(input->rows);
    tm tm;
    tm.h = input->rows[0].timeValue;
    tm.m = input->rows[1].timeValue;
    tm.s = input->rows[2].timeValue;
    input->rtc->SetTime(tm);
  } else {
    input->rows[0].isSelectedForEditing = true;
  }
}


//
// Input::changeRow() -- Change current row selected
//
void Input::changeRow(Input* input)
{
  if (!isEditingModeEnabled(input->rows))
    return;

  int selectedRowIndex = indexOfSelectedRow(input->rows);
  input->rows[selectedRowIndex].isSelectedForEditing = false;
  input->rows[selectedRowIndex + 1 % sizeof(input->rows)].isSelectedForEditing = true;
}


//
// Input::increaseTimeValue() -- Increase time value of currently selected row
//
void Input::increaseTimeValue(Input* input)
{
  if (!isEditingModeEnabled(input->rows))
    return;

  int selectedRowIndex = indexOfSelectedRow(input->rows);
  input->rows[selectedRowIndex]->timeValue = input->rows[selectedRowIndex]->timeValue + 1 % input->rows[selectedRowIndex]->maxTimeValue;
}