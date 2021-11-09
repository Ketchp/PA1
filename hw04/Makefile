all: best test 
main: main.c
	g++ -std=c++14 -Wall -pedantic -Wno-long-long -O2 main.c -o main

main_working: main_working.c
	g++ -std=c++14 -Wall -pedantic -Wno-long-long -O2 main_working.c -o main_working

sqrt_alg: sqrt_alg.c
	g++ -std=c++14 -Wall -pedantic -Wno-long-long -O2 sqrt_alg.c -o sqrt_alg

best: best.c
	g++ -std=c++14 -Wall -pedantic -Wno-long-long -O2 best.c -o best

test: best testFile CZE
	@./testFile