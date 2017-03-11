#include <string>
#include <vector>

//for debugging
#include <iostream>

#include "cst.h"
#include "token.h"

using namespace std;

CST::CST()
{
  //new root node
  Token* root = new Token("Program");
  root->parent = nullptr;
  curNode = root;
  depth = 0; //initialize depth to root
}

void CST::addChild(Token* t, bool changeToChild)
{
  //add reference to parent's children
  curNode->children.push_back(t);
  
  //set the parent of the token
  t->parent = curNode;
  
  if(changeToChild) //good for nonterminals
  {
    //change curNode reference to child node
    curNode = t;  //change the current node to the pointer (child)
  }
  return;
}

bool CST::deleteNode(Token *a)
{
  //if it has children, don't allow
  if(a->children.empty())
  {
    //get rid of reference in parent to avoid null ptr
    a->parent->children.pop_back();
    delete a;
    return true;
  }
  else
  {
    return false;
  }
}

void CST::returnToRoot()
{
  //scale up tree
  while(curNode->parent != nullptr)
  {
    curNode = curNode->parent;
  }
}

string CST::dfio(Token* a) //depth-first in order
{

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
  CST::deleteNode(a);
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