#include "jumps.h"

using namespace std;

void Jumps::addRow(unsigned int temp, unsigned int dist)
{
  //push back a new row
  rows.push_back(make_pair(temp, dist));
}