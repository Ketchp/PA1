#include <stdio.h>

int charInStr(char character, char* str);

int main(){
    printf("Enter an equation:\n");
    double a,b,c,expected;
    char operand, equal;
    int ret = scanf(" %lf %c %lf %c %lf", &a, &operand, &b, &equal, &c);
    if(ret != 5 || equal != '=' || !charInStr(operand, "+-*/") || (operand == '/' && b==0)){
        printf("Invalid input.\n");
        return 0;
    }
    while(scanf("%c", &equal) == 1){
        if(!charInStr(equal, " \t\r\n")){
            printf("Invalid input.\n");
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
        expected = (int)(a/b);
        break;
    
    }
    if(expected == c) printf("The equation holds.\n");
    else printf("%g != %g\n", expected, c);
    return 0;
}

int charInStr(char character, char* str){
    for(int i = 0; str[i] != '\0'; i++) if(str[i] == character) return 1;
    return 0;
}