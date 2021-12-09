main: main.c
	gcc -Wall -pedantic -Werror main.c -o main

test: main ENG testFile
	./testFile

ENG: sample.tgz
	tar -xvzf sample.tgz