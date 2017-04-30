#ifndef COMPILER_OUTPUT_H
#define COMPILER_OUTPUT_H

#include <vector>
#include <string>

#include "token.h"
#include "lexer.h"
#include "semantic.h"
#include "parser.h"
#include "symbolTable.h"
#include "codeGen.h"

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

    void printCodeGen();

    void printHTMLFooter();

    void printError();

    vector<Token> stream;

    vector<string> treeCST;

    vector<string> treeAST;

    vector<string> treeST;

    string codeGen[256];

  public:

    Output(Lexer l, Parser p, Semantic s, CodeGen c);

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
      printCodeGen();
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