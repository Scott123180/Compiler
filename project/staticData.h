#ifndef COMPILER_STATICDATA_H
#define COMPILER_STATICDATA_H

#include <string>
#include <vector>

using namespace std;

class StaticData {

  public:
    //table for holding static information for vars
      //temp, var, scope, offset
    StaticData() {};

    string addRow(string var, unsigned int scope);

  private:

};


#endif //COMPILER_STATICDATA_H
