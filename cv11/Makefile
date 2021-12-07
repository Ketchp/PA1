all: main test 
main: mainI.c
	g++ -std=c++14 -Wall -pedantic -Wno-long-long -O0 -g mainI.c -o main

test: main testFile CZE
	@./testFile