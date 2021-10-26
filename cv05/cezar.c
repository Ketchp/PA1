#include <stdio.h>

#define ALPHABET_LENGTH 26

int main(){
    int shift, ret = scanf("%d", &shift);
    if(ret != 1){
        printf("Pozadovana sifra\n");
        return 0;
    }
    char in, offset;
    while(scanf("%c", &in) == 1){
        if(in == '\n'){
            printf("\n");
            continue;
        }
        if(in < 'A' || ('Z' < in && in < 'a') || 'z' < in){
            printf("Sifrovanie definovane len pre pismena\n");
            return 0;
        }
        offset = in <= 'Z' ? 65:97;
        in -= offset;
        in += shift;
        in %= ALPHABET_LENGTH;
        in = (in+ALPHABET_LENGTH)%ALPHABET_LENGTH;
        in += offset;
        printf("%c", in);
    }

    return 0;
}
/*
65 A
90 Z

97 a
122 z
*/
