#include <string>
#include <vector>

#include "cst.h"
#include "token.h"

using namespace std;

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
  CST::curNode->children.push_back(t);
  
  //change curNode reference to child node
  CST::curNode = t;  //change the current node to the pointer (child)
}

//delete child
static bool deleteLastChild()
{
  //if it has children, don't allow
  if(CST::curNode->children.empty())
  {
    return false;
  }
  else
  {
    unsigned long len = CST::curNode->children.size(); //size of vector
    len--; //number of last element
    Token* deleteThis = CST::curNode->children.at(len);
    if(deleteNode(deleteThis))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}

static bool deleteNode(Token* a)
{
  //if it has children, don't allow
  if(a->children.empty())
  {
    return false;
  }
  else
  {
    delete a;
    return true;
  }
}
static void deleteRecur(Token* a)
{
  while(!(a->children.empty())) //has children
  {
    unsigned long len = CST::curNode->children.size(); //size of vector
    len--; //number of last element
    deleteRecur(a->children[len]);
  }
  //after get to a node with no children, delete it
  deleteNode(a);
}
//recursively delete all children
static bool deleteAllChildren()
{
  //starts at the current node
  while(!CST::curNode->children.empty())
  {

  }
}

/*
TODO: methods to add nodes and delete nodes
 -to add node, we need pointer to current parent node and to add it to the end
 of it's list of child nodes

 -to delete node, we need pointer to current node, and delete and delete / pop off of end
 and adjust other children accordingly

 -to delete children nodes, we need to recursively delete the pointers each of the children

 */