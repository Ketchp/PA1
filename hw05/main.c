#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SMALLEST_CHAR '!'
#define LARGEST_CHAR '~'
#define PRINTABLE_CHARS (LARGEST_CHAR - SMALLEST_CHAR)
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
    char letter;
    int count;
};

struct link
{
    link_t *next, *previous;
    leaf_t *name;
    item_t *root;
};

struct item
{
    link_t *first_link;
    item_t *next, *previous;
    int count;
    int value;
};

int treeIdx(char c);
void add(leaf_t *stringTree, char *new_string, item_t *topSellers,
            item_t *last_item, int geItemCount, int count, item_t *list_end);
leaf_t *getLeaf( leaf_t *stringTree, char *new_string);

int main()
{
    int count;
    char operation, cariage_return;
    printf( "Pocet sledovanych:\n" );
    if( scanf( "%d%c", &count, &cariage_return ) != 2 || count <= 0 || cariage_return != '\n' )
    {
        printf( "Nespravny vstup.\n" );
    }
    printf( "Pozadavky:\n" );
    char new_item[ MAX_ITEM_LEN ];
    leaf_t *stringTree = (leaf_t *)calloc( 1, sizeof(*stringTree) );
    item_t *topSellers = (item_t *)calloc( count, sizeof(*topSellers) );
    topSellers[0].next = topSellers + 1;
    for(int i = 1; i < count-1; i++)
    {
        topSellers[i].next = topSellers + i + 1;
        topSellers[i].previous = topSellers + i - 1;
    }
    topSellers[count - 1].previous = topSellers + count - 2;
    item_t *last_item = topSellers;
    item_t *list_end = topSellers + count - 1;
    int geItemCount = 0;
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
            add(stringTree, new_item, topSellers,
                 last_item, geItemCount, count, list_end);
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

void add(leaf_t *stringTree, char *new_string, item_t *topSellers,
 item_t *last_item, int geItemCount, int count, item_t *list_end)
{
    leaf_t *item_leaf = getLeaf( stringTree, new_string );
    item_leaf->count++;
    if( last_item->value <= item_leaf->count )
    {
        link_t *reference = item_leaf->reference;
        if( reference )
        {
            shiftLink(reference);
        }
    }
}

leaf_t *getLeaf( leaf_t *stringTree, char *new_string)
{
    for(char *c = new_string; *new_string; new_string++)
    {
        if( !stringTree->kids[ treeIdx(*c) ] )
        {
            stringTree->kids[ treeIdx(*c) ] = 
                (leaf_t *)calloc( 1, sizeof(*stringTree) );
            stringTree->kids[ treeIdx(*c) ].root = stringTree;
        }
        stringTree = stringTree[ treeIdx(*c) ];
        stringTree->letter = *c;
    }
    return stringTree;
}

int treeIdx(char c)
{
    return c - SMALLEST_CHAR;
}

shiftLink(link_t *reference)
{
    if( reference->next )
    {
        reference->next->previous = reference->previous;
    }
    if( reference->previous )
    {
        reference->previous->next = reference->next;
    }
    if( !reference->previous && !reference->next &&
        ( !reference->root->previous ||
            reference->root->previous->value == item_leaf->count ) )
    {
        if( reference->root->next )
        {
            reference->root->next->previous = reference->previous;
        }
        if( reference->root->previous )
        {
            reference->root->previous->next = 
        }
    }

}
