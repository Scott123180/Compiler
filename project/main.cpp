#include <iostream>
#include <vector>
#include <algorithm>

#include "token.h"

using namespace std;

bool addToBuffer(char c, int line, int pos);

void printTokens();

bool previousWasEq = false; //for "==" checking

bool inString = false; //for string expressions

string buffer = "";

bool eop = false; //end of program

vector<Token> stream = {};


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
        //TODO: needs to be removed after testing
        printTokens();
        cout << buffer << endl;
        //end TODO

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
/*
    //"=" processing
    if((c != '=') && previousWasEq && !(inString))
    {
      previousWasEq = false;
      cout << "pushing extra = " << endl;
      stream.push_back(Token(buffer, line, (pos - 1)));
    } //resume current character processing
*/
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
      if(previousWasEq) //double ==
      {
         stream.push_back(Token("==", line, pos));
         buffer = "";
         previousWasEq = false;
         return true;
      }

      else //don't know yet
      {
        buffer.push_back(c);
        previousWasEq = true;
        return true;
      }

    }

    else if(c == '$') //end of file
    {
      eop = true;
      buffer.push_back(c);
      stream.push_back(Token(buffer, line, pos));
      return true;
    }
    
    else if(c == ' ') //space (doesn't get tokenized)
    {
      if(!(buffer.empty())) //if the buffer has item
      {
        stream.push_back(Token(buffer, line, pos));
        buffer = "";
      }
      return true;
    }
    //regular separators
    else if((c == '{') || (c =='}') || (c == '(') || (c == ')'))
    {
      if(!(buffer.empty())) //buffer has contents
      {
        stream.push_back(Token(buffer, line, pos)); //push back buffer
        stream.push_back(Token(string(1,c), line, pos)); //push back separator
        buffer = ""; //clear buffer
      }
      else //buffer clear
      {
        stream.push_back(Token(string(1,c), line, pos)); //push back separator
      }
      return true;
    }

    else//regular character processing
    {
        buffer.push_back(c);
        return true;
    }

  }
  //symbol not in alphabet
  else
  {
    cout << "The symbol \"" << c <<"\" was not found. Aborting lex." << endl <<
         "Line: " << line << " position: " << pos << endl;
    exit(1);
  }
}

void printTokens()
{
  for(int i = 0; i < stream.size(); i++)
  {
    cout << i << ". Data: " << stream[i].getData() << " Type: T_" << stream[i].getType()
    << " Line: " << stream[i].getLine() << endl;
  }
}
