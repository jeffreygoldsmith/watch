#include "TimeRows.h"

TimeRows::TimeRows(Vector<Row> rows)
{
  this->rows = rows;
}

bool TimeRows::isEditingModeEnabled()
{
  for (int i = 0; i < rows.size(); i++)
  {
    if (rows[i].isSelectedForEditing)
      return true;
  }

  return false;
}


void TimeRows::deselectRows()
{
  for (int i = 0; i < rows.size(); i++)
  {
    if (rows[i].isSelectedForEditing)
      rows[i].isSelectedForEditing = false;
  }
}


int TimeRows::indexOfSelectedRow()
{
  for (int i = 0; i < rows.size(); i++)
  {
    if (rows[i].isSelectedForEditing)
      return i;
  }

  return -1;
}