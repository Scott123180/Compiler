#include <string>
#include <iostream>

#include "stEntry.h"

using namespace std;

StEntry::StEntry(char n, string t, int l, int s, bool i)
  : name(n), type(t), lineNum(l), scope(s), initialized(i)
{}