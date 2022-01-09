#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// 3:T 2:R 1:S
int getInput ( int *type, double *a, double *b, double *c );
int validate ( int type, double a, double b, double c );
void invalidInput ();
int validateSquare ( double a );
int validateRectangle ( double a, double b );
int validateTriangle ( double a, double b, double c );
int triangleIneq ( double a, double b, double c);
void getSizes ( int type, double a, double b, double c, double *area, double *cir );
void getSizesSq ( double a, double *area, double *cir );
void getSizesRec ( double a, double b, double *area, double *cir );
void getSizesTr ( double a, double b, double c, double *area, double *cir );
void compare ( int type1, int type2, double a, double b );
int compDouble ( double a, double b );


int main()
{
    int type1, type2;
    double a1, b1, c1, a2, b2, c2;
    printf ( "Obrazec #1\n" );
    if ( !getInput ( &type1, &a1, &b1, &c1 ) || !validate ( type1, a1, b1, c1 ) )
    {
        invalidInput ();
        return 0;
    }

    printf ( "Obrazec #2\n" );
    if ( !getInput ( &type2, &a2, &b2, &c2 ) || !validate ( type2, a2, b2, c2 ) )
    {
        invalidInput ();
        return 0;
    }

    double area1, area2;
    double cir1, cir2;
    getSizes ( type1, a1, b1, c1, &area1, &cir1 );
    getSizes ( type2, a2, b2, c2, &area2, &cir2 );

    printf ( "Obvod: " );
    compare ( type1, type2, cir1, cir2 );

    printf ( "Obsah: " );
    compare ( type1, type2, area1, area2 );
}



int getInput ( int *type, double *a, double *b, double *c )
{
    char obj;
    if ( scanf ( " %c ", &obj ) != 1 ) return 0;
    int count;
    switch ( obj )
    {
    case 'T':
        count = 3;
        break;
    case 'R':
        count = 2;
        break;
    case 'S':
        count = 1;
        break;    
    default:
        return 0;
    }
    double sides[ 3 ];
    for ( int i = 0; i < count; i++ )
    {
        if ( scanf ( " %lf ", sides + i ) != 1 ) return 0;
    }
    switch ( count )
    {
    case 3:
        *c = sides[ 2 ];
    case 2:
        *b = sides[ 1 ];
    case 1:
        *a = sides[ 0 ];
    }
    *type = count;
    return 1;
}

void invalidInput ()
{
    printf ( "Nespravny vstup.\n" );
}

int validate ( int type, double a, double b, double c )
{
    switch ( type )
    {
    case 1:
        return validateSquare ( a );
    case 2:
        return validateRectangle ( a, b );
    case 3:
        return validateTriangle ( a, b, c );
    }
    return 0;
}

int validateSquare ( double a )
{
    return a > 0;
}

int validateRectangle ( double a, double b )
{
    return a > 0 && b > 0 && a != b;
}

int validateTriangle ( double a, double b, double c )
{
    return a > 0 && b > 0 && c > 0 && triangleIneq ( a, b, c);
}

int triangleIneq ( double a, double b, double c)
{
    double eps = + __DBL_EPSILON__ * 10;
    return a + b > c + eps && a + c > b + eps && b + c > a + eps;
}

void getSizes ( int type, double a, double b, double c, double *area, double *cir )
{
    switch ( type )
    {
    case 1:
        getSizesSq ( a, area, cir );
        break;
    case 2:
        getSizesRec ( a, b, area, cir );
        break;
    case 3:
        getSizesTr ( a, b, c, area, cir );
        break;
    }
}

void getSizesSq ( double a, double *area, double *cir )
{
    *area = a * a;
    *cir = 4 * a;
}

void getSizesRec ( double a, double b, double *area, double *cir )
{
    *area = a * b;
    *cir = 2 * ( a + b );
}

void getSizesTr ( double a, double b, double c, double *area, double *cir )
{
    *cir = a + b + c;
    double s = *cir / 2;
    *area = sqrtl ( s * ( s - a ) * ( s - b ) * ( s - c) );
}

void compare ( int type1, int type2, double a, double b )
{
    switch ( type1 )
    {
    case 1:
        printf ( "ctverec #1 " );
        break;
    case 2:
        printf ( "obdelnik #1 " );
        break;
    case 3:
        printf ( "trojuhelnik #1 " );
        break;
    }

    switch ( compDouble( a, b ) )
    {
    case -1:
        printf ( "<" );
        break;
    case 0:
        printf ( "=" );
        break;
    case 1:
        printf ( ">" );
        break;
    }

    switch ( type2 )
    {
    case 1:
        printf ( " ctverec #2\n" );
        break;
    case 2:
        printf ( " obdelnik #2\n" );
        break;
    case 3:
        printf ( " trojuhelnik #2\n" );
        break;
    }
}

int compDouble ( double a, double b )
{
    double eps = __DBL_EPSILON__ * 1000;
    if ( -eps < (a - b) && (a - b) < eps ) return 0;
    if ( a > b ) return 1;
    return -1;
}















