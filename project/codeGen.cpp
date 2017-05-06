#include "codeGen.h"
#include "error.h"

using namespace std;

CodeGen::CodeGen(CST ast, SymbolTable* st)
  : cgAST(ast), cgSymbolTable(st)
{
  cout << "CodeGen constructor called" << endl;
  //initialize output array to 00's
  for(int i = 0; i < 256; i++)
  {
    output[i] = "00";
  }

  //check to see if boolean hell flag is set in the symbol table
  if(st->booleanHell)
  {
    printBoolHell = true;
    vector<string> errorData = {};
    Error booleanHell = Error(false, Error::codeGen,0,0,errorData, "Nested bool detected. WHY?!");
  }
  else
  {
    //process the code
    process();

    //check for overflow after execution
    if(overFlow)
    {
      vector<string> errorData = {};
      Error booleanHell = Error(true, Error::codeGen,0,0,errorData,
      "Overflow error: program output code longer than 256 bytes.");
    }
  }
}

CodeGen::~CodeGen()
{
  cout << "CodeGen Destructor Called" << endl;
}

//start processing tokens
void CodeGen::process()
{
  cout << "got to process" << endl;
  //ensure we're at the root token
  cgAST.returnToRoot();

  cout << "before code segment return" << endl;

  //calculate and store the stack in a string vector
  code = segment(cgAST.rootToken);

  //precaution for no code
  if(code.empty()) //no code, so set code size to 0
  {
    codeSize = 0;
  }
  else //code, so set size to code vector
  {
    codeSize = code.size();
    cout << "<><><>code size: " << codeSize << endl;
  }
  cout << "before print code" << endl;
  //print code to output
  printCode();

  cout << "before overflow" << endl;
  checkForOverFlow();

  cout << "before backpatching" << endl;
  //implement back-patching
  backPatching();
  cout << "after backpatching" << endl;
}



//returns a segment of code and appends it to the segment that called it
  //information that is also passed (in vector size) is how many instructions
  //it contains(useful in jumps)
vector<string> CodeGen::segment(Token *a)
{
  //initialize the segment that we'll return
  vector<string> returnSegment = {};

  //check for leaf node
  if(!a->getData().empty()) //(not a branch node)
  {
    //check parent
    string parentType = a->parent->getType();

    //if the token we're looking at is the first child
    if(a->parent->children[0] == a)
    {
      if(parentType == "VarDecl")
      {
        returnSegment.push_back(LDA_C); //A9
        returnSegment.push_back(BRK); //00
        returnSegment.push_back(STA); //8D

        //fill with temp memory location
        string tempVar = sdTable.addRow(a);
        cout << "tempVar: "<< tempVar << endl;

        returnSegment.push_back(tempVar);
        returnSegment.push_back("XX");
      }
      else if(parentType == "AssignStatement")
      {
        //retrieve the temp variable information from staticData table

        //code for evaluating expression below
      }
      else if(parentType == "==" || parentType == "!=")
      {

      }
      else if(parentType == "PrintStatement")
      {

      }
      else //unreachable unless I forgot something
      {
        cout << "Please edit the code to include this type" << endl;
        cout << parentType << endl;
      }
    }
    else //token we're looking at is not the first child
    {
      //do nothing because we don't want to process the tree twice
    }

  }
  else //token is a branch node
  {
    //if we reach an if statement, then handle a jump and call segment on it's children
    if(a->getType() == "IfStatement")
    {
      //process conditional

      //run segment on ifBlock children
      vector<string> ifBlock = segment(a->children[1]);

      //Jump stuff, I don't think I really need it with my method
      unsigned int jumpDistance = static_cast<unsigned int>(ifBlock.size());
      //create new jump
      jTable.addRow(jumpDistance);

      //push back segment to return segment
      for(vector<string>::size_type i = 0; i < ifBlock.size(); i++)
      {
        returnSegment.push_back(ifBlock[i]);
      }
    }
  }

  //recursion
  for(vector<Token*>::size_type i = 0; i < a->children.size(); i++)
  {
    //store recursion results in string vector
    vector<string> recursionSegment = segment(a->children[i]);
    //push each string to the back of the vector, in order
    for(vector<string>::size_type j = 0; j < recursionSegment.size(); j++)
    {
      returnSegment.push_back(recursionSegment[j]);
    }
  }

  //return the segment (works for end of program)
  cout << "~~~return segment regular" << endl;
  return returnSegment;
}

//compare stack head and heap head and make sure there is not an overflow
void CodeGen::checkForOverFlow()
{
  if((stackSize + codeSize) > (heapHead)) //heapHead is an index and will always be at least 1
  {
    //stack overflow - throw error
    vector<string> errorData = {};
    Error stackOverflow = Error(true,Error::codeGen, 0, 0, errorData,
                                "Error In code generation. Stack overflow detected.");
  }
}



//print the code to output
void CodeGen::printCode()
{
  //loop through vector and replace output tokens
  for(vector<string>::size_type i = 0; i < codeSize; i++)
  {
    output[i] = code.at(i);
  }
}



//replace temporary variable and jump names with actual memory locations
void CodeGen::backPatching()
{
  cout << "before allocate memory" << endl;
  //set the actual memory addresses of the rows on the stack
  allocateMemoryOnStack();

  cout << "before replace memory" << endl;
  //find and replace the temporary memory addresses in the code
  replaceTemporaryMemoryAddresses();

  cout << "after replace memory" << endl;

  //handle jumps

}



//calculate stack size and set the staticDataRows actual memory addresses
void CodeGen::allocateMemoryOnStack()
{
  //find end of code and set the head of the stack to that
  int stackHead = static_cast<int>(codeSize);
  cout << "STACKHEAD: " << endl;
  string stackHeadHex = intToHex(stackHead);
  cout << "intToHex results: " << endl;

  //calculate size of stack (for use in overflows later)
    //precaution for no stack being used
  if(sdTable.data.empty()) //no stack used
  {
    stackSize = 0;
  }
  else //stack used, set to number of rows in data
  {
    stackSize = sdTable.data.size(); //each var takes up one byte of space
    cout << "------stack size: " << stackSize << endl;
  }

  //loop through every data row to set memory location on the stack
  for(vector<StaticDataRow>::size_type i = 0; i < stackSize; i++)
  {
    //set the data row's actual memory location
    sdTable.data[i].setActualMemoryAddress(stackHeadHex);

    //initialize memory to value of 0
    output[stackHead] = "00";

    //don't move the stackHead if we're at the last data row
    if(i != (sdTable.data.size() - 1))
    {
      ++stackHead; //move stackHead
      stackHeadHex = intToHex(stackHead); //new stackHead, new stackHeadHex
    }

  }
}



//find and replace temporary memory addresses in the code portion
void CodeGen::replaceTemporaryMemoryAddresses()
{
  int codeEnd = static_cast<int>(codeSize) - 1;
  //loop through every address
  for(int i = 0; i <= codeEnd; i++)
  {
    //check for temporary variable
    if(output[i].front() == 'T') //if first character of location is T (temporary)
    {
      //if so, replace with actual address
      string address = sdTable.lookupRow(output[i]); //lookup hex address on stack
      output[i] = address;
      ++i; //increment i to put in 00's
      output[i] = "00";
    }
    //just move on to next input if not found
  }
}

//convert a string to null terminated hex array
vector<string> CodeGen::stringToHexChars(string a)
{
  vector<string> hexVals = {};

  //convert each character to hex value
  for(string::size_type i = 0; i < a.size(); i++)
  {
    stringstream ss;
    ss << std::hex << (int)a[i];
    hexVals.push_back(ss.str());
  }

  //null terminator
  hexVals.push_back("00");
  return hexVals;
}

string CodeGen::intToHex(int a)
{
  cout << "INT A: " << endl;
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

int CodeGen::hexToInt(string hexValue)
{
  int n;
  istringstream(hexValue) >> std::hex >> n; //convert
  return n;
}