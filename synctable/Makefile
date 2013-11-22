LINK.o = $(LINK.cc)
CXX = clang++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11

LDFLAGS = -pthread

PRGM = example
OBJECTS = $(PRGM).o synctable.o

all: $(PRGM)

$(PRGM): $(OBJECTS)

clean:
	rm -f *.o $(PRGM)
