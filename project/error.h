#ifndef COMPILER_ERROR_H
#define COMPILER_ERROR_H

#include <string>

using namespace std;

class Error
{

  private:
    bool quit;
    int state;
    int line, pos;
    string data, message;

  public:
    enum errorStage {fileInput, lex, parse, semantic, token};

    //create and assign error object
    Error(bool q, int e, int l, int p, string d, string m);

};
#endif