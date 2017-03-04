#include <string>
#include <fstream>
#include <vector>

#include "token.h"
#include "lexer.h"

using namespace std;

void clearHTML()
{
  //filestream
  ofstream outputHTML;
  outputHTML.open("output.html", ios::out | ios::trunc); //open and clear file
  outputHTML.close();
  return;
}

void printHTMLHeader()
{
  //filestream
  ofstream outputHTML;
  outputHTML.open("output.html", std::ios_base::app); //open and append to file

  //html header
  outputHTML << "<!DOCTYPE html> \n"
             << "<html>\n"
             << "<head>\n"
             << "<title>Lex Output</title>\n"
             << "<link rel=\"stylesheet\" type=\"text/css\" href=\"bootstrap/css/bootstrap.css\">"
             << "</head>\n"
             << "<body>\n";
  outputHTML.close();
  return;
}

void printTokens(vector<Token> stream) {

  //filestream
  ofstream outputHTML;
  outputHTML.open("output.html", std::ios_base::app); //open and append to file

  //table
  outputHTML << "<table class = \"table col-xs-6 col-sm-6 col-md-6 col-lg-6\">\n"
             << "<tr>\n"
             << "<th>Line</th>\n"
             << "<th>Type</th>\n"
             << "<th>Data</th>\n"
             << "</tr>\n";

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
  //html footer
  outputHTML << "</table>\n"
             << "</body>\n"
             << "</html>";

  outputHTML.close();

  return;
}

void printCST()
{
  //filestream
  ofstream outputHTML;
  outputHTML.open("output.html", std::ios_base::app); //open and append to file


  outputHTML.close();
  return;
}

void printError()
{
  //filestream
  ofstream outputHTML;
  outputHTML.open("output.html", std::ios_base::app); //open and append to file

  outputHTML.close();
  return;
}

void printHTMLFooter()
{
  //filestream
  ofstream outputHTML;
  outputHTML.open("output.html", std::ios_base::app); //open and append to file

  outputHTML.close();
  return;
}