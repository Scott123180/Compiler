#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include <vector>

#include "Token.h"

class Parser
{

public:
  //parser object
  Parser(vector<Token> stream);
private:
  int i = 0; //points to which token we're on
  vector<Token> tokens;

};

#endif //COMPILER_PARSER_H