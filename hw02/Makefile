all: ciselnaRada test
ciselnaRada: ciselnaRada.c
	g++ -Wall -pedantic -Werror ciselnaRada.c -o ciselnaRada

test: ciselnaRada testFile basic big radix
	./testFile

basic/CZE/0000_in.txt: sample.tgz
	@tar -xvzf sample.tgz