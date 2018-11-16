#ifndef ROW_H
#define ROW_H

#include <Vector.h>

struct Row {
  int timeValue;
  int maxTimeValue;
  int bitLength;
  int ledArrayIndex;
  bool isSelectedForEditing;
};

bool isEditingModeEnabled(Vector<Row> rows);
void deselectRows(Vector<Row> rows);
int indexOfSelectedRow(Vector<Row> rows);

#endif // ROW_H