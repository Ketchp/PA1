#include <stdio.h>
#include <math.h>
#include <float.h>

int precision = 1e3;

int charInStr(char character, char* str);
int LDBLequal(long double a, long double b);
//int DBLequal(double a, double b);

int main(){
    printf("Zadejte rovnici:\n");
    long double A,B,C,expected=0;
    char operand, equal;
    int ret = scanf(" %LF %c %LF %c %LF", &A, &operand, &B, &equal, &C);
//    printf("%d, %Lg, %Lg, %Lg, %c, %c\n", ret, A, B, C, operand, equal);
    if(!LDBLequal(A,(double)A) || !LDBLequal(B,(double)B) || !LDBLequal(C,(double)C)){
        printf("Nespravny vstup.\n");
        return 0;
    }
    if(ret != 5 || equal != '=' || !charInStr(operand, (char *)"+-*/") || (operand == '/' && LDBLequal(B,0))){
        printf("Nespravny vstup.\n");
        return 0;
    }
    if(A == NAN || B == NAN || C == NAN || A == INFINITY || B == INFINITY || C == INFINITY ||
    A == -INFINITY || B == -INFINITY || C == -INFINITY){
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
        expected = A+B;
        break;
    case '-':
        expected = A-B;
        break;
    case '*':
        expected = A*B;
        break;
    case '/':
        expected = truncl(A/B);
        break;
    
    }
    if(expected == INFINITY || expected == NAN || expected == -INFINITY){
         printf("Nespravny vstup.\n");
        return 0;       
    }
    if(LDBLequal(expected, C)) printf("Rovnice je spravne.\n");
    else printf("%Lg != %Lg\n", expected, C);
    return 0;
}

int charInStr(char character, char* str){
    for(int i = 0; str[i] != '\0'; i++) if(str[i] == character) return 1;
    return 0;
}

int LDBLequal(long double a, long double b){
    return abs(a-b) <= precision*LDBL_EPSILON*abs(a+b);
}
/*
int DBLequal(double a, double b){
    return abs(a-b) <= precision*DBL_EPSILON*abs(a+b);
}*/