all: plachty test
plachty: plachty.c
	g++ -Wall -pedantic -Werror plachty.c -o plachty

test: plachty testFile CZE
	./testFile