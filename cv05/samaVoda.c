#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAXS  10
#define MINS -10
#define DIST 0.05

void printTable(double *guesses, int count){
    int *table = (int *)calloc(sizeof(int),(MAXS - MINS+1)*(MAXS - MINS+1));
    for(int i = 0; i < count; i++){
        int x = *(guesses+2*i);
        int y = *(guesses+2*i+1);
        *(table+(MAXS - MINS+1)*(x-MINS) + (y-MINS)) = 1;
    }
    for(int i = 0; i < (MAXS - MINS+1); i++)
        printf("-");
    printf("\n");
    for(int i = 0; i < (MAXS - MINS+1); i++){
        for(int j = 0; j < (MAXS - MINS+1); j++){
            printf(table+(MAXS - MINS+1)*i + j?"0":"1");
        }
        printf("\n");
    }
}

double randomDBL(double min, double max){
    double random = (double)rand()/RAND_MAX;
    return (max-min)*random + min;
}

double distance(double ax, double ay, double bx, double by){
    return sqrt( (ax-bx)*(ax-bx) + (ay-by)*(ay-by) );
}

int main(){
    srand(time(NULL));
    double target_x = randomDBL(MINS, MAXS);
    double target_y = randomDBL(MINS, MAXS);
    double newDist, minDist = distance(target_x, target_y, 0, 0);
    double *guesses = (double *)malloc(2*sizeof(double));
    int guessCount = 0;
    int guessCap = 2;
    double player_x, player_y;

    
    while(scanf("%lf %lf", &player_x, &player_y) == 2){
        if(player_x < -10 || player_x > 10 || player_y < -10 || player_y > 10){
            printf("Nespravna pozicia\nSkus znovu\n");
            continue;
        }
        *(guesses+guessCount*2) = player_x;
        *(guesses+guessCount*2+1) = player_y;
        guessCount++;
        if(guessCap == guessCount*2){
            guessCap <<= 1;
            guesses = (double *)realloc(guesses, guessCap);
        }
        newDist = distance(target_x, target_y, player_x, player_y);
        if(newDist < DIST){
            printf("Hori!\n");
            printf("Pocet pokusov: %d\n", guessCount);
            printf("Hladane cisla: %lf %lf\n", target_x, target_y);
            return 0;
        }
        else if(newDist < minDist){
            printf("Prihoriva\n");
            minDist = newDist;
        }
        else{
            printf("Sama voda\n");
        }
        printTable(guesses, guessCount);
    }
    printf("Nespravny vstup\nPrehral si\n");
    return 0;
}