#ifndef COMPILER_ERROR_H
#define COMPILER_ERROR_H

#include <string>

class Error
{

  private:
  public:
    static void genError(errorStage e, int line, int pos, string data, string message);

    static enum errorStage {fileInput, lex, parse, semantic};


};
#endif