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
      cerr << "Found: " << message << endl;
      cerr << "Aborting Parse." << endl;
      break;

    case semantic:

      if(quit) //fatal error (not a warning)
      {
        cerr << "Semantic error: " << line << ":" << pos << endl;
        cerr << message << endl;
        for(vector<string>::size_type i = 0; i < data.size(); i++)
        {
          cerr << data[i] << endl;
        }
        cerr << "Aborting Compilation" << endl;
      }
      else //just a warning
      {
        cerr << "Semantic warning: " << line << ":" << pos << endl;
        cerr << message << endl;
        for(vector<string>::size_type i = 0; i < data.size(); i++)
        {
          cerr << data[i] << endl;
        }
      }

      break;
    case token:
      cerr << "Error at " << line << ":" << pos << endl
           << message << "\"" << data[0] << "\"" << endl
           << "Aborting Lex." << endl;
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
}
