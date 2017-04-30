#ifndef COMPILER_ERROR_H
#define COMPILER_ERROR_H

#include <string>
#include <vector>

using namespace std;

class Error
{

  private:
    bool quit;
    int state;
    int line, pos;
    string message;
    vector<string> data;

  public:
    enum errorStage {fileInput, lex, parse, semantic, token, codeGen};

    //create and assign error object
    Error(bool q, int e, int l, int p, vector<string> d, string m);

};
#endif //COMPILER_ERROR_H