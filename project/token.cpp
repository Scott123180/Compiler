#include "token.h"

using namespace std;

//type known
Token::Token(string t, string d, int l, int p) : type(t), data(d), line(l), position(p) {
  if(t == "leftQuote") type = "leftQuote";
  else if(t == "rightQuote") type = "rightQuote";
  else if(t == "charList") type = "charList";
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
    return textToType[data];
  }
  else //not a map character and because of conditions, cannot have a len > 1
  {
    if(data.length() > 1) //ERROR: Invalid character
    {
      //Exit execution if character invalid
      cout << "Error in token creation." << endl;
      cout << "Invalid input \"" << data << "\" at line " << line << " position: " << position << endl;
      exit(1);
    } 
    else
    {
      if(find(letters.begin(), letters.end(), data) != letters.end()) //true if char
      {
        return "id"; 
      }
      else if(find(digits.begin(), digits.end(), data) != digits.end()) //true if digit
      {
        return "digit";
      }
      else //ERROR: Invalid id
      {
        //Exit execution if character invalid
        cout << "Error in token creation." << endl;
        cout << "Invalid id \"" << data << "\" at line " << line << " position: " << position << endl;
        exit(1);
      }
    }
  }
}

string Token::getType() {return type;}
string Token::getData() {return data;}
int Token::getLine() { return line;}
int Token::getPos() { return position;}


