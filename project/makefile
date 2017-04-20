CXX = g++
CXXFLAGS=-std=c++11

all: lex

lex:  cst.o \
      error.o \
      genSymbolTable.o \
      lexer.o \
      main.o \
      output.o \
      parser.o \
      semantic.o \
      stEntry.o \
      symbolTable.o \
      token.o
	$(CXX) $^ -o test1 $(CXXLIBS)
	
%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
clean:
	rm *.o
