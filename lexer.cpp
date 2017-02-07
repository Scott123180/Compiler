#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "token.h"

using namespace std;

bool addToBuffer(char c);

string buffer = "";

vector<char> acceptedChars = {
'a', 'b', 'c', 'd', 'e', 'f',
'g', 'h', 'i', 'j', 'k', 'l',
'm', 'n', 'o', 'p', 'q', 'r',
's', 't', 'u', 'v', 'w', 'x',
'y', 'z', '0', '1', '2', '3',
'4', '5', '6', '7', '8', '9',
'=', ' ', '!', '+', '$', '{',
'}', '(', ')', '"'
};

vector<Token> stream;

bool eop = false; //end of program

int main()
{
  
  int lineNum = 0;

  while(!eop) //loop until end of file
  {
    ++lineNum;
    string curLine;
    getline(cin, curLine); 
   
    buffer = "";
     
    //loop through string
    for(int i = 0; i < curLine.length(); i++)
    {
      //add to buffer to recognize regex patterns 
      if(!addToBuffer(curLine[i], lineNum, i)) //try, if fails
      {
        //addToBuffer will print error message
        cout << "Error at line " << lineNum << endl;
        cout << "Error at position " << i << endl;
        return 1; //exit program 
      }
    } 
  }
  
  cout << "End of program reached. Printing tokens" << endl;
  
  printTokens();
  
  return 0;
}

//returns false if error, adds token to buffer otherwise
bool addToBuffer(char c, int line, int pos)
{ 
  //ensure symbol is in alphabet
  if(find(acceptedChars.begin(), acceptedChars.end(), c) != vector.end())
  {
    //separator processing
    if(c == '=')
    {
      if(buffer.back() == '=') //double ==
      {
         stream.push_back(Token::genToken("==", line, pos));
         buffer = "";
      }

      else //don't know yet 
      {
        buffer.push_back(c);
      }

    }

    else if(c == '$') //end of file
    {
      eop = true;
      return true;
    }

    else if(c == ' ') //space
    {
      stream.push_back(Token::genToken(buffer, line, pos));
      buffer = "";
    }
    //TODO: see if tab '\t\' is works in input
    
    //regular character processing
    buffer.push_back(c);
    

 
  }
  //symbol not in alphabet
  else
  {
    cout << "The symbol " << c << " was not found. Aborting lex.";
    return false;
  }
}

void printTokens()
{

}
