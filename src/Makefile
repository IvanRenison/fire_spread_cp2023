CC = g++
CPPFLAGS = -Wall -Wextra -Werror -std=c++23

CPPOPTFLAGS = -O2

files = $(wildcard *.cpp)
headers = $(wildcard *.hpp)

all: main

main: $(files) $(headers)
	$(CC) $(CPPFLAGS) $(CPPOPTFLAGS) *.cpp

clean:
	rm -f *.o a.out a.exe