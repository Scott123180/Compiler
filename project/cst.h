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
  
    Token* curNode;
    
    //construct new CST
    CST();
    
    void addChild(Token* t, bool changeToChild);

    //delete just the last child of current node
    bool deleteLastChild();

    //delete the node
    bool deleteNode(Token* a, bool recursive);

    //gets rid of specified tokens and all of their children
    void deleteRecur(Token* a);
    //delete child
    //static bool deleteLastChild();
    //recursively delete all children
    bool deleteAllChildren();
  
  private:
    
};

#endif