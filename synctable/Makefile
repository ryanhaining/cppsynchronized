LINK.o = $(LINK.cc)
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -O2 -g \
		   -I"/usr/include/x86_64-linux-gnu/c++/4.7/"
LDFLAGS = -pthread

OBJECTS = example.o synctable.o

all: example

example: $(OBJECTS)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm -f *.o example
