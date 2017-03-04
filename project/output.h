#ifndef COMPILER_OUTPUT_H
#define COMPILER_OUTPUT_H

#include <vector>

#include "token.h"
#include "lexer.h"

using namespace std;

class Output {
  private:
    static void clearHTML();

    static void printHTMLHeader();

    static void printTokens(vector<Token> s);

    static void printCST();

    static void printHTMLFooter();

    static void printError();

  public:
    static void printOutput(vector<Token> stream)
    {
      clearHTML();
      printHTMLHeader();
      printTokens(stream);
      //printCST();
      printHTMLFooter();
    }

    static void printErrors()
    {
      clearHTML();
      printHTMLHeader();
      printError();
      printHTMLFooter();
    }

};

#endif