all: main test 
main: main.c
	g++ -std=c++14 -Wall -pedantic -Wno-long-long -O0 -g main.c -o main

test: main testFile CZE
	@./testFile