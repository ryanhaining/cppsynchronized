LINK.o = $(LINK.cc)
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -O0 -g  \
		   -I"/usr/include/x86_64-linux-gnu/c++/4.7/"
LDFLAGS = -pthread

all: example tableexample

%.o: %.cpp

clean:
	rm -f *.o example tableexample
