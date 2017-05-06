#include "staticDataRow.h"

StaticDataRow::StaticDataRow(string temp, string var, int scope, int offset)
  : temporary(temp), variableName(var), variableScope(scope), variableOffset(offset)
{}

const string &StaticDataRow::getTemporary() const {
  return temporary;
}

const string &StaticDataRow::getVariableName() const {
  return variableName;
}

int StaticDataRow::getVariableScope() const {
  return variableScope;
}

int StaticDataRow::getVariableOffset() const {
  return variableOffset;
}

void StaticDataRow::setTemporary(const string &temporary) {
  StaticDataRow::temporary = temporary;
}

void StaticDataRow::setActualMemoryAddress(const string &actualMemoryAddress) {
  StaticDataRow::actualMemoryAddress = actualMemoryAddress;
}

const string &StaticDataRow::getActualMemoryAddress() const {
  return actualMemoryAddress;
}
