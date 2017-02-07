#ifindef LEX_TOKEN
#define LEX_TOKEN

#include <string>
#include <vector>
#include <map>

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


#endif
