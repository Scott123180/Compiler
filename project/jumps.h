#ifndef COMPILER_JUMPS_H
#define COMPILER_JUMPS_H

#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class Jumps {

  public:
    //table for holding jump information
      //temp, distance
    Jumps() {};

    string addRow();

    int calculateDistance(int start, int end);

    //pair<temp, dist>
    vector<string> rows;
  private:


};


#endif //COMPILER_JUMPS_H
