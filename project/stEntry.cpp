#include <string>
#include <iostream>

#include "stEntry.h"

using namespace std;

StEntry::StEntry(string n, string t, int l, bool i)
  : name(n), type(t), lineNum(l), initialized(i)
{}