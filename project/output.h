#ifndef COMPILER_OUTPUT_H
#define COMPILER_OUTPUT_H

#include <vector>
#include <string>

#include "token.h"
#include "lexer.h"
#include "symbolTable.h"

using namespace std;

class Output {
  private:
    void clearHTML();

    void clearCST();

    void printHTMLHeader();

    void printTitle();

    void printTokens(vector<Token> s);

    void printCST(vector<string> t);

    void printAST(vector<string> t);

    void printSymbolTable(SymbolTable* s);

    void printHTMLFooter();

    void printError();

  public:

    Output();

  void printOutput(vector<Token> stream, vector<string> treeCST, vector<string> treeAST,
    SymbolTable* rootSymbolTable)
  {
    clearHTML();
    //clearCST(); if decide to use js tree, implement
    printHTMLHeader();
    printTitle();
    printTokens(stream);
    printCST(treeCST);
    printAST(treeAST);
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