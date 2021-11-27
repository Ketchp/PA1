#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <assert.h>
#include <unistd.h>

int isUpper( const char *str )
{
    while( 'A' <= *str && *str <= 'Z' ) str++;
    return !*str;
}

int isEmpty( char *str )
{
    while( *str ) if( !isspace( *str++ ) ) return 0;
    return 1;
}

int parseLine( char *operation, int *inWordChar, char **text )
{
    *operation = '\0';
    char *line = NULL;
    size_t lineBuffLen;
    ssize_t lineLen = getline( &line, &lineBuffLen, stdin );
    if( lineLen <= 0 )
    {
        free( line );
        return 0;
    }
    *operation = '1';
    if( line[ lineLen - 1 ] != '\n' )
    {
        free( line );
        return 0;
    }
    char tmpOp;
    int tmpN;
    char *tmpText = (char *)malloc( lineLen * sizeof( *tmpText ) );
    *tmpText = '\0';
    int parsed = sscanf( line, "%c %d %[^.]", &tmpOp, &tmpN, tmpText );
    free( line );
    if( parsed != 3 || ( tmpOp != '#' && tmpOp != '?' ) || tmpN <= 0 )
    {
        free( tmpText );
        return 0;
    }
    char *newText = (char *)malloc( strlen( tmpText ) * sizeof( *newText ) );
    *newText = '\0';
    char *trail = (char *)malloc( strlen( tmpText ) * sizeof( *trail ) );
    *trail = '\0';

    parsed = sscanf( tmpText, " \"%[^\"]\"%[^.]", newText, trail );
    free( tmpText );
    if( parsed < 1 || !isEmpty( trail ) )
    {
//        printf( "%s:%s:%d\n", newText, trail, parsed );
        free( trail );
        free(newText);
        return 0;
    }

    free( trail );
    *operation = tmpOp;
    *inWordChar = tmpN;
    *text = newText;
    return 1;
}


void fail( char *contraction )
{
    free( contraction );
    printf( "Nespravny vstup.\n" );
    exit( 0 );
}

int lowerString( char *str )
{
    while( *str )
    {
        if( 'A' <= *str && *str <= 'Z' ) *str = tolower( *str );
        if( *str == '"' ) return 0;
        str++;
    }
    return 1;
}

void printAbbreviation( char *start, int *allPositions, int length )
{
    printf( "\"" );
    int index = 0;
    int upper = 0;
    while( start[ index ] )
    {
        if( upper < length && allPositions[ upper ] == index )
        {
            printf( "%c", toupper( start[ index ] ) );
            upper++;
        }
        else printf( "%c", start[ index ] );
        index++;
    }
    printf( "\"\n" );
}

long long int findContractions( char * const start, char *text, char *contraction, int *positions,
                         int * const allPositions, int contLength, int charPerWord, int leftToUse )
{
    long long int contractionCount = 0;
    char nextLetter = *contraction;
    if( !nextLetter )
    {
        printAbbreviation( start, allPositions, contLength );
        return 1;
    }
//    printf( "%s, %s\n", text, contraction );
    while( *text )
    {
        if( isspace(*text) ) leftToUse = charPerWord;
        if( *text == nextLetter && leftToUse )
        {
            *positions = text - start;
            contractionCount += findContractions( start, text + 1, contraction + 1,
                         positions + 1, allPositions, contLength, charPerWord, leftToUse - 1 );
        }
        text++;
    }
    return contractionCount;
}

long long int countContractions( char *text, char *contraction, int charPerWord, int leftToUse )
{
    long long int contractionCount = 0;
    char nextLetter = *contraction;
    if( !nextLetter ) return 1;
//    printf( "%s, %s\n", text, contraction );
    while( *text )
    {
        if( isspace(*text) ) leftToUse = charPerWord;
        if( *text == nextLetter && leftToUse )
        {
            contractionCount += countContractions( text + 1, contraction + 1,
                                                    charPerWord, leftToUse - 1 );
        }
        text++;
    }
    return contractionCount;
}


int main()
{
    printf("Zkratka:\n");
    char *contraction = NULL;
    size_t contractionBuffer;
    ssize_t ret = getline( &contraction, &contractionBuffer, stdin );
    if( ret <= 1 || contraction[ ret - 1 ] != '\n' )
    {
        fail( contraction );
    }
    contraction[ ret - 1 ] = '\0';
    if( !isUpper( contraction ) )
    {
        fail( contraction );
    }
    lowerString( contraction );

    int contractionLength = strlen( contraction );
    printf( "Problemy:\n" );

    char *text;
    int inWordChar;
    char operation;
    while ( parseLine( &operation, &inWordChar, &text ) )
    {
        if( !lowerString( text ) )
        {
            free( text );
            fail( contraction );
        }

        if( operation == '#' )
        {
            for( int limit = 1; limit <= inWordChar; limit++ )
            {
                long long int count = countContractions( text, contraction, limit, limit );
                printf( "> limit %d: %lld\n", limit, count );
            }
        }
        if( operation == '?' )
        {
            int *positions = (int *)malloc( contractionLength * sizeof( *positions ) );
            long long int count = findContractions( text, text, contraction, positions, positions,
                                            contractionLength, inWordChar, inWordChar );
            free( positions );
            printf( "> %lld\n", count );
        }

        free( text );
    }
    if( operation ) fail( contraction );
    free( contraction );

    return 0;
}
