#include "staticDataRow.h"

StaticDataRow::StaticDataRow(string temp, string var, int scope, int offset)
  : temporary(temp), variable(var), variableScope(scope), variableOffset(offset)
{}

const string &StaticDataRow::getTemporary() const {
  return temporary;
}

const string &StaticDataRow::getVariable() const {
  return variable;
}

int StaticDataRow::getVariableScope() const {
  return variableScope;
}

int StaticDataRow::getVariableOffset() const {
  return variableOffset;
}
