#include "TimeRows.h"


//
// Class responsible for time row state storage.
//

//
// TimeRows::TimeRows() -- Class constructor
//
TimeRows::TimeRows(Vector<Row> rows)
{
  this->rows = rows;
}

//
// TimeRows::isEditingModeEnabled() -- Iterate through rows to determine if isSelectedForEditing is enabled on any row.
//
bool TimeRows::isEditingModeEnabled()
{
  for (int i = 0; i < rows.size(); i++)
  {
    if (rows[i].isSelectedForEditing)
      return true;
  }

  return false;
}


//
// TimeRows::deselectRows() -- Iterate through rows and ensure isSelectedForEditing flag cleared false on each row.
//
void TimeRows::deselectRows()
{
  for (int i = 0; i < rows.size(); i++)
  {
    if (rows[i].isSelectedForEditing)
      rows[i].isSelectedForEditing = false;
  }
}


//
// TimeRows::indexOfSelectedRow() -- Iterate through rows and determine the index of a selected row if one exists.
//
int TimeRows::indexOfSelectedRow()
{
  for (int i = 0; i < rows.size(); i++)
  {
    if (rows[i].isSelectedForEditing)
      return i;
  }

  return -1;
}