#ifndef COMPILER_STATICDATAROW_H
#define COMPILER_STATICDATAROW_H

#include <string>

using namespace std;

class StaticDataRow {

  public:
    StaticDataRow(string temp, string var, int scope, int offset);

  private:
    string temporary;
  public:
    const string &getTemporary() const;

    const string &getVariable() const;

    int getVariableScope() const;

    int getVariableOffset() const;

  private:
    string variable;
    int variableScope;
    int variableOffset;
};


#endif //COMPILER_STATICDATAROW_H
