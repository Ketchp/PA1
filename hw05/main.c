#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SMALLEST_CHAR '!'
#define LARGEST_CHAR '~'
#define PRINTABLE_CHARS (LARGEST_CHAR - SMALLEST_CHAR)
#define MAX_ITEM_LEN 100
#define PARSE_STRING "%99s"

/**
 * @brief type for storing items{name, count}
 */
typedef struct leaf
{
//    leaf_t *root;
    struct leaf *kids[ PRINTABLE_CHARS ];
    struct item *reference;
    char word[ MAX_ITEM_LEN ];
    int sold;
} leaf_t;

/**
 * @brief linked list of items with references on rank and item tree
 */
typedef struct item
{
    struct item *next, *previous;
    struct leaf *leaf;
    struct rank *rank;
} item_t;

/**
 * @brief type for storing items with same rank
 */
typedef struct rank
{
    struct item *firstItem;
    struct rank *higher, *lower;
    int noItems; //number of items of this rank
    int sold; //number of sold items of one kind
} rank_t;

/**
 * @brief translation from prantable ASCII to indexes
 * 
 * @param c printable ASCII
 * @return int index
 */
int treeIdx( char c );

/**
 * @brief Add item record to multiple data structures
 * 
 * @param stringTree Tree storing all items, names and counts
 * @param new_string name of item to be added
 * @param firstRank ptr to list of most selling items 
 * @param edgeRank ptr to list of least selling items
 * @param endRank ptr to last rank which may not be updated/displyed
 *                because is beyond scope 
 * @param noRankedItems count of items which are in scope
 * @param count size of scope
 */
void add(leaf_t *stringTree, char *new_string,
          rank_t **firstRank, rank_t **edgeRank, rank_t **endRank,
          int *noRankedItems, int count );

/**
 * @brief Get/Create the Leaf object which stores item
 * 
 * @param stringTree Tree storing all items
 * @param new_string name of item
 * @return leaf_t* ptr to item in stringTree
 */
leaf_t *getLeaf( leaf_t *stringTree, char *new_string );

/**
 * @brief Get/Create the Rank object with items of newSold
 * 
 * @param reference rank to start search
 * @param newSold required sold count
 */
rank_t *getRank( rank_t *reference, int newSold,
                  rank_t **firstRank, rank_t **edgeRank, rank_t **endRank );

/**
 * @brief Create a Rank object
 * 
 * @param position rank lower than new rank
 * @param sold sold count of newly created rank
 * @return rank_t* created rank
 */
rank_t *createRank( rank_t *position, int newSold, 
                     rank_t **firstRank, rank_t **endRank );

/**
 * @brief moves referenced item to destRank (updates edgeRank if necessary)
 */
void moveItem( item_t *reference, rank_t *destRank,
                rank_t **edgeRank,
                int *noRankedItems, int count );

/**
 * @brief Creates new item reference in appropiate rank
 * 
 * @param item item to be added
 */
void addItem( leaf_t *item,
               rank_t **firstRank, rank_t **edgeRank, rank_t **endRank,
               int *noRankedItems, int count );

/**
 * @brief free's all items in rank
 */
void emptyRank( rank_t *rank );

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
    
    leaf_t *stringTree = (leaf_t *)calloc( 1, sizeof(*stringTree) );
    rank_t *ranks = (rank_t *)calloc( count + 1, sizeof(*ranks) );
    ranks[0].lower = ranks + 1;
    for(int i = 1; i < count; i++)
    {
        ranks[i].lower =  ranks + i + 1;
        ranks[i].higher = ranks + i - 1;
    }
    ranks[ count ].higher = ranks + count - 1;
    rank_t *firstRank = ranks;
    rank_t *edgeRank = ranks;
    rank_t *endRank = ranks + count;
    int noRankedItems = 0;
    while( scanf( "%c", &operation ) != EOF )
    {
        char new_item[ MAX_ITEM_LEN ];
        switch( operation )
        {
        case '+':
            if( scanf( " %99s%c", new_item, &cariage_return ) != 2 || cariage_return != '\n' )
            {
                printf( "Nespravny vstup.\n" );
                return 0;
            }
            add( stringTree, new_item,
                 &firstRank, &edgeRank, &endRank,
                 &noRankedItems, count );

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

void add(leaf_t *stringTree, char *new_string,
         rank_t **firstRank, rank_t **edgeRank, rank_t **endRank,
         int *noRankedItems, int count)
{
    leaf_t *itemLeaf = getLeaf( stringTree, new_string );
    int sold = ++itemLeaf->sold;
    if( (*edgeRank)->sold <= sold )
    {
        item_t *reference = itemLeaf->reference;
        if( reference )
        {
            rank_t *destinationRank = getRank( reference->rank, sold, 
                                                firstRank, edgeRank, endRank);

            moveItem( reference, destinationRank, edgeRank, noRankedItems, count );
        }
        else //no reference -> need to create new, surely(!!verify!!) in edgeRank
        {
            addItem( itemLeaf, firstRank, edgeRank, endRank, 
                      noRankedItems, count );
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
//            stringTree->kids[ treeIdx(*c) ].root = stringTree;
        }
        stringTree = stringTree->kids[ treeIdx(*c) ];
    }
    if( stringTree->word[0] == '\0' ) strcpy( stringTree->word, new_string );
    return stringTree;
}

int treeIdx(char c)
{
    return c - SMALLEST_CHAR;
}

/* reference exists and there are x posibilities
 *  1. higher rank has right sold count
 *  2. higher rank does not have right sold count / does not exist
 *               { higher sold count than needed} / {if rank is already first}
 * 
 *  1. - - - - - - - - - - - - - - - - - - - - - - - - - - \
 *  2. -> delete endRank{free all Items} -> create new rank -> shift item to previous
 * 
 * if( shifting from edgeRank )
 * {
 *   if(noRankedItems - edgeRank.count >= count) { shift edgeRank and update noRankedItems }
 * }
 */
rank_t *getRank( rank_t *reference, int newSold,
                  rank_t **firstRank, rank_t **edgeRank, rank_t **endRank )
{
    rank_t *destination = reference;
    while( destination->higher && destination->higher->sold <= newSold )
    {
        destination = destination->higher;
    }
    if( destination->sold == newSold ) return destination;

}

rank_t *createRank( rank_t *position, int newSold, 
                     rank_t **firstRank, rank_t **endRank )
{
    emptyRank( *endRank );
    rank_t *newEndRank = (*endRank)->higher;
    rank_t *twoHigherRank = position->higher;

    (*endRank)->lower = position;
    (*endRank)->higher = twoHigherRank;
    (*endRank)->sold = newSold;

    position->higher = *endRank;
    if( twoHigherRank ) twoHigherRank->lower = *endRank;
    else *firstRank = *endRank;

    *endRank = newEndRank;
    (*endRank)->lower = NULL;
    return position->higher;
}

void emptyRank( rank_t *rank )
{
    item_t *item = rank->firstItem;
    while( item )
    {
        item->leaf->reference = NULL;
        item_t *nextItem = item->next;
        free( item );
        item = nextItem;
    }
    rank->firstItem = NULL;
    rank->noItems = 0;
}
