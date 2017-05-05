#include "jumpsRow.h"

JumpsRow::JumpsRow(string temp, int dist)
  : temporary(temp), distance(dist)
{}

const string &JumpsRow::getTemporary() const {
  return temporary;
}

int JumpsRow::getDistance() const {
  return distance;
}
