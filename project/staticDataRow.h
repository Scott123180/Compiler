#ifndef COMPILER_STATICDATAROW_H
#define COMPILER_STATICDATAROW_H

#include <string>

#include "staticDataRow.h"

using namespace std;

class StaticDataRow {

  public:
    StaticDataRow(string temp, string var, int scope, int offset);
    
    const string &getTemporary() const;

    const string &getVariableName() const;

    int getVariableScope() const;

    int getVariableOffset() const;

    void setTemporary(const string &temporary);

    void setActualMemoryAddress(const string &actualMemoryAddress);

    const string &getActualMemoryAddress() const;
  private:
    string temporary;
    string variableName;
    int variableScope;
    int variableOffset;

    //for use after in back-patching
    string actualMemoryAddress;

};


#endif //COMPILER_STATICDATAROW_H
