#ifndef COMPILER_OUTPUT_H
#define COMPILER_OUTPUT_H

#include <vector>
#include <string>

#include "token.h"
#include "lexer.h"
#include "semantic.h"
#include "parser.h"
#include "symbolTable.h"

using namespace std;

class Output {
  private:
    void clearHTML();

    void clearCST();

    void printHTMLHeader();

    void printTitle();

    void printTokens();

    void printCST();

    void printAST();

    void printSymbolTable();

    void printHTMLFooter();

    void printError();

    vector<Token> stream;

    vector<string> treeCST;

    vector<string> treeAST;

    vector<string> treeST;

  public:

    Output(Lexer l, Parser p, Semantic s);

  void printOutput()
  {
    clearHTML();
    //clearCST(); if decide to use js tree, implement
    printHTMLHeader();
    printTitle();
    printTokens();
    printCST();
    printAST();
    printSymbolTable();
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