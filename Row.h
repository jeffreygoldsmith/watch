#ifndef ROW_H
#define ROW_H

#include <ArduinoSTL.h>

using namespace std;

struct Row {
  int timeValue;
  int maxTimeValue;
  int bitLength;
  int ledArrayIndex;
  bool isSelectedForEditing;
};

bool isEditingModeEnabled(vector<Row> rows)
{
  for (int i = 0; i < rows.size(); i++)
  {
    if (rows[i].isSelectedForEditing)
      return true;
  }

  return false;
}

void deselectRows(vector<Row> rows)
{
  for (int i = 0; i < rows.size(); i++)
  {
    if (rows[i].isSelectedForEditing)
      rows[i].isSelectedForEditing = false;
  }
}

int indexOfSelectedRow(vector<Row> rows)
{
  for (int i = 0; i < rows.size(); i++)
  {
    if (rows[i].isSelectedForEditing)
      return i;
  }

  return -1;
}

#endif // ROW_H