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
    
    static void addChild(Token* t, bool changeToChild);

    //delete just the last child of current node
    static bool deleteLastChild();

    //delete the node
    static bool deleteNode(Token* a, bool recursive);

    //gets rid of specified tokens and all of their children
    static void deleteRecur(Token* a);
    //delete child
    //static bool deleteLastChild();
    //recursively delete all children
    static bool deleteAllChildren();


    static Token* curNode;
  private:

};
#endif