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
  cout << "trying: " << t->getType() << endl;
  //add reference to parent's children
  curNode->children.push_back(t);
  
  //set the parent of the token
  t->parent = curNode;
  
  if(t->parent == curNode->parent)
  {
    cout << "we done f*d up"<< endl;
  }
  
  if(changeToChild) //good for nonterminals
  {
    //change curNode reference to child node
    curNode = t;  //change the current node to the pointer (child)
    cout << "we switched" << endl;
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
  //cout << "made it to return root" << endl;
  //scale up tree
  while(curNode->parent->getType() != "Program") //check for root
  {
    //cout << curNode->getType() <<endl;
    //cout << curNode->parent->getType() <<endl;
    curNode = curNode->parent;
  }
}

void CST::dfio(Token* a) //depth-first in order
{
  cout << "made it to dfio"<< endl;
  for(unsigned int i = 0; i < a->children.size(); i++)
  {
    cout << "child " << i << endl;
    dfio(a->children[i]);
  }
  cout << a->getType() << endl;
}

/*
TODO: methods to add nodes and delete nodes
 -to add node, we need pointer to current parent node and to add it to the end
 of it's list of child nodes

 -to delete node, we need pointer to current node, and delete and delete / pop off of end
 and adjust other children accordingly

 -to delete children nodes, we need to recursively delete the pointers each of the children

 */