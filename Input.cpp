#include "Input.h"

//
// Class to control buttons
//


//
// Input::Input() -- Class constructor
//
Input::Input(Row rows, RTC rtc, int buttonOnePin, int buttonTwoPin)
{
  this.rows = rows;
  this.rtc = rtc;
  this.buttonOne = Button(buttonOnePin, this.changeRow, this.toggleSet);
  this.buttonTwo = Button(buttonTwoPin, this.increaseTimeValue, void empty() {});
}


//
// Input::Init() -- Initialization method
//
void Input::Init()
{
  this.buttonOne.Init();
  this.buttonTwo.Init();
}


//
// Input::toggleSet() -- Toggle set mode of watch
//
void Input::toggleSet()
{
  if (isRowSelected(this.rows))
  {
    deselectRows(this.rows);
    tm tm;
    tm.h = this.rows[0].timeValue;
    tm.m = this.rows[1].timeValue;
    tm.s = this.rows[2].timeValue;
    this.rtc.SetTime(tm);
  } else {
    this.rows[0].isSelectedForEditing = true;
  }
}


//
// Input::changeRow() -- Change current row selected
//
void Input::changeRow()
{
  if (!isRowSelected(this.rows))
    return;

  int selectedRowIndex = indexOfSelectedRow(this.rows);
  this.rows[selectedRowIndex].isSelectedForEditing = false;
  this.rows[selectedRowIndex + 1 % sizeof(this.rows)].isSelectedForEditing = true;
}


//
// Input::increaseTimeValue() -- Increase time value of currently selected row
//
void Input::increaseTimeValue()
{
  if (!isRowSelected(this.rows))
    return;

  int selectedRowIndex = indexOfSelectedRow(this.rows);
  this.rows[selectedRowIndex].timeValue = this.rows[selectedRowIndex].timeValue + 1 % this.rows[selectedRowIndex].maxTimeValue;
}


//
// Input::TakeInput() -- Take readings from input sources
//
void Input::TakeInput()
{
  this.buttonOne.Poll();
  this.buttonTwo.Poll();
}