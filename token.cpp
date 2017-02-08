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
  type = Token::findType();
}

string Token::findType() //line num and position for errors
{
  //check the map
  auto it = textToType.find(data);
  if(it != textToType.end()) //if recognized in the map
  {
    type = textToType[data];
  }
  else //not a map character and because of conditions, cannot have a len > 1
  {
    if(d.length > 1) //ERROR: Invalid character
    {
      //TODO: Deal with error 
    } 
    else
    {
      if(find(letters.begin(), letters.end(), data != letters.end()) //true if char
      {
        return "id"; 
      }
      else if(find(digits.begin(), digits.end() data != digits.end()) //true if digit
      {
        return "digit";
      }
      else //ERROR: Invalid id
      {
        //TODO: Deal with error
      }
    }
  }
} 


