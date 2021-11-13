#include <stdio.h>
#include <stdlib.h>

#define PRINTABLE_CHARS ('~' - '!')
#define MAX_ITEM_LEN 100
#define PARSE_STRING "%99s"

typedef struct leaf leaf_t;
typedef struct link link_t;
typedef struct item item_t;


struct leaf
{
    leaf_t *root;
    leaf_t *kids[ PRINTABLE_CHARS ];
    link_t *reference;
    char leaf_name;
};

struct link
{
    link_t *next, *previous;
    leaf_t *name;
};

struct item
{
    link_t *first, *last;
    item_t *next, *previous;
    int count;
};

int main()
{
    int count;
    char operation, cariage_return;
    printf( "Pocet sledovanych:\n" );
    if( scanf( "%d%c", &count, &cariage_return ) != 2 || count <= 0 || cariage_return != '\n' )
    {
        printf( "Nespravny vstup.\n" );
    }
    item_t items[ count ];
    printf( "Pozadavky:\n" );
    char new_item[ MAX_ITEM_LEN ];
    while( scanf( "%c", &operation ) != EOF )
    {
        switch( operation )
        {
        case '+':
            if( scanf( " %99s%c", new_item, &cariage_return ) != 2 || cariage_return != '\n' )
            {
                printf( "Nespravny vstup.\n" );
                return 0;
            }
            //todo
            break;

        case '#':
            if( scanf( "%c", &cariage_return ) != 1 || cariage_return != '\n' )
            {
                printf( "Nespravny vstup.\n" );
                return 0;
            }
            break;

        case '?':
            if( scanf( "%c", &cariage_return ) != 1 || cariage_return != '\n' )
            {
                printf( "Nespravny vstup.\n" );
                return 0;
            }
            break;

        default:
            printf( "Nespravny vstup.\n" );
            return 0;
        }
    }
    return 0;
}
