#ifndef LEX_TOKEN_H
#define LEX_TOKEN_H

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

    //branch token, just type
    Token(string t);


    string getType();
    string getData();
    int getLine();
    int getPos();

    void setType(string a){type = a;}
    void setData(string a){data = a;}

    string findType();
    
    //for use in trees
    Token* parent;
    vector<Token*> children;

    //for use in in-order traversal
    bool visited = false;

  private:
    string type;
    string data;
    int line;
    int position;
    
    map<string, string> textToType = {
      {"print", "print"}, //special terminal
      {"boolean", "type"},
      {"int", "type"},
      {"string", "type"},
      {"=", "assign"},  //special terminal
      {"==", "boolOp"},
      {"!=", "boolOp"},
      {"+", "intOp"},
      {"true", "boolVal"},
      {"false", "boolVal"},
      {"while", "while"}, //special terminal
      {"if", "if"}, //special terminal
      {"{", "leftBrace"},
      {"}", "rightBrace"},
      {"(", "leftParen"},
      {")", "rightParen"},
      {"$", "EOP"}
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



#endif  //LEX_TOKEN_H
