#ifndef COMPILER_OUTPUT_H
#define COMPILER_OUTPUT_H

class Output {
  private:
    void clearHTML();

    void printHTMLHeader();

    void printTokens();

    void printCST();

    void printHTMLFooter();

    void printError();

  public:
    void printOutput()
    {
      clearHTML();
      printHTMLHeader();
      printTokens();
      //printCST();
      printHTMLFooter();
    }

    void printErrors()
    {
      clearHTML();
      printHTMLHeader();
      printError();
      printHTMLFooter();
    }

};

#endif