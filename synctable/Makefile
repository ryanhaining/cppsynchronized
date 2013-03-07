LINK.o = $(LINK.cc)
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -O2

LDFLAGS = -pthread

OBJECTS = example.o synctable.o
PRGM = example

all: $(PRGM)

$(PRGM): $(OBJECTS)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm -f *.o $(PRGM)
