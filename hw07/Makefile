all: faster test
main: main.c
	g++ -std=c++14 -Wall -pedantic -Wno-long-long -O0 -g main.c -o main

faster: faster.c
	g++ -std=c++14 -Wall -pedantic -Wno-long-long -O0 -g faster.c -o faster

test: faster testFile CZE
	@./testFile