all: main test
main: main.c
	g++ -std=c++14 -Wall -pedantic -Wno-long-long -O2 main.c -o main

test: main CZE testFile
	./testFile

CZE: sample.tgz
	tar -xvzf sample.tgz