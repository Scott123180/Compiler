#ifndef COMPILER_STATICDATA_H
#define COMPILER_STATICDATA_H


class StaticData {

  public:
    //table for holding static information for vars
      //temp, var, scope, offset
    StaticData() {};

    void addRow(char var, unsigned int scope, unsigned int offset);

  private:


    //holds the number of the current temporary variable for static var table
    int tempNum = 0;
};


#endif //COMPILER_STATICDATA_H
