all: main test
main: main.c
	g++ -std=c++14 -Wall -pedantic -Wno-long-long main.c -o main

test: main ENG testFile
	./testFile

ENG: sample.tgz
	tar -xvzf sample.tgz