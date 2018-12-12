#ifndef ROW_H
#define ROW_H

#include <Vector.h>

struct Row
{
  int timeValue;
  int maxTimeValue;
  int bitLength;
  int ledArrayIndex;
  bool isSelectedForEditing;
};

class TimeRows
{
  public:
    TimeRows(Vector<Row> rows);
    bool isEditingModeEnabled();
    void deselectRows();
    int indexOfSelectedRow();

    Vector<Row> rows;
};

#endif // ROW_H