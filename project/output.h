#ifndef COMPILER_OUTPUT_H
#define COMPILER_OUTPUT_H

#include <vector>
#include <string>

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

    void printCST(vector<string> t);

    void printHTMLFooter();

    void printError();

  public:

    Output();

  void printOutput(vector<Token> stream, vector<string> tree)
  {
    clearHTML();
    //clearCST(); if decide to use js tree, implement
    printHTMLHeader();
    printTitle();
    printTokens(stream);
    printCST(tree);
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