#include "Row.h"

bool isEditingModeEnabled(Vector<Row> rows)
{
  for (int i = 0; i < rows.size(); i++)
  {
    if (rows[i].isSelectedForEditing)
      return true;
  }

  return false;
}

void deselectRows(Vector<Row> rows)
{
  for (int i = 0; i < rows.size(); i++)
  {
    if (rows[i].isSelectedForEditing)
      rows[i].isSelectedForEditing = false;
  }
}

int indexOfSelectedRow(Vector<Row> rows)
{
  for (int i = 0; i < rows.size(); i++)
  {
    if (rows[i].isSelectedForEditing)
      return i;
  }

  return -1;
}