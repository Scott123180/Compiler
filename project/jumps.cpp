#include "jumps.h"

using namespace std;

//keep track of while jumps
string Jumps::addRow()
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
  string jumpTempId = "J" + to_string(jumpID);
  //push back a new row
  rows.push_back(jumpTempId);

  return jumpTempId;

}

int Jumps::calculateDistance(int start, int end)
{
  //calculate distance
  int distance;

  cout << 255 << "-" << end << endl;
  distance = 255 - end; //for jumping to end of memory, 255
  ++distance; //for jumping to 0
  cout << "+" << distance << endl;
  cout << distance << "+=" << start << "+" << 1 << endl;
  distance += (start); //for jumping to end from 00

  return distance;
}