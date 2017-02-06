#include <iostream>
#include <vector>
#include <string>

using namespace std;

token genToken(string type, string data, int line, int position);

bool addToBuffer(char c);

string buffer;

int main()
{
  bool eof = false; //end of file

  while(!eof) //loop until end of file
  {
    char curLine;
    getline(cin, curLine); 
   
     
    //loop through string
    for(int i = 0; i < curLine.length(); i++)
    {
      //add to buffer to recognize regex patterns 

      //craft token from buffer

      //clear buffer
    } 
  }
  return 0;
}
