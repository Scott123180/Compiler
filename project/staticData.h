#ifndef COMPILER_STATICDATA_H
#define COMPILER_STATICDATA_H

#include <string>

using namespace std;

class StaticData {

  public:
    //table for holding static information for vars
      //temp, var, scope, offset
    StaticData() {};

    void addRow(string var, unsigned int scope);

  private:


    //holds the number of the current temporary variable for static var table
    int tempNum = 0;
};


#endif //COMPILER_STATICDATA_H
