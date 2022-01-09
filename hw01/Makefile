all: plachty obrazce

obrazce: obrazce.c
	g++ -Wall -pedantic -Werror obrazce.c -o obrazce

plachty: plachty.c
	g++ -Wall -pedantic -Werror plachty.c -o plachty

test: plachty testFile CZE
	./testFile