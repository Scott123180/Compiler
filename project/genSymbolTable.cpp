#include <iostream>

#include "semantic.h"
#include "genSymbolTable.h"

GenSymbolTable::GenSymbolTable(Semantic s, bool v)
  : verbose(v)
{
  tokens = s.tokens;

  //initialize symbol table
  rootSymbolTable = new SymbolTable(nullptr, uniqueScope++);
  curSymbolTable = rootSymbolTable;

  //get root tokens
  vector<Token*> rootTokens = s.newAST.treeRoots;

  for(vector<Token*>::size_type i = 0; i < rootTokens.size(); i++)
  {
    //set curToken for produceST
    curToken = rootTokens[i];
    //call the function to complete the ST generation
    produceST();
  }

}

//produce symbol table
void GenSymbolTable::produceST()
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