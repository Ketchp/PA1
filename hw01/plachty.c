#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

double precision = DBL_EPSILON * 1;

int validate(double num);
long long calculate(double width, double height, double x, double y, double overlap);
long long apprCeil(double num);


int main(){
    printf("Velikost latky:\n");
    double x, y;
    int ret = scanf("%lf %lf", &x, &y);
    if(ret != 2 || validate(y) || validate(x)){
        printf("Nespravny vstup.\n");
        return 0;
    }

    printf("Velikost plachty:\n");
    double width, height;
    ret = scanf("%lf %lf", &height, &width);
    if(ret != 2 || validate(height) || validate(width)){
        printf("Nespravny vstup.\n");
        return 0;
    }

    if((x >= width && y >= height) || (y >= width && x >= height)){
        printf("Pocet kusu latky: 1\n");
        return 0;
    }
    printf("Prekryv:\n");
    double overlap;
    ret = scanf("%lf", &overlap);
    if(ret != 1 || overlap < 0 || overlap == INFINITY || overlap == -INFINITY || overlap == NAN){
        printf("Nespravny vstup.\n");
        return 0;
    }

    long long plain, rotated;
    plain = calculate(width, height, x, y, overlap);
    rotated = calculate(width, height, y, x, overlap);
    if(plain == 0 && rotated == 0){
        printf("Nelze vyrobit.\n");
        return 0;
    }

    ret = plain > rotated ? rotated : plain;
    if(!ret) ret = plain > rotated ? plain : rotated;
    printf("Pocet kusu latky: %d\n", ret);
    return 0;
}

int validate(double num){
    return num <= 0 || num == INFINITY || num == -INFINITY || num == NAN;
}

long long calculate(double width, double height, double x, double y, double overlap){
//    printf("%lg %lg %lg %lg %lg = ", width, height, x, y, overlap);
    width -= x;
    height -= y;
    x -= overlap;
    y -= overlap;
    if((width > 0 && x <= 0) || (height > 0 && y <= 0)) return 0;
    long long xn = 1, yn = 1;
    if(width > 0) xn += apprCeil(width / x);
    if(height > 0) yn += apprCeil(height / y);
//    printf(".%lf %lf.", width / x, height / y);
//    printf("%d\n", xn*yn);
    return xn * yn;
}

long long apprCeil(double num){
    if(num - (long long)num > DBL_EPSILON*10) return (long long)num + 1;
    return num;
}