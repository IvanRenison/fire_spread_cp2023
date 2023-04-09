CXX = g++
CXXSTD = c++23
CXXFLAGS = -Wall -Wextra -Werror -std=$(CXXSTD)

CPPOPTFLAGS = -Ofast

files = $(filter-out ../src/main.cpp,$(wildcard ../src/*.cpp))
headers = $(wildcard ../src/*.hpp)

all: fire_animation_data

fire_animation_data: $(files) $(headers) fire_animation_data.cpp
	$(CXX) $(CXXFLAGS) $(CPPOPTFLAGS) -I../src -DGRAPHICS fire_animation_data.cpp $(files) -o fire_animation_data

clean:
	rm -f fire_animation_data

.PHONY: all test clean
