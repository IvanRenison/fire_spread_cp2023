CXX = g++
CXXSTD = c++23
CXXFLAGS = -Wall -Wextra -Werror -std=$(CXXSTD)

CPPOPTFLAGS = -O2

files = $(filter-out main.cpp,$(wildcard *.cpp))
headers = $(wildcard *.hpp)

all: main

main: $(files) $(headers) main.cpp
	$(CXX) $(CXXFLAGS) $(CPPOPTFLAGS) main.cpp $(files)

test: $(files) $(headers) test/test.cpp test/*.csv
	$(CXX) $(CXXFLAGS) $(CPPOPTFLAGS) -I. test/test.cpp $(files) -o test/test
	cd test && ./test

clean:
	rm -f *.o a.out test/test

.PHONY: all test clean
