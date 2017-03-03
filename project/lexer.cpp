#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

#include "token.h"
#include "lexer.h"

using namespace std;

void Lexer::Lex(string fileName)
{
  ifstream input;
  input.open(fileName); //open file

  if (input.fail()) //check for error in file reading
  {
    cerr << "Error opening parameter file for simulation" << endl;
    exit(1); //exit the program
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
      //add to buffer to recognize regex patterns 
      if(!addToBuffer(curLine[i], lineNum, i, static_cast<int>(curLine.length()))) //try, if fails
      {

        cout << "Error: " << buffer << endl;
        //addToBuffer will print error message
        cout << "Error at line " << lineNum << " at position " << i << endl;
        return 1; //exit program 
      }
    } 
  }
  
  cout << "End of program reached. Printing tokens to \"output.html\"." << endl;

  return;
}

//returns false if error, adds token to buffer otherwise
bool Lexer::addToBuffer(char c, int line, int pos, int curLineLen)
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
        if(!(buffer.empty())) //don't make empty charlist
        {
        stream.push_back(Token("charList", buffer, line, pos)); //tokenize buffer
        }
        buffer = "";
        stream.push_back(Token("rightQuote", string(1,c), line, pos));
        return true;
      }
    }

    //push separator if not "=="
    if(!(buffer.empty()))
    {
      if(buffer.back() == '=')
      {
        if(c != '=')
        {
          stream.push_back(Token(buffer, line, pos));
          buffer = "";
        }

      }
    } //continue processing

    if(inString) //if in a string expression
    {
      //search for valid char: true if is valid
      if(find(acceptedString.begin(), acceptedString.end(), c) != acceptedString.end())
      {
        buffer.push_back(c);
      }
      else //ERROR:invalid character
      {
        cout << "Lex error: character: " << c 
        << " is not a valid character in a string expression." << endl;
        return false;
      }
      //got to end of line without end quote
      if ((curLineLen - 1) == pos)
      {
        cout << "Lex error: run-on quote. Ensure string literals start and end on the same line" << endl;
        return false;
      }
      return true;
    }

    //separator processing

    else if(c == '=')
    {
      if(buffer.back() == '=') //double ==
      {
         stream.push_back(Token("==", line, pos));
         buffer = "";
         return true;
      }

      else //don't know yet
      {
        if(!(buffer.empty())) //create token from buffer
        {
          stream.push_back(Token(buffer, line, pos));
          buffer = "";
        }
        buffer.push_back(c); //put '=' in buffer
        return true;
      }

    }

    else if(c == '$') //end of program
    {
      buffer.push_back(c);
      stream.push_back(Token(buffer, line, pos));
      buffer="";
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
    else if((c == '{') || (c =='}') || (c == '(') || (c == ')' || c == '+'))
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
    //end of line
    else if ((curLineLen - 1) == pos)
    {
      buffer.push_back(c); //push remainder of line
      stream.push_back(Token(buffer, line, pos));
      buffer = "";
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