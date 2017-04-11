#ifndef COMPILER_STENTRY_H
#define COMPILER_STENTRY_H

#include <string>
#include <iostream>


using namespace std;

class StEntry
{
  public:
    StEntry(char n, string t, int l, int s, bool i);

    char name;
    string type;
    int lineNum;
    int scope;
    bool initialized;
    bool utilized;

    //data getters - can control later if want to fancy up
    //let's trust the programmer now lol
    int getDigit() {return digit;}
    string getcharList(){return charList;}
    bool getBoolean(){return boolean;}

    //data setters - can control later if want to fancy up
    //let's trust the programmer now lol
    void setDigit(int d) {digit = d;}
    void setCharList(string s) {charList = s;}
    void setBoolean(bool b) {boolean = b;}

  private:
    //these are the possible types, let's control with getters and setters
    //initialize to dummy values
    int digit = -1;
    string charList = "\0";
    bool boolean = false;
};

#endif