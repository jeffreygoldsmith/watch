// #include "Input.h"

// //
// // Class to control buttons
// //

// void empty() {}

// //
// // Input::Input() -- Class constructor
// //
// Input::Input(vector<Row> rows, RTC rtc, int buttonOnePin, int buttonTwoPin)
// {
//   this->rows = rows;
//   this->rtc = rtc;
//   buttonOne = new Button(buttonOnePin, empty, empty);
//   buttonTwo = new Button(buttonTwoPin, empty, empty);
// }


// //
// // Input::Init() -- Initialization method
// //
// void Input::Init()
// {
//   buttonOne.Init();
//   buttonTwo.Init();
// }


// //
// // Input::toggleSet() -- Toggle set mode of watch
// //
// void Input::toggleSet()
// {
//   if (isEditingModeEnabled(rows))
//   {
//     deselectRows(rows);
//     tm tm;
//     tm.h = rows[0].timeValue;
//     tm.m = rows[1].timeValue;
//     tm.s = rows[2].timeValue;
//     rtc.SetTime(tm);
//   } else {
//     rows[0].isSelectedForEditing = true;
//   }
// }


// //
// // Input::changeRow() -- Change current row selected
// //
// void Input::changeRow()
// {
//   if (!isEditingModeEnabled(rows))
//     return;

//   int selectedRowIndex = indexOfSelectedRow(rows);
//   rows[selectedRowIndex].isSelectedForEditing = false;
//   rows[selectedRowIndex + 1 % sizeof(rows)].isSelectedForEditing = true;
// }


// //
// // Input::increaseTimeValue() -- Increase time value of currently selected row
// //
// void Input::increaseTimeValue()
// {
//   if (!isEditingModeEnabled(rows))
//     return;

//   int selectedRowIndex = indexOfSelectedRow(rows);
//   rows[selectedRowIndex].timeValue = rows[selectedRowIndex].timeValue + 1 % rows[selectedRowIndex].maxTimeValue;
// }


// //
// // Input::TakeInput() -- Take readings from input sources
// //
// void Input::TakeInput()
// {
//   buttonOne.Poll();
//   buttonTwo.Poll();
// }