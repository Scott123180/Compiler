#ifndef COMPILER_STATICDATA_H
#define COMPILER_STATICDATA_H

#include <string>
#include <vector>

#include "token.h"
#include "staticDataRow.h"

using namespace std;

class StaticData {

  public:
    //table for holding static information for vars
      //temp, var, scope, offset
    StaticData() {};

    string addRow(Token* a);

    vector<StaticDataRow> data = {};

  private:

};


#endif //COMPILER_STATICDATA_H
