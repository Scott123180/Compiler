#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

#include "token.h"
#include "lexer.h"
#include "error.h"

using namespace std;

Lexer::Lexer(string fileName)
{

  ifstream input;
  input.open(fileName); //open file

  if (input.fail()) //check for error in file reading
  {
    Error newError(true, newError.fileInput, 0, 0, fileName, "Error opening the filename specified: ");
  }

  int lineNum = 0;

  while(!(input.eof())) //loop until end of file
  {
    ++lineNum;
    string curLine;
    getline(input, curLine);

    Lexer::buffer = "";

    //loop through string
    for(int i = 0; i < curLine.length(); i++)
    {
      //attempt to add to buffer
      addToBuffer(curLine[i], lineNum, i, static_cast<int>(curLine.length()));
    } 
  }
  
  cout << "End of program reached. Printing tokens to \"output.html\"." << endl;

  return;
}

//returns false if error, adds token to buffer otherwise
void Lexer::addToBuffer(char c, int line, int pos, int curLineLen)
{

  //ensure symbol is in alphabet
  if(find(Lexer::acceptedChars.begin(), Lexer::acceptedChars.end(), c) != Lexer::acceptedChars.end())
  {

    //string expression processing
    if(c == '"')
    {

      if(!(Lexer::inString)) //left quote
      {
        Lexer::inString = true;
        Lexer::stream.push_back(Token("leftQuote", string(1,c), line, pos));
        return;
      }
      else //right quote
      {
        Lexer::inString = false;
        if(!(Lexer::buffer.empty())) //don't make empty charlist
        {
          Lexer::stream.push_back(Token("charList", Lexer::buffer, line, pos)); //tokenize buffer
        }
        Lexer::buffer = "";
        Lexer::stream.push_back(Token("rightQuote", string(1,c), line, pos));
        return;
      }
    }

    //push separator if not "=="
    if(!(Lexer::buffer.empty()))
    {
      if(Lexer::buffer.back() == '=')
      {
        if(c != '=')
        {
          Lexer::stream.push_back(Token(Lexer::buffer, line, pos));
          Lexer::buffer = "";
        }

      }
    } //continue processing

    if(Lexer::inString) //if in a string expression
    {
      //search for valid char: true if is valid
      if(find(Lexer::acceptedString.begin(), Lexer::acceptedString.end(), c) != Lexer::acceptedString.end())
      {
        Lexer::buffer.push_back(c);
      }
      else //ERROR:invalid character
      {
        string castChar(1, c);
        Error newError(true, newError.lex, line, pos, castChar, "Invalid character in string expression: ");
        return;
      }
      //got to end of line without end quote
      if ((curLineLen - 1) == pos)
      {
        cout << "Lex error: run-on quote. Ensure string literals start and end on the same line" << endl;
        Error newError(true, newError.lex, line, pos, "", "Ensure string literals start and end on the same line");
        return;
      }
      return;
    }

    //separator processing

    else if(c == '=')
    {
      if(Lexer::buffer.back() == '=') //double ==
      {
        Lexer::stream.push_back(Token("==", line, pos));
        Lexer::buffer = "";
        return;
      }

      else //don't know yet
      {
        if(!(Lexer::buffer.empty())) //create token from buffer
        {
          Lexer::stream.push_back(Token(Lexer::buffer, line, pos));
          Lexer::buffer = "";
        }
        Lexer::buffer.push_back(c); //put '=' in buffer
        return;
      }

    }

    else if(c == '$') //end of program
    {
      Lexer::buffer.push_back(c);
      Lexer::stream.push_back(Token(Lexer::buffer, line, pos));
      Lexer::buffer="";
      return;
    }
    
    else if(c == ' ') //space (doesn't get tokenized)
    {
      if(!(Lexer::buffer.empty())) //if the buffer has item
      {
        Lexer::stream.push_back(Token(Lexer::buffer, line, pos));
        Lexer::buffer = "";
      }
      return;
    }
    //regular separators
    else if((c == '{') || (c =='}') || (c == '(') || (c == ')' || c == '+'))
    {
      if(!(Lexer::buffer.empty())) //buffer has contents
      {
        Lexer::stream.push_back(Token(Lexer::buffer, line, pos)); //push back buffer
        Lexer::stream.push_back(Token(string(1,c), line, pos)); //push back separator
        Lexer::buffer = ""; //clear buffer
      }
      else //buffer clear
      {
        Lexer::stream.push_back(Token(string(1,c), line, pos)); //push back separator
      }
      return;
    }
    //end of line
    else if ((curLineLen - 1) == pos)
    {
      Lexer::buffer.push_back(c); //push remainder of line
      Lexer::stream.push_back(Token(Lexer::buffer, line, pos));
      Lexer::buffer = "";
      return;
    }
    else//regular character processing
    {
      Lexer::buffer.push_back(c);
        return;
    }

  }
  //symbol not in alphabet
  else
  {
    string castChar(1, c);
    Error newError(true, newError.lex, line, pos, castChar, "This symbol was not found: ");
    return;
  }
}