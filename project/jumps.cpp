#include "jumps.h"

using namespace std;

//returns id of jump
string Jumps::addRow()
{
  string dist = "00"; //initialize distance to 00
  unsigned long jumpID;
  if(rows.empty())
  {
    jumpID = 0;
  }
  else
  {
    jumpID = rows.size();
  }
  string jumpTempId = "J" + jumpID;
  //push back a new row
  rows.push_back(make_pair(jumpTempId, dist));

  return jumpTempId;

}

void Jumps::setDistance(string jump, int start, int end)
{
  //calculate distance
  int distance;

  //check for wraparound jump
  if (start <= end) //regular jump
  {
    distance = (end - start);
  } else //wraparound jump
  {
    distance = 255 - start; //for jumping to end of memory, 255
    ++distance; //for jumping to 0
    distance += ((end - 0) + 1); //for jumping to end from 00
  }

  //look through and find jump
  for (vector<pair<string, int>>::size_type i = 0; i < rows.size(); i++)
  {
    if (jump == rows[i].first) //if the same jump id
    {
      //get the hex value of a jump
      string distanceToHex = intToHex(distance);
      //set the hex value in the temporary location
      rows[i].second = distanceToHex;
    }
  }
}

string Jumps::lookupDistance(string tempName)
{
  //look through and find jump
  for (vector<pair<string, int>>::size_type i = 0; i < rows.size(); i++)
  {
    if (tempName == rows[i].first) //if the same jump id
    {
      //return the distance
      return rows[i].second;
    }
  }
}

//stolen from CodeGen
string Jumps::intToHex(int a)
{
  string hexValue;
  stringstream ss;
  ss << std::uppercase << std::hex << a;
  hexValue = ss.str();

  //add leading zero if only one size
  if(hexValue.size() == 1)
  {
    hexValue.push_back(' ');
    hexValue[1] = hexValue[0];
    hexValue[0] = '0';
  }
  cout << "HEX VALUE INT TO HEX: " << endl;
  return hexValue;
}