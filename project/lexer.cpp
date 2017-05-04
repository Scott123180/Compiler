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
    vector<string> f = {fileName}; //create string vector of filename
    Error newError(true, newError.fileInput, 0, 0, f, "Error opening the filename specified: ");
  }

  int lineNum = 0;

  while(!(input.eof())) //loop until end of file
  {
    ++lineNum;
    string curLine;
    getline(input, curLine);

    buffer.clear();

    //loop through string
    for(int i = 0; i < curLine.length(); i++)
    {
      //attempt to add to buffer
      addToBuffer(curLine[i], lineNum, i, static_cast<int>(curLine.length()));
    }
  }

  //programmer forgot the EOP at the end of the file
  if(stream.back().getType() != "EOP")
  {
    cout << "LEX WARNING. Missing '$' (EOP) marker. Inserting one at end." << endl;
    //create token with last token as template
    stream.push_back(Token("$", stream.back().getLine(), (stream.back().getPos() + 1)));
  }
  
  cout << "Lex: End of program reached. Lex successful!" << endl;

  return;
}

Lexer::~Lexer()
{

}

//attempts to add token to buffer, if fails, generates error
void Lexer::addToBuffer(char c, int line, int pos, int curLineLen)
{

  //ensure symbol is in alphabet
  if(find(acceptedChars.begin(), acceptedChars.end(), c) != acceptedChars.end())
  {
    //push '=' separator if not "=="
    if(!(buffer.empty()))
    {
      if(buffer.back() == '=')
      {
        if(c != '=')
        {
          stream.push_back(Token(buffer, line, pos));
          buffer.clear();
        }
      }
    } //continue processing

    //string expression processing
    if(c == '"')
    {

      if(!(inString)) //left quote
      {
        inString = true;
        stream.push_back(Token("leftQuote", string(1,c), line, pos));
        return;
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
        return;
      }
    }



    if(inString) //if in a string expression
    {
      //search for valid char: true if is valid
      if(find(acceptedString.begin(), acceptedString.end(), c) != acceptedString.end())
      {
        buffer.push_back(c);
      }
      else //ERROR:invalid character
      {
        string castChar(1, c);
        vector<string> castVec = {castChar}; //create string vector of data
        Error newError(true, newError.lex, line, pos, castVec, "Invalid character in string expression: ");
        return;
      }
      //got to end of line without end quote
      if ((curLineLen - 1) == pos)
      {
        cout << "Lex error: run-on quote. Ensure string literals start and end on the same line" << endl;
        vector<string> empty = {""}; //create string vector of data
        Error newError(true, newError.lex, line, pos, empty, "Ensure string literals start and end on the same line");
        return;
      }
      return;
    }

    //separator processing

    else if(c == '=')
    {
      //avoid invalid read size with !buffer.empty()
      if(!buffer.empty() && buffer.back() == '=') //   ==
      {
        stream.push_back(Token("==", line, pos));
        buffer.clear();
        return;
      }
      //avoid invalid read size with !buffer.empty()
      else if(!buffer.empty() && buffer.back() == '!') //   !=
      {
        stream.push_back(Token("!=", line, pos));
        buffer.clear();
        return;
      }
      else //don't know yet
      {
        if(!(buffer.empty())) //create token from buffer
        {
          stream.push_back(Token(buffer, line, pos));
          buffer.clear();
        }
        buffer.push_back(c); //put '=' in buffer
        return;
      }

    }

    else if(c == '!') //start of !=
    {
      if(!(buffer.empty())) //if the buffer has item
      {
        stream.push_back(Token(buffer, line, pos));
        buffer = "";
      }
      buffer.push_back(c);  //push back '!'
      return;
    }

    else if(c == '$') //end of program
    {
      buffer.push_back(c);
      stream.push_back(Token(buffer, line, pos));
      buffer="";
      return;
    }
    
    else if(c == ' ') //space (doesn't get tokenized)
    {
      if(!(buffer.empty())) //if the buffer has item
      {
        stream.push_back(Token(buffer, line, pos));
        buffer = "";
      }
      return;
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
      return;
    }
    //end of line
    else if ((curLineLen - 1) == pos)
    {
      buffer.push_back(c); //push remainder of line
      stream.push_back(Token(buffer, line, pos));
      buffer = "";
      return;
    }
    else//regular character processing
    {
      buffer.push_back(c);
      return;
    }

  }
  //symbol not in alphabet
  else
  {
    string castChar(1, c);
    vector<string> castVec = {castChar}; //create string vector of data
    Error newError(true, newError.lex, line, pos, castVec, "This symbol was not found: ");
    return;
  }
}