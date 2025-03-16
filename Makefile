CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall

all: differentiator test

differentiator: main.o expression.o parser.o
	$(CXX) $(CXXFLAGS) -o differentiator main.o expression.o parser.o

test: test.o expression.o parser.o
	$(CXX) $(CXXFLAGS) -o test test.o expression.o parser.o

main.o: main.cpp expression.hpp parser.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

test.o: test.cpp expression.hpp parser.hpp
	$(CXX) $(CXXFLAGS) -c test.cpp

expression.o: expression.cpp expression.hpp
	$(CXX) $(CXXFLAGS) -c expression.cpp

parser.o: parser.cpp parser.hpp expression.hpp
	$(CXX) $(CXXFLAGS) -c parser.cpp

clean:
	rm -f *.o differentiator test
