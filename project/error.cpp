#include <iostream>
#include <string>

#include "error.h"

using namespace std;

Error::Error(bool q, int e, int l, int p, vector<string> d, string m)
  : quit(q), state(e), line(l), pos(p), data(d), message(m)
{
  switch(state) {
    case fileInput:
      cerr << message << data[0] << endl
           << "Aborting Compilation." << endl;
      break;

    case lex:
      cerr << "Lex Error at " << line << ":" << pos << endl
           << message << data[0] << endl
           << "Aborting Lex." << endl;
      break;

    case parse:
      cerr << "Parse error at " << line << ":" << pos << endl
           << "Expecting: ";
      for(auto i = 0; i < d.size(); i++)
      {
        if(i != 0 && (i == d.size() - 1)) //for formatting
        {
          cerr << "or " << d[i] << "." << endl;
        }
        else
        {
          cerr << d[i] << ", ";
        }
        
      }
      cerr << "Aborting Parse." << endl;
      break;

    case semantic:
      cerr << "I don't know how you got here, but thanks for coding for me." << endl;
      break;

    default:
      cerr << "Unknown error at " << line << ":" << pos << endl
           << message << data[0] << endl
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
