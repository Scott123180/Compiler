#ifndef COMPILER_ERROR_H
#define COMPILER_ERROR_H

#include <string>

class Error
{
  public:
    void Error::Error(errorStage e, int line, int pos, string message){};

  private:
    enum errorStage {fileInput, lex, parse, semantic, other};

};
#endif