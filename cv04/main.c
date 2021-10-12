#include <stdio.h>
#include <math.h>

int charInStr(char character, char* str);

int main(){
    printf("Zadejte rovnici:\n");
    long double a,b,c,expected=0;
    char operand, equal;
    int ret = scanf(" %LF %c %LF %c %LF", &a, &operand, &b, &equal, &c);
    if(ret != 5 || equal != '=' || !charInStr(operand, (char *)"+-*/") || (operand == '/' && b==0)){
        printf("Nespravny vstup.\n");
        return 0;
    }
    if(a == NAN || b == NAN || c == NAN || a == INFINITY || b == INFINITY || c == INFINITY ||
    a == -INFINITY || b == -INFINITY || c == -INFINITY){
        printf("Nespravny vstup.\n");
        return 0;
    }
    while(scanf("%c", &equal) == 1){
        if(!charInStr(equal, (char *)" \t\r\n")){
            printf("Nespravny vstup.\n");
            return 0;
        }
    }
    switch (operand){
    case '+':
        expected = a+b;
        break;
    case '-':
        expected = a-b;
        break;
    case '*':
        expected = a*b;
        break;
    case '/':
        expected = (long long)(a/b);
        break;
    
    }
    if(expected == INFINITY || expected == NAN){
         printf("Nespravny vstup.\n");
        return 0;       
    }
    if(abs(expected - c) < (long double)1/100000000000) printf("Rovnice je spravne.\n");
    else printf("%Lg != %Lg\n", expected, c);
    return 0;
}

int charInStr(char character, char* str){
    for(int i = 0; str[i] != '\0'; i++) if(str[i] == character) return 1;
    return 0;
}