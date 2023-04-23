CXX = g++
CXXSTD = c++23
CXXFLAGS = -Wall -Wextra -Werror -std=$(CXXSTD)

CPPOPTFLAGS = -Ofast -march=native -flto

files = $(filter-out ../main.cpp,$(wildcard ../*.cpp))
headers = $(wildcard ../*.hpp)

all: predParams

predParams: $(files) $(headers) predParams.cpp predParams.hpp predParams_main.cpp predParams_main_singleFile.cpp
	$(CXX) $(CXXFLAGS) $(CPPOPTFLAGS) -I.. predParams_main_singleFile.cpp -o predParams


clean:
	rm -f predParams

.PHONY: all test clean
