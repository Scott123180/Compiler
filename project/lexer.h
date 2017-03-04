#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H

#include <vector>
#include <string>

#include "token.h"

using namespace std;

class Lexer
{
  public:

  Lexer(string fileName);
  vector<Token> stream;

  private:

  void addToBuffer(char c, int line, int pos, int curLineLen);
  //accepted string literal characters:any characters a...z and spaces
  vector<char> acceptedString = {
    'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l',
    'm', 'n', 'o', 'p', 'q', 'r',
    's', 't', 'u', 'v', 'w', 'x',
    'y', 'z',' '
  };

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

  bool inString = false; //for string expressions

  string buffer = "";


};

#endif //COMPILER_LEXER_H