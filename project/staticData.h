#ifndef COMPILER_STATICDATA_H
#define COMPILER_STATICDATA_H

#include <string>
#include <vector>

#include "token.h"
#include "staticDataRow.h"

using namespace std;

class StaticData {

  public:

    vector<StaticDataRow> data = {};

    //table for holding static information for vars
      //temp, var, scope, offset
    StaticData() {};

    string addRow(Token* a);

    string lookupRow(string varName, int uniqueScope);

  private:

};


#endif //COMPILER_STATICDATA_H
