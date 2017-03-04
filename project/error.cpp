#include <iostream>
#include <string>

#include "error.h"

using namespace std;

Error::Error(bool q, int e, int l, int p, string d, string m)
  : quit(q), state(e), line(l), pos(p), data(d), message(m)
{
  switch(state) {
    case fileInput:
      cerr << message << data << endl
           << "Aborting Compilation." << endl;
      break;

    case lex:
      cerr << "Lex Error at " << line << ":" << pos << endl
           << message << data << endl
           << "Aborting Lex." << endl;
      break;

    case parse:

      break;

    case semantic:

      break;

    default:
      cerr << "Unknown error at " << line << ":" << pos << endl
           << message << data << endl
           << "Aborting compilation." << endl;
      break;
  }

  if(quit)
  {
    //exit program with unique error type code (shifted up one to avoid exit(0)
    int typeErr = state + 1;
    exit(typeErr);
  }
  else
  {
    //exit program either way, can change later if needed
    exit((99));
  }
}
