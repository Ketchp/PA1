#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

#define debug 0

unsigned long long accCiph(unsigned long long base, unsigned long long ciphers);

unsigned long long powers[65];

int main(int argc, char *argv[]){
    int size = 1000000*1000;
    int *ptr = (int *)malloc(size * sizeof(int));
    for(int i = 0; i < size; i+=10000)
    {
        ptr[i] = i * i;
    }
    free(ptr);
    char baseChar[]="0123456789abcdefghijklmnopqrstuvwxyz";
    printf("Pozice a soustava:\n");
    unsigned long long position, base, last_base=0, ciphers, guess, number, temp_num;
    int ret, min_cip, max_cip;
    unsigned int max_power = 64;
    char output[65];
    powers[0] = 1;
    long long temp_pos, temp_base;
    while((ret = scanf("%lld %lld", &temp_pos, &temp_base)) != EOF){
        if(ret != 2 || temp_pos < 0 || temp_base < 2 || temp_base > 36){
            printf("Nespravny vstup.\n");
            break;
        }
        base = temp_base;
        position = temp_pos;
        if(position < base){
            printf("%c\n^\n", baseChar[position]);
            continue;
        }
        if(base != last_base){
            temp_num = ULLONG_MAX/base;
            for(int i = 0; i < 65; i++){
                if(temp_num < powers[i]){
                    max_power = i;
                    if(debug) printf("Max: %u\n", max_power);
                    break;
                }
                powers[i+1] = powers[i]*base;
            }
        }
        min_cip = 1;
        max_cip = max_power;
        ciphers = (min_cip+max_cip)/2;
        guess = accCiph(base, ciphers);
        while(1){
            if(debug) printf("%llu vs %llu %u<%llu<%u\n", guess, position, min_cip, ciphers, max_cip);
            if(guess <= position){
                min_cip = ciphers+1;
            }
            else if(guess > position){
                max_cip = ciphers;
            }
            if(ciphers == (unsigned long long)(min_cip+max_cip)/2){
                if(debug) printf("solve: %llu < %llu < %llu for %u, %llu, %u\n",
                accCiph(base, min_cip), accCiph(base, ciphers),
                accCiph(base, max_cip), min_cip, ciphers, max_cip);
                ciphers = min_cip;
                break;
            }
            else{
                ciphers = (min_cip+max_cip)/2;
            }
            guess = accCiph(base, ciphers);
            if(!guess){
                ciphers--;
                guess = accCiph(base, ciphers);
            }
            if(debug) sleep(1);
        }
        if(debug) printf("cip: %llu\n", ciphers);
//        ciphers++;
        position -= accCiph(base, ciphers-1);
        number = powers[ciphers-1] + position / ciphers;

        if(debug){
        printf("Cifry: %llu\nZaklad: %llu\n%lldte cislo\n%lluty znak\nCislo: %llu\n",
         ciphers, powers[ciphers-1], position / ciphers, position % ciphers, number);
        }

        int i = 0;
        while(number){
            output[i] = baseChar[number % base];
            number /= base;
            i++;
        }
        while(i){
            printf("%c", output[--i]);
        }
        printf("\n");

/*        for(int i = ciphers-1; i > 0; i--){
            printf("%lld-\n",powers[i]);
            printf("%c\n", baseChar[number/powers[i]]);
            number %= powers[i];
        }
        printf("%c\n", baseChar[number]);
*/
        for(unsigned int i = 0; i<position%ciphers;i++){
            printf(" ");
        }
        printf("^\n");

        last_base = base;
    }
    return 0;
}

unsigned long long accCiph(unsigned long long base, unsigned long long ciphers){
    unsigned long long limit = ULLONG_MAX/powers[ciphers-1];
    if(limit < (ciphers*(base-1)-1)) return 0;

    unsigned long long first = powers[ciphers-1]*(ciphers*(base-1)-1)+1;
    if(first<powers[ciphers-1]) return 0;

    first /= base-1;

    limit = ULLONG_MAX/first;
    if(limit < base) return 0;
    first *= base;
    return first;
}

