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

    /*
    Token* firstToken;

    Token* lastToken;

    vector<bool> eqList;

    int eqListPlace = 0;

     */
    vector<string> tree;
  
    //keep track of how deep in the tree for printing out
    unsigned int depth;
    
    //constructor for CST
    CST();
    
    void addChild(Token *t, bool changeToChild, bool verbose);

    //delete the node
    bool deleteNode(Token *a);

    Token* returnToRoot();

    //depth-first in-order
    void dfio(Token* a, bool verbose); //depth-first in order

    /*
    //in-order traversal
    void io(Token*a, vector<bool> e);

    //call function on the rest of the tree
    void recurse();

    int checkChildrenVisited(Token* a)
    {

    }

    //use root token to calculate the first token
    void calcFirstTok();

    //use root token to calculate the last token
    void calcLastTok();

    //go to the deepest node without visiting
    void goDeep();

    string boolToEqSymbol(bool b);
     */
  
    void calcTokDepth(bool verbose);

  private:
    
};

#endif