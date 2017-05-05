#ifndef COMPILER_JUMPSROW_H
#define COMPILER_JUMPSROW_H

#include <string>

using namespace std;

class JumpsRow {
  public:
    JumpsRow(string temp, int dist);

    const string &getTemporary() const;

    int getDistance() const;

    void setTemporary(const string &temporary);

  private:
    string temporary;

    int distance;

};


#endif //COMPILER_JUMPSROW_H