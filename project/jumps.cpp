#include "jumps.h"

using namespace std;

//returns id of jump
string Jumps::addRow(unsigned int dist)
{
  unsigned long jumpID;
  if(rows.empty())
  {
    jumpID = 0;
  }
  else
  {
    jumpID = rows.size();
  }
  //push back a new row
  rows.push_back(make_pair(jumpID, dist));

  string jumpReturn = "J" + to_string(jumpID);

}