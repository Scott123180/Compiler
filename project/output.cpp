#include <string>
#include <fstream>
#include <vector>

#include "token.h"
#include "lexer.h"
#include "output.h"
#include "parser.h"
#include "cst.h"

using namespace std;

Output::Output(Lexer l, Parser p, Semantic s)
{
  //set print variables
  stream = l.stream;

  treeCST = p.newCST.tree;

  treeAST = s.newAST.tree;

  //treeST = s.symbolTableOuput;
}

void Output::clearHTML()
{
  //filestream
  ofstream outputHTML;
  outputHTML.open("output.html", ios::out | ios::trunc); //open and clear file
  outputHTML.close();
  return;
}

void Output::clearCST()
{
  ofstream cstJS;
  cstJS.open("js/cst.js", ios::out | ios::trunc); //open and clear file
  cstJS.close();
  return;
}

void Output::printHTMLHeader()
{
  //filestream
  ofstream outputHTML;
  outputHTML.open("output.html", std::ios_base::app); //open and append to file

  //html header
  outputHTML << "<!DOCTYPE html> \n"
              "<html>\n"
              "<head>\n"
              "<title>Lex and Parsing Output</title>\n"
              "<link rel=\"stylesheet\" type=\"text/css\" href=\"bootstrap/css/bootstrap.css\">"
              "<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js\"></script>\n"
              //"<script src=\"js/pic1.js\"></script> \n"
              //"<script src=\"js/pic2.js\"></script> \n"
              //"<script src=\"js/pic3.js\"></script> \n"
              "</head>\n"
              "<body>\n";
  outputHTML.close();


  return;
}

void Output::printTitle()
{
  ofstream outputHTML;
  outputHTML.open("output.html", std::ios_base::app); //open and append to file

  outputHTML << "<div class=\"page-header\"> \n"
                "<h1>Scott's Dank Compiler <small>Scott Hansen</small></h1>\n"
                "</div>\n"

    /*
                //random pictures
                "<div class=\"row\"> \n"
                "<div class=\"col-xs-4 col-sm-4 col-md-4 col-lg-4\"> \n"
                "<a href=\"#\" class=\"thumbnail\"> \n"
                "<img src=\"...\" alt=\"...\" id=\"pic1\" style=\"{width:200px; height:200px;}\"> \n"
                "</a> \n"
                "</div>\n"

                "<div class=\"col-xs-4 col-sm-4 col-md-4 col-lg-4\"> \n"
                "<a href=\"#\" class=\"thumbnail\"> \n"
                "<img src=\"...\" alt=\"...\" id=\"pic2\" style=\"{width:200px; height:200px;}\"> \n"
                "</a> \n"
                "</div>\n"

                "<div class=\"col-xs-4 col-sm-4 col-md-4 col-lg-4\"> \n"
                "<a href=\"#\" class=\"thumbnail\"> \n"
                "<img src=\"...\" alt=\"...\" id=\"pic3\ style=\"{width:200px; height:200px;}\"> \n"
                "</a> \n"
                "</div>\n"
*/

                "</div> \n";

  outputHTML.close();
  return;
}

void Output::printTokens() {

  //filestream
  ofstream outputHTML;
  outputHTML.open("output.html", std::ios_base::app); //open and append to file

  //table
  outputHTML << "<div class=\"panel panel-default col-xs-6 col-sm-6 col-md-6 col-lg-6\">\n"
                "<div class=\"panel-heading\">\n"
                "<h3 class=\"panel-title\">Token List</h3>\n"
                "</div>\n"
                "<div class=\"panel-body\">\n"
                "<table class = \"table col-xs-6 col-sm-6 col-md-6 col-lg-6\">\n"
                "<tr>\n"
                "<th>Line</th>\n"
                "<th>Type</th>\n"
                "<th>Data</th>\n"
                "</tr>\n";

  //table data
  for (int i = 0; i < stream.size(); i++) {
    string blank = ""; //gotta love that left association
    outputHTML << blank << "<tr>\n"
               << "<th>" << stream[i].getLine() << "</th>\n"
               << "<th>" << "T_" << stream[i].getType() << "</th>\n"
               << "<th style=\"white-space: pre;\">" << stream[i].getData()
               << "</th>\n" //preserve white space with "pre"
               << "</tr>\n";
  }
  outputHTML << "</table>\n"
             << "</div>\n"
             << "</div>\n";

  outputHTML.close();

  return;
}

void Output::printCST()
{
  /*
  modify the cst javascript
  ofstream cstJS;
  cstJS.open("js/cst.js", std::ios_base::app); //open and append
  TODO: code for CST tree
  cstJS.close();
  */

  //modify the output html
  ofstream outputHTML;
  outputHTML.open("output.html", std::ios_base::app); //open and append to file
  
  outputHTML <<"<div class=\"panel panel-default col-xs-6 col-sm-6 col-md-6 col-lg-6\">\n"
    "<div class=\"panel-heading\">\n"
    "<h3 class=\"panel-title\">CST</h3>\n"
    "</div>\n"
    "<div class=\"panel-body\">\n";
  
  for (vector<string>::size_type a = 0; a < treeCST.size(); a++)
  {
    outputHTML << treeCST[a] << "<br>\n";
  }
  outputHTML << "</div>\n"
             "</div>";
  outputHTML.close();

  return;
}

void Output::printAST()
{
  /*
  modify the AST javascript
  ofstream cstJS;
  cstJS.open("js/cst.js", std::ios_base::app); //open and append
  TODO: code for CST tree
  cstJS.close();
  */

  //modify the output html
  ofstream outputHTML;
  outputHTML.open("output.html", std::ios_base::app); //open and append to file

  outputHTML <<"<div class=\"panel panel-default col-xs-6 col-sm-6 col-md-6 col-lg-6\">\n"
    "<div class=\"panel-heading\">\n"
    "<h3 class=\"panel-title\">AST</h3>\n"
    "</div>\n"
    "<div class=\"panel-body\">\n";

  for (vector<string>::size_type a = 0; a < treeAST.size(); a++)
  {
    outputHTML << treeAST[a] << "<br>\n";
  }
  outputHTML << "</div>\n"
             "</div>";
  outputHTML.close();

  return;
}

void Output::printSymbolTable()
{
  ofstream outputHTML;
  outputHTML.open("output.html", std::ios_base::app); //open and append to file

  outputHTML <<"<div class=\"panel panel-default col-xs-6 col-sm-6 col-md-6 col-lg-6\">\n"
      "<div class=\"panel-heading\">\n"
      "<h3 class=\"panel-title\">Symbol Table</h3>\n"
      "</div>\n"
      "<div class=\"panel-body\">\n";

  for(vector<string>::size_type i = 0; i < treeST.size(); i++) //loop through tables
  {
    outputHTML << treeST[i] << "\n";
  }

  outputHTML.close();
}

void Output::printError()
{
  //filestream
  ofstream outputHTML;
  outputHTML.open("output.html", std::ios_base::app); //open and append to file

  outputHTML.close();
  return;
}

void Output::printHTMLFooter()
{
  //filestream
  ofstream outputHTML;
  outputHTML.open("output.html", std::ios_base::app); //open and append to file

  outputHTML << "</body>\n"
                "<footer></footer> \n"
                "</html>\n";

  outputHTML.close();
  return;
}