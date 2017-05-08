#include "codeGen.h"
#include "error.h"

using namespace std;

CodeGen::CodeGen(CST* ast, GenSymbolTable* st)
  : cgAST(ast), cgSymbolTable(st->rootSymbolTable), cgGenSymbolTable(st)
{
  cout << "CodeGen constructor called" << endl;
  //initialize output array to 00's
  for(int i = 0; i < 256; i++)
  {
    output[i] = "00";
  }

  //check to see if boolean hell flag is set in the symbol table
  if(cgSymbolTable->booleanHell)
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
  cgAST->returnToRoot();

  cout << "before code segment return" << endl;

  //calculate and store the stack in a string vector
  code = segment(cgAST->rootToken);

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
        cout << "`````````WTF are we ADDING" << a->getData() << " " << a->getType() << endl;
        string tempVar = sdTable.addRow(a->parent->children[1]);
        cout << "tempVar: "<< tempVar << endl;

        returnSegment.push_back(tempVar);
        returnSegment.push_back("XX");
      }
      else if(parentType == "AssignStatement")
      {
        cout << "!!!!!!!! Got to ass statment" << endl;
        cout << "is this valid " << a->parent->children[1]->getData() << endl;

        cout << "memory location of 'a' token: " << a << endl;

        //assignment expression segment, pass right side(expr) and current token temp
        cout << "scope value: " << a->scope << endl;
        string tempMemLocation = sdTable.lookupTempRow(a);
        cout << tempMemLocation << endl;
        returnSegment = assignExpressionSegment(a->parent->children[1], tempMemLocation);
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
      //TODO: process conditional

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
    else if(a->getType() == "WhileStatement")
    {
      //TODO: process while statement
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
  return returnSegment;
}

//use symbol table to get the variable type of particular variable
string CodeGen::getVariableType(Token *a)
{
  string type;
  //get type from symbolTable
  return cgGenSymbolTable->returnType(a);
}

//process the code for an expression and return it
  //we need to know this to re-parse this section and generate code for it
  //this code is used in assigning variables
vector<string> CodeGen::assignExpressionSegment(Token* a, string tempVarName)
{
  cout << "I think we assignin" << endl;
  //determine what kind of expression
  string expressionType = "\0"; //initialize

  //check the type if it's a char
  if(a->getType() == "char")
  {
    expressionType = getVariableType(a);
  }

  string td = a->getData(); //token Data
  string tt = a->getType(); //token Type

  cout << "after the a->getdata and type thign" << endl;
  cout << "token data" << td << endl;
  cout << "token type" << tt << endl;
  cout << "expressionType" << expressionType << endl;

  vector<string> returnSegment;
  //check for boolean expression
  if(tt == "!=" || tt == "==" || td == "true" || td == "false" || expressionType == "boolean")
  {
    returnSegment = assignBooleanExpressionSegment(a, tempVarName);
  }
  //check for int expression
  else if(tt == "+" || tt == "int" || expressionType == "int")
  {
    cout << "WE checkin for expressin intexpr" << endl;
    cout << tempVarName << endl;
    returnSegment = assignIntExpressionSegment(a, tempVarName);
  }
  //check for string expression
  else if(tt == "string" || expressionType == "string")
  {
    returnSegment = assignStringExpressionSegment(a, tempVarName);
  }

  return returnSegment;
}



//recurse through leaves and perform operations
vector<string> CodeGen::assignIntExpressionSegment(Token* a, string tempVarName)
{
  cout << "in the assignintexpr segment" << endl;
  vector<string> returnIntSegment;
  cout << "before get type" << endl;
  string tt = a->getType(); //token type
  cout << "after get type" << endl;
  if(tt == "+") //addition intexpr
  {
    //a = 1+1
    //load constant 01, store constant in new stack, load constant 01, store constant in  new stack,
    //load constant 00 in acc, add with carry mem1, add with carry, store in new stack, load from last
    //stack place, store in left side memory location


    //calculate the output for the integer addition
    assignIntExpressionLoop(a);
    vector<string> results = assignIntExpressionTerminals; //store in temp array
    assignIntExpressionTerminals.clear(); //clear for future operations

    //store each in memory, which are constants or temp memory addresses
    for(vector<string>::size_type i = 0; i < results.size(); i++)
    {
      string intTerminal = results[i];
      //determine whether it's a constant or load from memory
      if(intTerminal[0] == 'T')//variable
      {

      }

      else //constant
      {
        returnIntSegment.push_back(LDA_C);
        returnIntSegment.push_back(intTerminal); //already added the 0
        //add row in stack
        string stackStore; //TODO: add special row in the stack
        //push back memory address - no XX
        returnIntSegment.push_back(stackStore);

      }
    }
    //clear accumulator
    returnIntSegment.push_back(LDA_C); //A9
    returnIntSegment.push_back("00"); //00

    //add each to accumulator

    //store in memory address

    //assign to left side (tempVarName)


    return returnIntSegment;
  }
  else //just a digit or variable that is a digit
  {
    cout << "ELSE STMT. Type: " << tt << endl;
    if(tt == "int")
    {
      //load value to acc
      returnIntSegment.push_back(LDA_C); //A9
      returnIntSegment.push_back("0" + a->getData()); //digit value

      //store in temporary variable we're assigning to
      returnIntSegment.push_back(STA); //8D
      returnIntSegment.push_back(tempVarName);
      returnIntSegment.push_back("XX");

    }
    else //assigning variable to variable
    {
      //lookup right side temp name of variable
      string rightSideTempVarName = sdTable.lookupTempRow(a);

      //load right side variable in the accumulator
      returnIntSegment.push_back(LDA_M); //AD
      returnIntSegment.push_back(rightSideTempVarName); //load right-side temp var name
      returnIntSegment.push_back("XX");

      //store the accumulator in the memory location of the left side
      returnIntSegment.push_back(STA); //8D
      returnIntSegment.push_back(tempVarName); //left side
      returnIntSegment.push_back("XX");
    }
  }

  return  returnIntSegment;
}

//loop through and grab all terminals on the right side
void CodeGen::assignIntExpressionLoop(Token *a)
{

  //push back number
  if(a->getType() == "int") //number
  {
    assignIntExpressionTerminals.push_back("0" + a->getData());
  }
  //push back the variable T0 location
  else if(a->getType() == "char") //variable
  {
    //get temporary memory address of variable in question
    string tempAddress = sdTable.lookupTempRow(a);
    assignIntExpressionTerminals.push_back(tempAddress);
  }
  //otherwise it's a plus sign, do nothing but recurse

  //recurse through function
  for(vector<Token*>::size_type i = 0; i < a->children.size(); i++)
  {
    assignIntExpressionLoop(a->children[i]);
  }

}

//set up conditionals, don't worry about branches
vector<string> CodeGen::assignBooleanExpressionSegment(Token *a, string tempVarName)
{
  vector<string> returnBooleanSegment;

  return returnBooleanSegment;
}

//needs to create temporary variables and also put string in heap automatically
vector<string> CodeGen::assignStringExpressionSegment(Token *a, string tempVarName)
{
  vector<string> returnStringSegment;

  return returnStringSegment;
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
  string stackHeadHex = intToHex(stackHead);

  //calculate size of stack (for use in overflows later)
    //precaution for no stack being used
  if(sdTable.data.empty()) //no stack used
  {
    stackSize = 0;
  }
  else //stack used, set to number of rows in data
  {
    stackSize = sdTable.data.size(); //each var takes up one byte of space
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