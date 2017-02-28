#ifndef COMPILER_OUTPUT_H
#define COMPILER_OUTPUT_H

class Output {
  private:
    void clearHTML();

    void printHTMLHeader();

    void printTokens();

    void printCST();

    void printHTMLFooter();

  public:
    void printOutput()
    {
      printHTMLHeader();
      printTokens();
      printCST();

    }

};

#endif