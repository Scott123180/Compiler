#include <string>
#include <vector>

//for debugging
#include <iostream>

#include "cst.h"
#include "token.h"

using namespace std;

CST::CST()
{
 // cout << "CST Constructor Called" << endl;
  //new root node
  Token* root = new Token("Program");
  root->parent = nullptr;
  curNode = root; //set current node
  rootToken = root; //set root token
  depth = 0; //initialize depth to root
}

CST::~CST()
{
  //cout << "CST Destructor Called" << endl;
  //delete all children of
  //deleteTreeChildren(rootToken);
  //delete(rootToken);
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

void CST::deleteTreeChildren(Token* a)
{
  if(!a->children.empty()) //prevent empty children problems
  {
    while(a->children.size() > 0)
    {
      //call on children
      deleteTreeChildren(a->children.back());

      Token* deleteThis = a->children.back();

      //get rid of reference and decrement size
      a->children.pop_back();

      //free memory
      deleteNode(deleteThis);
    }
  }

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
    cout << "ERROR in deleting child node of tree" << endl;
    if(a->parent)
    {
      cout << "Parent: " << a->parent->getType() << " " << a->parent->getData() << endl;
    }
    cout << "This token: " << a->getType() << " " << a->getData() << endl;

    return false;
  }
}

Token* CST::returnToRoot()
{
  //scale up tree
  while(curNode->parent) //check for root (nullptr parent)
  {
    curNode = curNode->parent;
  }
  return curNode;
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
