#ifndef ROW_H
#define ROW_H

struct Row {
  int timeValue;
  int maxTimeValue;
  int bitLength;
  int ledArrayIndex;
  bool isSelectedForEditing;
};

bool isRowSelected(Row rows[])
{
  bool isEditingModeEnabled = false;

  for (int i = 0; i < sizeof(rows); i++)
  {
    if (rows[i].isSelectedForEditing)
      return true;
  }

  return false;
}

void deselectRows(Row rows[])
{
  for (int i = 0; i < sizeof(rows); i++)
  {
    if (rows[i].isSelectedForEditing)
      rows[i].isSelectedForEditing = false;
  }
}

int indexOfSelectedRow(Row rows[])
{
  for (int i = 0; i < sizeof(rows); i++)
  {
    if (rows[i].isSelectedForEditing)
      return i;
  }

  return -1;
}

#endif // ROW_H