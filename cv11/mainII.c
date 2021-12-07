#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>
#include <unistd.h>

int readInt( int max )
{
    int num;
    if( scanf( "%d\n", &num ) != 1 || num <= 0 || num > max )
    {
        printf( "Nespravny vstup.\n" );
        exit( 0 );
    }
    return num;
}

unsigned long long printBin(unsigned long long number)
{
    if( !number ) return 0;
    int p = 0;
    for( unsigned long long mask = 1ull << 63; mask; mask >>= 1)
    {
        unsigned long long n = number & mask;
        if( n ) p = 1;
        else if( !p ) continue;
        printf( "%c", n ? '1' : '0' );
    }
    printf( "\n" );
    return 1;
}

unsigned long long findBin( unsigned long long n, int k, int streak,
 unsigned long long mask, unsigned long long number )
{
    unsigned long long last = number & ( mask >> 1 );
    unsigned long long count = 0;
    if( !last || streak < k ) 
    {
        count = printBin( number | mask );
    }
    if( mask == n ) return count;
    if( streak == k )
    {
        return count + findBin( n, k, 1, mask << 1, number | ( last ? 0 : mask ) );
    }

    count += findBin( n, k, 1, mask << 1, number ^ mask ^ ( last << 1 ) );
    count += findBin( n, k, streak + 1, mask << 1, number ^ ( last << 1 ) );

    return count;
}

int main()
{
    printf( "Pocet cifer:\n" );
    unsigned long long n = readInt( 64 ) - 1;
    n = 1ull << n;
    printf( "Max. stejnych po sobe:\n" );
    int k = readInt( __INT32_MAX__ );

//    printf( "%d %d\n", n, k );
    unsigned long long count = findBin( n, k, 0 , 1ull, 0ull );

    printf( "Celkem: %llu\n", count);

    return 0;
}