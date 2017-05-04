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
    //constructor for CST
    CST();
    ~CST();

    Token* curNode;
  
    Token* calcDepth;

    Token* rootToken;

    vector<string> tree;
  
    //keep track of how deep in the tree for printing out
    unsigned int depth;
    
    void addChild(Token *t, bool changeToChild, bool verbose);

    void deleteTreeChildren(Token* a);

    //delete the node
    bool deleteNode(Token *a);

    Token* returnToRoot();

    //depth-first in-order
    void dfio(Token* a, bool verbose); //depth-first in order
  
    void calcTokDepth(bool verbose);


  private:
    
};

#endif