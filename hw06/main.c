#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>
#include <unistd.h>

#define LARGEST_CHAR 0x7f

typedef struct leaf {
    struct leaf *kids[ LARGEST_CHAR ];
    int count;
} leaf_t;

typedef struct stringList {
    int count, capacity, length;
    char **strings;
} stringList_t;

int findAll( leaf_t *root, stringList_t *strings, char *buffer, int maxLen, int depth );

void addString( stringList_t *strings, char *buffer );

void findSubstring( char **array, int maxRows, int maxColumns,
                   leaf_t *root, int *maxLen );

void searchFrom( char **array, int maxRows, int maxColumns,
            leaf_t *root, int x, int y, int *maxLen );

void insertString( char **array, int maxRows, int maxColumns, leaf_t *root,
              int x, int y, int dx, int dy, int *maxLen );

void freeArray( char **array, int rows);

void freeTree( leaf_t *root );

void freeString( stringList_t strings );

int main()
{
    int maxRows = 2, maxColumns, rows = 0, columns = 0;
    char **array = (char **)calloc( maxRows, sizeof(*array) );

    printf( "Hlavolam:\n" );
    size_t rowBuffer = 0;
    maxColumns = getline( array + rows, &rowBuffer, stdin );
    rows++;

    while( ( columns = getline( array + rows, &rowBuffer, stdin ) ) > 0 )
    {
        rows++;
        if(rows == maxRows)
        {
            maxRows <<= 1;
            array = (char **)realloc( array, maxRows * sizeof(*array) );
        }
        
        if( maxColumns != columns)
        {
            freeArray( array, rows - 1 );
            printf( "Nespravny vstup.\n" );
            return 0;
        }
        array[rows] = NULL;
    }

    if( array[ rows - 1 ][ maxColumns - 1 ] != '\n' || rows != maxColumns - 1)
    {
        freeArray( array, rows );

        printf( "Nespravny vstup.\n" );
        return 0;
    }
    
    maxColumns--;
    maxRows = rows;

    leaf_t *root = (leaf_t *)calloc( 1, sizeof(*root) );
    
    int maxLen = 0;
    findSubstring( array, maxRows, maxColumns, root, &maxLen );

    stringList_t strings = { 0, 1, maxLen + 1, (char **)calloc(1, sizeof(char *))};

    char *buffer = (char *)calloc( maxLen + 1, sizeof(*buffer) );
    findAll( root, &strings, buffer, maxLen, 0 );

    int found = strings.count;
    if( found )
    {
        printf("Nejdelsi opakujici se slova:\n");
        for(int i = 0; i < found; i++)
        {
            printf("%s\n", strings.strings[i]);
        }
    }
    else printf( "Zadne opakujici se slovo.\n" );

    freeString( strings );
    free( buffer );
    freeTree( root );
    freeArray( array, maxRows );
    return 0;
}

int findAll( leaf_t *root, stringList_t *strings, char *buffer, int maxLen, int depth )
{
    int count = 0;
    if( root == NULL ) return 0;
//    printf("in <%s> %d, %d/%d\n", buffer, root->count, depth, maxLen);
    if( depth < maxLen )
    {
        for (int i = 1; i < LARGEST_CHAR; i++)
        {
            buffer[ depth ] = i;
            count += findAll( root->kids[i], strings, buffer, maxLen, depth+1);
        }
        buffer[ depth ] = '\0';
        return count;
    }
    if(depth == maxLen && root->count > 1)
    {
        addString( strings, buffer );
        return 1;
    }
    return 0;
}

void addString( stringList_t *strings, char *buffer )
{
//    printf("Adding <%s> to ", buffer );
    if( strings->count <= strings->capacity )
    {
        strings->capacity <<= 1;
        strings->strings = (char **)realloc( strings->strings, strings->capacity * sizeof(*(strings->strings)) );
    }
    strings->strings[ strings->count ] = (char *)malloc( strings->length * sizeof( char ) );
    strcpy( strings->strings[ strings->count ], buffer );
    strings->count++;
}

void findSubstring( char **array, int maxRows, int maxColumns,
                   leaf_t *root, int *maxLen )
{
    for( int row = 0; row < maxRows; row++ )
    {
        for( int column = 0; column < maxColumns; column++ )
        {
            searchFrom( array, maxRows, maxColumns, root, column, row, maxLen );
        }
    }
}

void searchFrom( char **array, int maxRows, int maxColumns,
            leaf_t *root, int x, int y, int *maxLen )
{
//    printf("searching from (%d,%d):%c\n", x, y, array[y][x]);
    int current = (int)array[y][x];
    if( root->kids[ current ] == NULL )
    {
        root->kids[ current ] = (leaf_t *)calloc( 1, sizeof(*root) );
    }
    root = root->kids[ current ];
    root->count++;
    if(root->count == 2) *maxLen = ( *maxLen > 1 ) ? *maxLen : 1;

    for( int dx = -1; dx <= 1; dx++ )
    {
        for( int dy = -1; dy <= 1; dy++ )
        {
            if( dx == 0 && dy == 0 ) continue;
            insertString( array, maxRows, maxColumns, root, x+dx, y+dy, dx, dy, maxLen );
        }
    }
}

void insertString( char **array, int maxRows, int maxColumns, leaf_t *root,
              int x, int y, int dx, int dy, int *maxLen )
{
    int depth = 1;
    while( 0 <= y && y < maxRows && 0 <= x && x < maxColumns )
    {
        int current = (int)array[y][x];
        if( root->kids[ current ] == NULL ) root->kids[current] = (leaf_t *)calloc( 1, sizeof(*root) );
        root = root->kids[ current ];
        root->count++;
        depth++;
        if( root->count == 2 ) *maxLen = ( depth > *maxLen ) ? depth : *maxLen;
        x += dx;
        y += dy;
    }
}

void freeArray( char **array, int rows)
{
    for (int i = 0; i <= rows; i++)
    {
        free( array[i] );
    }
    free( array );
}

void freeTree( leaf_t *root )
{
    if ( root == NULL ) return;
    for (int i = 0; i < LARGEST_CHAR; i++)
    {
        freeTree( root->kids[i] );
    }
    free( root );
}

void freeString( stringList_t strings )
{
    for(int i = 0; i < strings.count; i++)
    {
        free( strings.strings[i] );
    }
    free( strings.strings );
}
