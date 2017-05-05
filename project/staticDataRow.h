#ifndef COMPILER_STATICDATAROW_H
#define COMPILER_STATICDATAROW_H

#include <string>

#include "staticDataRow.h"

using namespace std;

class StaticDataRow {

  public:
    StaticDataRow(string temp, string var, int scope, int offset);

    const string &getTemporary() const;

    const string &getVariable() const;

    int getVariableScope() const;

    int getVariableOffset() const;

    void setTemporary(const string &temporary);

  private:
    string temporary;
    string variable;
    int variableScope;
    int variableOffset;
};


#endif //COMPILER_STATICDATAROW_H
