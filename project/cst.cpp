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
  if(t->getData() == "")
  {
    cout << "trying: " << t->getType() << endl;
  }
  else
  {
    cout << "trying: " << t->getData() << endl;
  }
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
  while(curNode->parent) //check for root (nullptr parent)
  {
    curNode = curNode->parent;
  }
}

void CST::dfio(Token* a) //depth-first in order
{
  //1 calculate current token dashes
  string dashes = "";
  for(unsigned int d = 0; d < depth; d++)
  {
    dashes.append("-");
  }
  
  //2 current token info
  if(a->getData() == "")  //branch non-terminal
  {
    //3 print branch-nonterminal
    cout << dashes << a->getType() << endl;
    
    //4 depth calculations
    //check for special epsilon terminal
    if(!(a->children.empty())) //not special epsilon (has children)
    {
      ++depth;
    }
    //recursive call
    for(auto i = 0; i < a->children.size(); i++)
    {
      //check for last child
      if((i+1) == a->children.size()) //last node
      {
        dfio(a->children[i]);
        --depth;
      }
      else
      {
        dfio(a->children[i]);
      }
      
    }
  }
  else //leaf terminal
  {
    cout << dashes << a->getType() << endl;
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