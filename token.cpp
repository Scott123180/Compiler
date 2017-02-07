#include "disease.h"

using namespace std;

class Token
{
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

    //id's | if length one and is a-z, then set type as id
    if(d.length == 1)
    {
      //search through letters
      if(find(letters.begin(), letters.end(), d) != vector.end()) //if in recognizes list id's
      {
      
      }
    }
    //call function to look through map without adding to it if it doesn't find anything
  }
}
