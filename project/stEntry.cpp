#include <string>
#include <iostream>

#include "stEntry.h"

using namespace std;

/* Character As Name (no extra modification needed) */
//generic
StEntry::StEntry(char n, string t, int l, int p, int s, bool i)
  : name(n), type(t), lineNum(l), position(p), scope(s), initialized(i)
{}

//Integer
StEntry::StEntry(char n, string t, int l, int p, int s, bool i, int d)
  : name(n), type(t), lineNum(l), position(p), scope(s), initialized(i), digit(d)
{}
//String
StEntry::StEntry(char n, string t, int l, int p, int s, bool i, string c)
  : name(n), type(t), lineNum(l), position(p), scope(s), initialized(i), charList(c)
{}
//Boolean
StEntry::StEntry(char n, string t, int l, int p, int s, bool i, bool b)
  : name(n), type(t), lineNum(l), position(p), scope(s), initialized(i), boolean(b)
{}

/* String As Name (need to convert string to character */

//generic
StEntry::StEntry(string n, string t, int l, int p, int s, bool i)
  : type(t), lineNum(l), position(p), scope(s), initialized(i)
{
  //get first char of name
  name = n.front();
}

//Integer
StEntry::StEntry(string n, string t, int l, int p, int s, bool i, int d)
  : type(t), lineNum(l), position(p), scope(s), initialized(i), digit(d)
{
  //get first char of name
  name = n.front();
}
//String
StEntry::StEntry(string n, string t, int l, int p, int s, bool i, string c)
  : type(t), lineNum(l), position(p), scope(s), initialized(i), charList(c)
{
  //get first char of name
  name = n.front();
}
//Boolean
StEntry::StEntry(string n, string t, int l, int p, int s, bool i, bool b)
  : type(t), lineNum(l), position(p), scope(s), initialized(i), boolean(b)
{
  //get first char of name
  name = n.front();
}
