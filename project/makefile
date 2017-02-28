CXX = g++
CXXFLAGS=-std=c++11

all: lex

lex:  main.o token.o lexer.o parser.o output.o
	$(CXX) $^ -o test1 $(CXXLIBS)
	
%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
clean:
	rm *.o
