#include <string>
#include <vector>

#include "cst.h"
#include "token.h"

CST::CST()
{
  //new root node
  Token* root = new Token("Program");
  root->parent = nullptr;
  CST::curNode = root; //ATTN: may be wrong way to set root
}

void addChild(Token* t)
{
  //add reference to parent's children
  curNode->children.push_back(t);
  
  //change curNode reference to child node
}

//returns the current token
void curNode();

//delete child
void deleteChild();
//recursively delete all children
void deleteAllChildren();

//return the parent node
void parent();

/*
TODO: methods to add nodes and delete nodes
 -to add node, we need pointer to current parent node and to add it to the end
 of it's list of child nodes

 -to delete node, we need pointer to current node, and delete and delete / pop off of end
 and adjust other children accordingly

 -to delete children nodes, we need to recursively delete the pointers each of the children

 */