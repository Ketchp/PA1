#include <stdio.h>

int getNum();
void printTop(int length);


int main(){
    printf("Zadejte pocet poli:\n");
    int count = getNum(); // 4 -> |XX  XX  |
    if(!count) return 0;

    printf("Zadejte velikost pole:\n");
    int size = getNum(); // 4 -> |..X    XXXX    X..|
    if(!size) return 0;

    int length = count * size, odd_row, odd_column;
    printTop(length);
    for(int row = 0; row < length; row++){
        odd_row = (row % (2*size)) < size;
        printf("|");
        for(int column = 0; column < length; column++){
            odd_column = (column % (2*size)) < size;
            printf( odd_row == odd_column % 2 ? " ": "X");
        }
        printf("|\n");
    }
    printTop(length);
    return 0;
}

int getNum(){
    int num;
    int ret = scanf("%d", &num);
    if(ret != 1 || num <= 0){
        printf("Nespravny vstup.\n");
        return 0;
    }
    return num;
}

void printTop(int length){
    printf("+");
    for(int i = 0; i < length; i++) printf("-");
    printf("+\n");
}

/*
Zadejte pocet poli:
5
Zadejte velikost pole:
3
+---------------+
|   XXX   XXX   |
|   XXX   XXX   |
|   XXX   XXX   |
|XXX   XXX   XXX|
|XXX   XXX   XXX|
|XXX   XXX   XXX|
|   XXX   XXX   |
|   XXX   XXX   |
|   XXX   XXX   |
|XXX   XXX   XXX|
|XXX   XXX   XXX|
|XXX   XXX   XXX|
|   XXX   XXX   |
|   XXX   XXX   |
|   XXX   XXX   |
+---------------+
*/
