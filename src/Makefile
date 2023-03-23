CC = g++
CPPFLAGS = -Wall -Wextra -Werror -std=c++23

CPPOPTFLAGS = -O2

files = $(wildcard *.cpp)
files := $(filter-out main.cpp,$(files))
headers = $(wildcard *.hpp)

all: main

main: $(files) $(headers) main.cpp
	$(CC) $(CPPFLAGS) $(CPPOPTFLAGS) main.cpp $(files)

test: $(files) $(headers) test/test.cpp test/*.csv
	$(CC) $(CPPFLAGS) $(CPPOPTFLAGS) -I. test/test.cpp $(files) -o test/test
	./test/test

clean:
	rm -f *.o a.out test/test