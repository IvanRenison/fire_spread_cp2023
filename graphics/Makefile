CXX = g++
CXXSTD = c++23
CXXFLAGS = -Wall -Wextra -Werror -std=$(CXXSTD)

CPPOPTFLAGS = -O2

files = $(filter-out ../src/main.cpp,$(wildcard ../src/*.cpp))
headers = $(wildcard ../src/*.hpp)

all: graph_data

graph_data: $(files) $(headers) graph_data.cpp
	$(CXX) $(CXXFLAGS) $(CPPOPTFLAGS) -I../src -DGRAPHICS graph_data.cpp $(files) -o graph_data

clean:
	rm -f graph_data

.PHONY: all test clean
