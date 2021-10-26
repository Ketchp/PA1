all: sachovnice test
sachovnice: sachovnice.c
	g++ -std=c++14 -Wall -pedantic -Wno-long-long -O2 sachovnice.c -o sachovnice

test: sachovnice CZE testFile
	./testFile

cezar: cezar.c
	g++ -std=c++14 -Wall -pedantic -Wno-long-long -O2 cezar.c -o cezar

samaVoda: samaVoda.c
	g++ -std=c++14 -Wall -pedantic -Wno-long-long -O2 samaVoda.c -o samaVoda
