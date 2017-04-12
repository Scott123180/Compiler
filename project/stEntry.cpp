#include <string>
#include <iostream>

#include "stEntry.h"

using namespace std;

//generic
StEntry::StEntry(char n, string t, int l, int s, bool i)
  : name(n), type(t), lineNum(l), scope(s), initialized(i)
{}

//Integer
StEntry::StEntry(char n, string t, int l, int s, bool i, int d)
  : name(n), type(t), lineNum(l), scope(s), initialized(i), digit(d)
{}
//String
StEntry::StEntry(char n, string t, int l, int s, bool i, string c)
  : name(n), type(t), lineNum(l), scope(s), initialized(i), charList(c)
{}
//Boolean
StEntry::StEntry(char n, string t, int l, int s, bool i, bool b)
  : name(n), type(t), lineNum(l), scope(s), initialized(i), boolean(b)
{}