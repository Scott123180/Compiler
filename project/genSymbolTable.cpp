#include <iostream>

#include "semantic.h"
#include "genSymbolTable.h"

GenSymbolTable::GenSymbolTable(Semantic s, bool v)
  : verbose(v)
{
  tokens = s.tokens;

  //get root tokens
  vector<Token*> rootTokens = s.newAST.treeRoots;

  for(vector<Token*>::size_type i = 0; i < rootTokens.size(); i++)
  {
    //initialize symbol table
    rootSymbolTable = new SymbolTable(nullptr, uniqueScope++);
    curSymbolTable = rootSymbolTable;

    //set curToken for produceST
    curToken = rootTokens[i];
    //call the function to complete the ST generation
    produceST(curToken);
    calcSymbolTableOutput(rootSymbolTable, verbose);

    //free up memory
    deleteAllST(rootSymbolTable);

  }

}

void GenSymbolTable::deleteAllST(SymbolTable *a)
{
  for(vector<SymbolTable*>::size_type i = 0; i < a->children.size(); i++)
  {
    //recursively call for all children
    deleteAllST(a->children[i]);
  }
  delete a;
}

//produce symbol table
void GenSymbolTable::produceST(Token* a)
{
  //switch to first child of program (block)
  curToken = curToken->children.front();

  //next token will be block
  blockST();
}

bool GenSymbolTable::blockST()
{
  if(curToken->getType() == "Block")
  {
    if(firstBlock)
    {
      //if it's the first block, don't create new symbol table
      firstBlock = false;
    }
    else //not first block
    {
      //create new symbol table
      SymbolTable* newScope = new SymbolTable(curSymbolTable, uniqueScope++);
      newScope->parent = curSymbolTable; //set parent of newScope
      curSymbolTable = newScope; //set current symbolTable to this
    }

    Token* blockBranch = curToken;
    //loop through children to try other types of statements
    for(vector<Token*>::size_type i = 0; i < blockBranch->children.size(); i++)
    {
      //switch to token
      curToken = blockBranch->children[i];
      //try different operations
      if(blockST());
      else if(varDeclST());
      else if(assignST());
      else if(ifST());
      else if(printST());
      else //should be unreachable, but throw an error if we reach it
      {
        vector<string> errorData = {curToken->getType()};
        Error newError = Error(true, Error::semantic,curToken->getLine(), curToken->getPos()
        , errorData, "You did the impossible and reached an unreachable state. Congrats! : ");
      }

    }

    kickST(); //move up in symbol table
    return true;
  }
  else //not a block token
  {
    return false;
  }

}

bool GenSymbolTable::varDeclST()
{
  if(curToken->getType() == "VarDecl")
  {
    string varType = curToken->children[0]->getType();
    string varName = curToken->children[1]->getData();
    //try to initialize variable
    StEntry variable = StEntry(varName, varType, curToken->getLine(),
                               curToken->getPos(),curSymbolTable->scope,true);
    return true;
  }
  else //not Variable Declaration
  {
    return false;
  }
}

bool GenSymbolTable::assignST()
{
  if(curToken->getType() == "AssignStatement")
  {
    //first child is always variable name
    string varName = curToken->children[0]->getData();
    //let exprST handle type checking of expression
    string exprType = exprST(curToken);
    //create template entry
    StEntry assignVar = StEntry(varName,exprType,curToken->children[0]->getLine(), 0,
      curSymbolTable->scope, false);

    //lookup variable to assign
    curSymbolTable->assignVarTable(assignVar);
  }
  else //not assign statement
  {
    return false;
  }
}

bool GenSymbolTable::ifST()
{
  if(curToken->getType() == "IfStatement")
  {
    //type check the boolean expression
      // -first child will always be the boolexpr branch
    boolExprST(curToken->children[0]);
    //handle the block
    curToken->children[1]; //switch curToken so block can reference
    blockST();
    return true;
  }
  else //not an if statement
  {
    return false;
  }
}

bool GenSymbolTable::printST()
{
  if(curToken->getType() == "PrintStatement")
  {
    //type check and utilize expression with first child
    exprST(curToken->children[0]);
    return true;
  }
  else //not a print statement
  {
    return false;
  }
}

string GenSymbolTable::exprST(Token* a)
{

}

string GenSymbolTable::boolExprST(Token* a)
{

}

string GenSymbolTable::intExprST()
{

}

string GenSymbolTable::stringExprST()
{

}

//calculate the output for the symbol table
void GenSymbolTable::calcSymbolTableOutput(SymbolTable* a, bool verbose) //depth-first in order
{

  unsigned int depth = a->calcTableDepth(a);

  //table html stuff
  string table;


  table.append("<table class =\"table\">\n");
  table.append("<tr>\n");

  table.append("<th>\n");
  table.append("Name");
  table.append("</th>\n");

  table.append("<th>\n");
  table.append("Type");
  table.append("</th>\n");

  table.append("<th>\n");
  table.append("Line Number");
  table.append("</th>\n");

  table.append("<th>\n");
  table.append("Scope");
  table.append("</th>\n");

  table.append("<th>\n");
  table.append("Scope Depth");
  table.append("</th>\n");

  table.append("<th>\n");
  table.append("Initialized");
  table.append("</th>\n");

  table.append("<th>\n");
  table.append("Utilized");
  table.append("</th>\n");

  table.append("</tr>\n");
  for(vector<StEntry>::size_type i = 0; i < a->rows.size(); i++)
  {
    table.append("<tr>\n");

    //NAME
    table.append("<td>\n");
    string charToString = "";
    charToString.push_back(a->rows[i].name);
    table.append(charToString);
    table.append("</td>\n");

    //TYPE
    table.append("<td>\n");
    table.append(a->rows[i].type);
    table.append("</td>\n");

    //LINE NUMBER
    table.append("<td>\n");
    table.append(to_string(a->rows[i].lineNum));
    table.append("</td>\n");

    //SCOPE
    table.append("<td>\n");
    table.append(to_string(a->rows[i].scope));
    table.append("</td>\n");

    //SCOPE DEPTH
    table.append("<td>\n");
    table.append(to_string(depth));
    table.append("</td>\n");

    string boolToString;

    //INITIALIZED
    table.append("<td>\n");
    if(a->rows[i].initialized) boolToString = "true";
    else boolToString = "false";
    table.append(boolToString);
    table.append("</td>\n");

    //UTILIZED
    table.append("<td>\n");
    if(a->rows[i].utilized) boolToString = "true";
    else boolToString = "false";
    table.append(boolToString);
    table.append("</td>\n");

    table.append("</tr>\n");
  }
  table.append("</table>");
  //end html

  //push output to string vector
  symbolTableOuput.push_back(table);

  //recursive call
  for (vector<SymbolTable>::size_type i = 0; i < a->children.size(); i++) {
    calcSymbolTableOutput(a->children[i], verbose);
  }
}

void GenSymbolTable::kickTok()
{
  if(curToken->parent) //make sure no nullptr
  {
    curToken = curToken->parent;
  }
}

void GenSymbolTable::kickST()
{
  if(curSymbolTable->parent) //make sure no nullptr
  {
    curSymbolTable = curSymbolTable->parent;
  }
}