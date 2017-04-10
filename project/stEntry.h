#ifndef COMPILER_STENTRY_H
#define COMPILER_STENTRY_H

#include <string>
#include <iostream>


using namespace std;

class StEntry
{
  public:
    StEntry(string n, string t, unsigned int s, int l, bool i);

    string name;
    string type;
    unsigned int scope;
    int lineNum;
    bool initialized;
    bool utilized;

  private:
};

#endif