#ifindef LEX_TOKEN
#define LEX_TOKEN

#include <string>
#include <vector>
#include <map>
#include <algorithm>

class Token
{
  public:
    Token(string t, string d, int l, int p);
    Token(string d,int l, int p);
  private:
    string type;
    string data;
    int line;
    int position;
    
    map textToType<string, string> = {
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
    vector letters<string> = {
    "a", "b", "c", "d", "e", "f",
    "g", "h", "i", "j", "k", "l",
    "m", "n", "o", "p", "q", "r",
    "s", "t", "u", "v", "w", "x",
    "y", "z"};
    
    string findType()
    {
      auto it = textToType.find(data);
      if(it != textToType.end()) //if recognized in the map
      {
        type = textToType[data];
      }
      else
      {
        //Error
      }
    } 
};


#endif
