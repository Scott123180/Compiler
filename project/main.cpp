#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "token.h"

using namespace std;

bool addToBuffer(char c, int line, int pos);

void printTokens();

bool previousWasEq = false; //for "==" checking

bool inString = false; //for string expressions

string buffer = "";

bool eop = false; //end of program

vector<Token> stream;


//defined characters
vector<char> acceptedChars = {
'a', 'b', 'c', 'd', 'e', 'f',
'g', 'h', 'i', 'j', 'k', 'l',
'm', 'n', 'o', 'p', 'q', 'r',
's', 't', 'u', 'v', 'w', 'x',
'y', 'z', '0', '1', '2', '3',
'4', '5', '6', '7', '8', '9',
'=', ' ', '!', '+', '$', '{',
'}', '(', ')', '"', ' '
};

//accepted string literal characters:any characters a...z and spaces
vector<char> acceptedString = {
'a', 'b', 'c', 'd', 'e', 'f',
'g', 'h', 'i', 'j', 'k', 'l',
'm', 'n', 'o', 'p', 'q', 'r',
's', 't', 'u', 'v', 'w', 'x',
'y', 'z', ' '
};



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
  //reset equal condition
  if(c != '=') previousWasEq = false;

  //ensure symbol is in alphabet
  if(find(acceptedChars.begin(), acceptedChars.end(), c) != acceptedChars.end())
  {
    //string expression processing
    if(c == '"')
    {
      if(!(inString)) //left quote
      {
        inString = true;
        stream.push_back(Token("leftQuote", string(1,c), line, pos));
        return true;
      }
      else //right quote
      {
        inString = false;
        stream.push_back(Token("charList", buffer, line, pos)); //tokenize buffer
        buffer = "";
        stream.push_back(Token("rightQuote", string(1,c), line, pos));
        return true;
      }
    }
   
 
    if(inString) //if in a string expression
    {
      //search for valid char: true if is valid
      if(find(acceptedString.begin(), acceptedString.end(), c) != acceptedString.end())
      {
        buffer.push_back(c);
        return true;
      }
   
      else //ERROR:invalid character
      {
        cout << "Lex error: character: " << c 
        << " is not a valid character in a string expression." << endl;
        return false;
      }
    }
    //separator processing
    else if(c == '=')
    {
      if(buffer.back() == '=') //double ==
      {
         stream.push_back(Token("==", line, pos));
         buffer = "";
         previousWasEq = false;
      }

      else //don't know yet 
      {
        buffer.push_back(c);
        previousWasEq = true;
      }

    }

    else if(c == '$') //end of file
    {
      eop = true;
      return true;
    }
    
    else if(c == ' ') //space (doesn't get tokenized)
    {
      stream.push_back(Token(buffer, line, pos));
      buffer = "";
    }
    //regular separators
    else if(c == '{' || '}' || '(' || ')')
    {
      stream.push_back(Token(buffer, line, pos)); //push back buffer
      buffer = "";
    }
    
    
    else//regular character processing
    {
        buffer.push_back(c);
    }

      //TODO: see if tab '\t' is works in input
 
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
  for(int i = 0; i < stream.size(); i++)
  {
    cout << i << ". Data: " << stream.at(i).getData() << " Type: " << stream.at(i).getType()
    << " Line: " << stream.at(i).getLine() << endl;
  }
}
