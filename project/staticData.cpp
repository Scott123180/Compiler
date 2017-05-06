#include "staticData.h"
#include "staticDataRow.h"

using namespace std;

//return temporary memory value of added row
  //only called in varDecl
string StaticData::addRow(Token* a)
{
  int numRows;
  if(data.empty()) //define empty data vector behavior
  {
    numRows = 0;
  }
  else
  {
    //get the next temp var number
    numRows = static_cast<int>(data.size());
  }

  string temp = "T" + to_string(numRows); //temp var name

  //get varname
  string var = a->getData();

  //get unique scope
  int scope = a->scope;

  int offset = numRows;

  StaticDataRow newSDRow = StaticDataRow(temp, var, scope, offset);

  //push back row to staticData
  data.push_back(newSDRow);

  return temp;
}

//return the actual memory address on stack of a given variable
string StaticData::lookupRow(string tempName)
{
  for(vector<StaticDataRow>::size_type i = 0; i < data.size(); i++)
  {
    //same variable name and unique scope means they are the same variable
    if(tempName == data[i].getTemporary())
    {
      //return the temporary name of that variable
      return data[i].getActualMemoryAddress();
    }
  }

  //this won't ever happen but we have to return something or compiler will complain
  return "\0";

}

