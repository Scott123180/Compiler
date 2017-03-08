#ifndef COMPILER_CST_H
#define COMPILER_CST_H

/*
 * holds methods for accessing and manipulating a token's children/parent
 */

#include <string>

#include "token.h"

using namespace std;

class CST
{
  public:
    //construct new CST
    CST();
    
    void addChild(Token* t);
    
    Token* getcurNode() { return curNode; }
    Token* upTree(Token* parentNode);
    
    //delete child
    void deleteChild();
    //recursively delete all children
    void deleteAllChildren();
    
    //return the parent node
    void parent();
    
  private:
    Token* curNode = nullptr;
};
#endif