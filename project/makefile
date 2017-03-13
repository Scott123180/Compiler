CXX = g++
CXXFLAGS=-std=c++11

all: lex

lex:  main.o error.o lexer.o output.o token.o parser.o cst.o
	$(CXX) $^ -o test1 $(CXXLIBS)
	
%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
clean:
	rm *.o
