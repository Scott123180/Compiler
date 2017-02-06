#ifindef LEX_TOKEN
#define LEX_TOKEN

#include <string>
#include <vector>

class Token
{
  public:
    Token::Token(string t, string d, int l, int p);
  private:
    string type;
    string data;
    int line;
    int position;
    
};

#endif
