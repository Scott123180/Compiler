CXX = g++
CXXFLAGS=-std=c++11

all: lex

lex:  error.o lexer.o main.o output.o token.o parser.o
	$(CXX) $^ -o test1 $(CXXLIBS)
	
%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
clean:
	rm *.o
