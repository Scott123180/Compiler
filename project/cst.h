#ifndef COMPILER_CST_H
#define COMPILER_CST_H

/*
 * holds methods for accessing and manipulating a token's children/parent
 */

#include <string>
#include <vector>

#include "token.h"

using namespace std;

class CST
{
  public:
  
    Token* curNode;
  
    Token* calcDepth;

    Token* rootToken;
  
    vector<string> tree;
  
    //keep track of how deep in the tree for printing out
    unsigned int depth;
    
    //constructor for CST
    CST();
    
    void addChild(Token *t, bool changeToChild, bool verbose);

    //delete the node
    bool deleteNode(Token *a);

    Token* returnToRoot();
  
    void dfio(Token* a, bool verbose); //depth-first in order
  
    void calcTokDepth(bool verbose);

  private:
    
};

#endif