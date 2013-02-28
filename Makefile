CC = g++
CPPFLAGS = -Wall -Wextra -pedantic -std=c++11
LDFLAGS = -pthread


all: example

example.o: example.cpp

clean:
	rm -f *.o example
