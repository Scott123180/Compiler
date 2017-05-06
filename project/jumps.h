#ifndef COMPILER_JUMPS_H
#define COMPILER_JUMPS_H

#include <vector>
#include <utility>
#include <string>

using namespace std;

class Jumps {

  public:
    //table for holding jump information
      //temp, distance
    Jumps() {};

    string addRow(unsigned int dist);
  private:
    //pair<temp, dist>
    vector<pair<int, int>> rows;
};


#endif //COMPILER_JUMPS_H
