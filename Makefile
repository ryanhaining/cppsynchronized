LINK.o = $(LINK.cc)
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g
LDFLAGS = -pthread


all: example

example.o: example.cpp

clean:
	rm -f *.o example
