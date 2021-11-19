all: best test 
main: main.c
	g++ -std=c++14 -Wall -pedantic -Wno-long-long -O2 main.c -o main

codeReview: codeReview.c
	g++ -std=c++14 -Wall -pedantic -Wno-long-long -O2 codeReview.c -o codeReview

sqrt_alg: sqrt_alg.c
	g++ -std=c++14 -Wall -pedantic -Wno-long-long -O2 sqrt_alg.c -o sqrt_alg

best: best.c
	g++ -std=c++14 -Wall -pedantic -Wno-long-long -O2 -g best.c -o best

test: best testFile CZE
	@./testFile