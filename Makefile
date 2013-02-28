LINK.o = $(LINK.cc)
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -O2
LDFLAGS = -pthread


all: example

example.o: example.cpp

clean:
	rm -f *.o example
