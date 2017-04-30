#ifndef COMPILER_STATICDATA_H
#define COMPILER_STATICDATA_H


class StaticData {

  public:
    //table for holding static information for vars
      //temp, var, scope, offset
    StaticData() {};

    void addRow(unsigned int temp, char var, unsigned int scope, unsigned int offset);
  private:
};


#endif //COMPILER_STATICDATA_H
