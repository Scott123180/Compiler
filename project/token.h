#ifndef LEX_TOKEN
#define LEX_TOKEN

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>

using namespace std;

class Token
{
  public:
    Token(string t, string d, int l, int p);
    Token(string d,int l, int p);
    string getType();
    string getData();
    int getLine();
    int getPos();

    string findType();

  private:
    string type;
    string data;
    int line;
    int position;
    
    map<string, string> textToType = {
      {"print", "typePrint"},
      {"int", "typeInt"},
      {"string", "typeString"}, 
      {"boolean", "typeBoolean"},
      {"=", "assignOp"},
      {"==", "boolOp"},
      {"!=", "boolOp"},
      {"true", "boolVal"},
      {"false", "boolVal"},
      {"while", "whileStatement"},
      {"if", "ifStatement"},
      {"{", "leftBrace"},
      {"}", "rightBrace"},
      {"(", "leftParen"},
      {")", "rightParen"}
    };
    vector<string> letters= {
    "a", "b", "c", "d", "e", "f",
    "g", "h", "i", "j", "k", "l",
    "m", "n", "o", "p", "q", "r",
    "s", "t", "u", "v", "w", "x",
    "y", "z"};
    vector<string> digits = {
    "0", "1", "2", "3", "4",
    "5", "6", "7", "8", "9"
    };
   
 

};



#endif
