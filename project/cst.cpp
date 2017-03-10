#include <string>
#include <vector>

#include "cst.h"
#include "token.h"

using namespace std;

CST::curNode = nullptr;

CST::CST()
{
  //new root node
  Token* root = new Token("Program");
  root->parent = nullptr;
  CST::curNode = root; //ATTN: may be wrong way to set root
}

void CST::addChild(Token* t, bool changeToChild)
{
  //add reference to parent's children
  curNode->children.push_back(t);

  if(changeToChild) //good for nonterminals
  {
    //change curNode reference to child node
    CST::curNode = t;  //change the current node to the pointer (child)
  }
}


//delete child
bool CST::deleteLastChild()
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
    if(CST::deleteNode(CST::curNode->children.at(len), false))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}

bool CST::deleteNode(Token* a, bool recursive)
{
  //if it has children, don't allow
  if(a->children.empty())
  {
    if(recursive)
    {
      CST::deleteRecur(a);
    }
    else
    {
      return false;
    }
  }
  else
  {
    delete a;
    return true;
  }
}
void CST::deleteRecur(Token* a)
{
  while(!(a->children.empty())) //has children
  {
    unsigned long len = CST::curNode->children.size(); //size of vector
    len--; //number of last element
    CST::deleteRecur(a->children[len]);
  }
  
  CST::curNode = CST::curNode->parent; //change current node to parent
  //TODO: dereference node in parent
  //after get to a node with no children, delete it
  CST::deleteNode(a, false);
}
//recursively delete all children
bool CST::deleteAllChildren()
{

  //starts at the current node
  while(!CST::curNode->children.empty())
  {
    //keep on deleting the first child until none left
    CST::deleteRecur(CST::curNode->children.at(0));
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