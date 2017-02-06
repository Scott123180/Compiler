#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "token.h"

using namespace std;

token genToken(string type, string data, int line, int position);

bool addToBuffer(char c);

string buffer;

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

int main()
{
  bool eof = false; //end of file
  
  int lineNum = 0;

  while(!eof) //loop until end of file
  {
    ++lineNum;
    string curLine;
    getline(cin, curLine); 
   
    buffer = ""; 
    //loop through string
    for(int i = 0; i < curLine.length(); i++)
    {
      //add to buffer to recognize regex patterns 
      if(!addToBuffer(curLine[i])) //try, if fails
      {
        //addToBuffer will print error message
        cout << "Error at line " << lineNum << endl;
        cout << "Error at position " << i << endl;
        return 1; //exit program 
      }
      
      //clear buffer
      string buffer = "";
    } 
  }
  return 0;
}

bool addToBuffer(char c)
{ 
  //ensure symbol is in alphabet
  if(find(acceptedChars.begin(), acceptedChars.end(), c) != vector.end())
  {
    //separator processing
    if(c == '=')
    {

    } 
  }
  //symbol not in alphabet
  else
  {
    cout << "The symbol " << c << " was not found. Aborting lex.";
    return false;
  }
}
