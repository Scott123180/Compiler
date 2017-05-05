CXX = g++
CXXFLAGS=-std=c++11

all: lex

lex:  codeGen.o \
      cst.o \
      error.o \
      genSymbolTable.o \
      jumps.o \
      jumpsRow.o \
      lexer.o \
      main.o \
      output.o \
      parser.o \
      semantic.o \
      staticData.o \
      staticDataRow.o \
      stEntry.o \
      symbolTable.o \
      token.o
	$(CXX) $^ -o test1 $(CXXLIBS)
	
%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -g $< -o $@
	
clean:
	rm *.o
