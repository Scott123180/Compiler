#ifndef COMPILER_STENTRY_H
#define COMPILER_STENTRY_H

#include <string>
#include <iostream>


using namespace std;

class StEntry
{
  public:
    /* Character As Name (no extra modification needed) */
    //Generic - name, type, lineNum, scope, utilized
    StEntry(char n, string t, int l, int p, int s, bool i);

    //Integer - name, type, lineNum, scope, utilized, digit
    StEntry(char n, string t, int l, int p, int s, bool i, int d);

    //String - name, type, lineNum, scope, utilized, charList
    StEntry(char n, string t, int l, int p, int s, bool i, string c);

    //Boolean - name, type, lineNum, scope, utilized, boolean
    StEntry(char n, string t, int l, int p, int s, bool i, bool b);

    /* String As Name (need to convert string to character */
    //generic
    StEntry::StEntry(string n, string t, int l, int p, int s, bool i);

    //Integer
    StEntry::StEntry(string n, string t, int l, int p, int s, bool i, int d);

    //String
    StEntry::StEntry(string n, string t, int l, int p, int s, bool i, string c);

    //Boolean
    StEntry::StEntry(string n, string t, int l, int p, int s, bool i, bool b);

    char name;
    string type;
    int lineNum;
    int position;
    int scope;
    bool initialized;
    bool utilized = false;

    //keep track of whether or not values have been set
    bool hasBoolBeenSet = false;
    bool hasIntBeenSet = false;
    bool hasStringBeenSet = false;

    //data getters - can control later if want to fancy up
    //let's trust the programmer now lol
    int getDigit() {return digit;}
    string getcharList(){return charList;}
    bool getBoolean(){return boolean;}

    //data setters - can control later if want to fancy up
    //let's trust the programmer now lol
    void setDigit(int d)
    {
      hasIntBeenSet = true;
      digit = d;
    }
    void setCharList(string s)
    {
      hasStringBeenSet = true;
      charList = s;
    }
    void setBoolean(bool b)
    {
      hasBoolBeenSet = true;
      boolean = b;
    }

  private:
    //these are the possible types, let's control with getters and setters
    //initialize to dummy values
    int digit;
    string charList;
    bool boolean;
};

#endif