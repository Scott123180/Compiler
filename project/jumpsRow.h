#ifndef COMPILER_JUMPSROW_H
#define COMPILER_JUMPSROW_H

#include <string>

using namespace std;

class JumpsRow {
  public:
    JumpsRow(string temp, int dist);

  private:
    string temporary;
  public:
    const string &getTemporary() const;

    int getDistance() const;

  private:
    int distance;

};


#endif //COMPILER_JUMPSROW_H
