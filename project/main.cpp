#include <iostream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>


#include "token.h"

using namespace std;

bool addToBuffer(char c, int line, int pos, int curLineLen);

void printTokens();

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
      if(!addToBuffer(curLine[i], lineNum, i, static_cast<int>(curLine.length()))) //try, if fails
      {
        //TODO: needs to be removed after testing
        cout << "Error: " << buffer << endl;
        //end TODO

        //addToBuffer will print error message
        cout << "Error at line " << lineNum << " at position " << i << endl;
        return 1; //exit program 
      }
    } 
  }
  
  cout << "End of program reached. Printing tokens to \"lexer.html\"." << endl;
  
  printTokens();
  
  return 0;
}

//returns false if error, adds token to buffer otherwise
bool addToBuffer(char c, int line, int pos, int curLineLen)
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
         return true;
      }

      else //don't know yet
      {
        buffer.push_back(c);
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



void printTokens()
{
  //filestream
  ofstream outputHTML;
  outputHTML.open("lexer.html", ios::out | ios::trunc); //open and clear file

  //html header
  outputHTML << "<!DOCTYPE html> \n"
       << "<html>\n"
       << "<head>\n"
       << "<title>Lex Output</title>\n"
       << "<link rel=\"stylesheet\" type=\"text/css\" href=\"bootstrap/css/bootstrap.css\">"
       << "</head>\n"
       << "<body>\n";

  //table
  outputHTML <<"<table class = \"table\" style=\"width: 30%;\">\n"
       << "<tr>\n"
       << "<th>Data</th>\n"
       << "<th>Type</th>\n"
       << "<th>Line</th>\n"
       << "</tr>\n";
  //table data
  for(int i = 0; i < stream.size(); i++)
  {
    string blank = ""; //gotta love that left association
    outputHTML << blank << "<tr>\n"
         << "<th>" << stream[i].getData() << "</th>\n"
         << "<th>" << "T_" << stream[i].getType() << "</th>\n"
         << "<th>" << stream[i].getLine() << "</th>\n"
         << "</tr>\n";
  }
  //html footer
  outputHTML << "</table>\n"
       << "</body>\n"
       << "</html>";

  outputHTML.close();
}
