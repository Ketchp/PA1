all: main test
main: main.c
	g++ -Wall -pedantic -Werror main.c -o main

test: main testFile CZE
	./testFile