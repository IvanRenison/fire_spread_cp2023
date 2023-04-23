CXX = g++
CXXSTD = c++23
CXXFLAGS = -Wall -Wextra -Werror -std=$(CXXSTD)

CPPOPTFLAGS = -Ofast

files = $(filter-out ../main.cpp,$(wildcard ../*.cpp))
headers = $(wildcard ../*.hpp)

all: predParams

predParams: $(files) $(headers) predParams.cpp predParams.hpp predParams_main.cpp
	$(CXX) $(CXXFLAGS) $(CPPOPTFLAGS) -I.. \
	predParams.cpp predParams_main.cpp $(files) -o predParams


clean:
	rm -f predParams

.PHONY: all test clean
