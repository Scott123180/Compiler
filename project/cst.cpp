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
}

void CST::addChild(Token* t, bool changeToChild)
{
  //add reference to parent's children
  curNode->children.push_back(t);

  if(changeToChild) //good for nonterminals
  {
    //change curNode reference to child node
    curNode = t;  //change the current node to the pointer (child)
  }
  return;
}


//delete child
bool CST::deleteLastChild()
{
  //if it has children, don't allow
  if(curNode->children.empty())
  {
    return false;
  }
  else
  {
    unsigned long len = CST::curNode->children.size(); //size of vector
    len--; //number of last element
    if(CST::deleteNode(CST::curNode->children.at(len)))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}

bool CST::deleteNode(Token *a)
{
  cout << "enter function " << endl;
  //if it has children, don't allow
  if(a->children.empty())
  {
  cout << "in the if" << endl;
    cout << "did we make it?" << endl;
    delete a;
    cout << "can't do this" << endl;
    return true;
  }
  else
  {
    return false;
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