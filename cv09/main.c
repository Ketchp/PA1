#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int equal( char start, char end);

int main()
{
    printf( "Zadejte retezec:\n" );
    size_t capacity;
    char *buff = NULL;
    int read;
    while( (read = getline( &buff, &capacity, stdin )) != -1)
    {
        if( read == 1 || buff[ read-1 ] != '\n')
        {
            printf( "Nespravny vstup.\n" );
            free(buff);
            return 0;
        }
        char *start = buff, *end = buff + read - 1;
        int printable = 0, sensitive = 1;
        while( start < end )
        {
            if( isspace( *start ) ) start++;
            else if( isspace( *end ) ) end--;
            else
            {
                printable = 1;
                if( *start == *end )
                {
                    start++;
                    end--;
                }
                else if( equal( *start ,*end ) )
                {
                    sensitive = 0;
                    start++;
                    end--;
                }
                else break;
            }
            
        }

        if( start < end )
        {
            printf( "Retezec neni palindrom.\n" );
        }
        else
        {
            if( !printable )
            {
                printf( "Nespravny vstup.\n" );
                free(buff);
                return 0;
            }
            printf( "Retezec je palindrom " );
            if( sensitive ) printf( "(case-sensitive).\n" );
            else  printf( "(case-insensitive).\n" );
        }

    }
    free( buff );
    return 0;
}

int equal( char start, char end)
{
    if( !isalpha(start) || !isalpha(end) ) return 0;
    return abs( start - end ) == abs( 'A' - 'a' ); 
}
