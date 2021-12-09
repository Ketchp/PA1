all: main test
main: main.c
	g++ -std=c++14 -Wall -pedantic -Wno-long-long -O2 main.c -o main

test: main testFile CZE
	@./testFile