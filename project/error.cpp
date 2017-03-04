#include <iostream>
#include <string>

#include "error.h"

using namespace std;

void Error::genError(errorStage e, int line, int pos,string data, string message)
{
  string errorStmt;

  switch(errorStage) {
    case fileInput:
      cerr << message << data << endl;
      break;

    case lex:

      break;

    case parse:

      break;

    case semantic:

      break;

    default:

      break;
  }

  //exit program with unique error type code (shifted up one to avoid exit(0)
  exit((errorStage+1));

}
