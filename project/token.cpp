#include "token.h"

using namespace std;

//type known
Token::Token(string t, string d, int l, int p) : type(t), data(d), line(l), position(p) {
  if(t == "leftQuote") {}
  else if(t == "rightQuote") {}
  else if(t == "charList") {}
 }
//type unknown
Token::Token(string d, int l, int p) : data(d), line(l), position(p)
{
  //based on type, do conversions / checking 
  type = Token::findType(data);
}

string Token::findType(string tContent) //line num and position for errors
{
  //check the map
  auto it = textToType.find(tContent);
  if(it != textToType.end()) //if recognized in the map
  {
    return textToType[tContent];
  }
  else //not a map character and because of conditions, cannot have a len > 1
  {
    if(tContent.length() > 1) //ERROR: Invalid character
    {
      //TODO: Deal with error
      return "bologna data.length";
    } 
    else
    {
      if(find(letters.begin(), letters.end(), tContent) != letters.end()) //true if char
      {
        return "id"; 
      }
      else if(find(digits.begin(), digits.end(), tContent) != digits.end()) //true if digit
      {
        return "digit";
      }
      else //ERROR: Invalid id
      {
        //TODO: Deal with error
        return "bologna invalid id";
      }
    }
  }
}

string Token::getType() {return type;}
string Token::getData() {return data;}
int Token::getLine() { return line;}
int Token::getPos() { return position;}


