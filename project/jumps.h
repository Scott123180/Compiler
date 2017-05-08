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

    void  setDistance(string jump, int start, int end);

  private:
    //pair<temp, dist>
    vector<pair<string, string>> rows;

    string intToHex(int a);

};


#endif //COMPILER_JUMPS_H
