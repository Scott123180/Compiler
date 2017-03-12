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

void CST::addChild(Token *t, bool changeToChild, bool verbose)
{
  if(verbose)
  {
    if(t->getData() == "")
    {
      cout << "trying: " << t->getType() << endl;
    }
    else
    {
      cout << "trying: " << t->getData() << endl;
    }
  }

  //add reference to parent's children
  curNode->children.push_back(t);
  
  //set the parent of the token
  t->parent = curNode;
  
  
  if(changeToChild) //good for nonterminals
  {
    if(verbose)
    {
      cout << "changing to child" << endl;
    }
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
    //safely remove reference from parent
    a->parent->children.erase(std::remove(a->parent->children.begin(),
                                          a->parent->children.end(),
                                          a), a->parent->children.end());
    
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

void CST::dfio(Token* a, bool verbose) //depth-first in order
{
  //set depth node to curNode
  calcDepth = a;
  //calculate depth
  calcTokDepth(verbose);
  if(verbose)
  {
    cout << depth << endl;
  }
  //calculate current token dashes
  string dashes = "";
  for(unsigned int d = 0; d < depth; d++)
  {
    dashes.append("-");
  }
  //reset depth
  depth = 0;
  
  //2 current token info
  if(a->getData() == "")  //branch non-terminal
  {
    //3 print branch-nonterminal
    if(verbose)
    {
      cout << dashes << "<"<< a->getType() << ">" << endl;
    }
    //keep track of data in object
    string branch = dashes + "&lt" + a->getType() + "&gt";//&lt and &gt to escape html tags in output
    tree.push_back(branch);
    
    //recursive call
    for (auto i = 0; i < a->children.size(); i++) {
      dfio(a->children[i], verbose);
    }
  }
  else //leaf terminal
  {
    if (verbose)
    {
      cout << dashes << "[" << a->getData() << "]" << endl;
    }
    //keep track of data in object
    string branch = dashes + "[" + a->getData() + "]";
    tree.push_back(branch);
  }
}

void CST::calcTokDepth(bool verbose)  //verbose
{
  //scale up tree
  while(calcDepth->parent) //check for root (nullptr parent)
  {
    ++depth;
    if(verbose)
    {
      cout << calcDepth->getType() << endl;
    }
    calcDepth = calcDepth->parent;
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