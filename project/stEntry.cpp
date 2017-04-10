#include <string>
#include <iostream>

#include "stEntry.h"

using namespace std;

StEntry::StEntry(string n, string t, unsigned int s, int l, bool i)
  : name(n), type(t), scope(s), lineNum(l), initialized(i)
{}