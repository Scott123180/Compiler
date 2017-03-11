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
  
    //keep track of how deep in the tree for printing out
    unsigned int depth;
    
    //constructor for CST
    CST();
    
    void addChild(Token* t, bool changeToChild);

    //delete the node
    bool deleteNode(Token *a);

    void returnToRoot();
  
    void dfio(Token* a); //depth-first in order
  
  private:
    
};

#endif