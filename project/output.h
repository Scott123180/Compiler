#ifndef COMPILER_OUTPUT_H
#define COMPILER_OUTPUT_H

#include <vector>

#include "token.h"
#include "lexer.h"

using namespace std;

class Output {
  private:
    void clearHTML();

    void clearCST();

    void printHTMLHeader();

    void printTitle();

    void printTokens(vector<Token> s);

    void printCST();

    void printHTMLFooter();

    void printError();

  public:

    Output();

  void printOutput(vector<Token> stream)
  {
    clearHTML();
    printHTMLHeader();
    printTitle();
    printTokens(stream);
    //printCST();
    printHTMLFooter();
  }

  void printErrors()
    {
      clearHTML();
      printHTMLHeader();
      printError();
      printHTMLFooter();
    }

};

#endif //COMPILER_OUTPUT_H