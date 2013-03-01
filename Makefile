LINK.o = $(LINK.cc)
CXX = clang++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -O2  \
		   
#-I"/usr/include/x86_64-linux-gnu/c++/4.7/"
LDFLAGS = -pthread


all: example

example.o: example.cpp

clean:
	rm -f *.o example
