#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>
#include <unistd.h>

int debug = 0;

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
    struct leaf *root;
    struct leaf *kids[ PRINTABLE_CHARS ];
    struct item *reference;
//    char word[ MAX_ITEM_LEN ];
    char letter;
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
 * @param noRankedItems items which are in scope
 * @param count size of scope
 * @param soldRanked count od items*solds in scope
 */
void add(leaf_t *stringTree, char *new_string,
          rank_t **firstRank, rank_t **edgeRank, rank_t **endRank,
          int *noRankedItems, int count, int *soldRanked );

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
                  rank_t **firstRank, rank_t **endRank );

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
void moveItem( item_t *reference, rank_t *destRank, rank_t **endRank, int *soldRanked );

/**
 * @brief Creates new item reference in appropiate rank
 * 
 * @param item item to be added
 */
void addItem( leaf_t *item, rank_t *destRank, int *noRankedItems, int *soldRanked );

/**
 * @brief check wheather edge rank is set correctly and moves it if neccesary
 */
void moveEdge( rank_t **edgeRank, int *noRankedItems, int count, int *soldRanked );

/**
 * @brief Print all ranked items
 */
void printRanks( rank_t *firstRank, rank_t *edgeRank );

/**
 * @brief Print number of sold ranked items 
 */
void printItemCount( int soldRanked );

/**
 * @brief string representing leaf
 */
void getString( leaf_t *leaf, char *buffer);

/**
 * @brief free's all items in rank
 */
void emptyRank( rank_t *rank );

/**
 * @brief free's all items from all ranks
 */
void emptyList( rank_t *rank );

/**
 * @brief recursively free whole tree
 */
void emptyTree( leaf_t *tree);

rank_t *gRank;
int main()
{
    int count;
    char operation, space, cariage_return;
    printf( "Pocet sledovanych:\n" );
    char *lineBuffer;
    size_t bufferSize;
    ssize_t ret;
    if( getline( &lineBuffer, &bufferSize, stdin ) <= 1 )
    {
        printf( "Nespravny vstup.\n" );
        free( lineBuffer );
        return 0;
    }

    if( sscanf( lineBuffer, "%d%c", &count, &cariage_return ) != 2 || count <= 0 || cariage_return != '\n')
    {
        printf( "Nespravny vstup.\n" );
        free( lineBuffer );
        return 0;
    }

    printf( "Pozadavky:\n" );
    
    leaf_t *stringTree = (leaf_t *)calloc( 1, sizeof(*stringTree) );
    rank_t *ranks = (rank_t *)calloc( count + 1, sizeof(*ranks) );
    gRank = ranks;
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
    int soldRanked = 0;
    while( (ret = getline( &lineBuffer, &bufferSize, stdin )) != -1 )
    {
        char newItem[ MAX_ITEM_LEN ];
        if( sscanf( lineBuffer, "%c%c%99s%c", &operation, &space, newItem, &cariage_return ) == 4 )
        {
            if( operation != '+' || space != ' ' || strlen(newItem) == 0 || cariage_return != '\n' )
            {
                printf( "Nespravny vstup.\n" );
                emptyTree( stringTree );
                emptyList( firstRank );
                free( lineBuffer );
                return 0;
            }
        }
        else if( sscanf( lineBuffer, "%c%c", &operation, &cariage_return ) == 2)
        {
            if( (operation != '?' && operation != '#') || cariage_return != '\n' )
            {
                printf( "Nespravny vstup.\n" );
                emptyTree( stringTree );
                emptyList( firstRank );
                free( lineBuffer );
                return 0;
            }
        }
        else
        {
            printf( "Nespravny vstup.\n" );
            emptyTree( stringTree );
            emptyList( firstRank );
            free( lineBuffer );
            return 0;
        }
        
        switch( operation )
        {
        case '+':
            add( stringTree, newItem,
                 &firstRank, &edgeRank, &endRank,
                 &noRankedItems, count, &soldRanked );

            break;

        case '#':
            printRanks( firstRank, edgeRank );
            printItemCount( soldRanked );
            break;

        case '?':
            printItemCount( soldRanked );
            break;

        }
    }
    emptyTree( stringTree );
    emptyList( firstRank );
    free( lineBuffer );
    return 0;
}

void add(leaf_t *stringTree, char *new_string,
         rank_t **firstRank, rank_t **edgeRank, rank_t **endRank,
         int *noRankedItems, int count, int *soldRanked)
{
    leaf_t *itemLeaf = getLeaf( stringTree, new_string );
    if(debug)
    {
        char buffer[ MAX_ITEM_LEN ];
        getString( itemLeaf, buffer);
        printf("leaf found: %s\n", buffer);
    }
    int sold = ++itemLeaf->sold;
    if( (*edgeRank)->sold <= sold )
    {
        item_t *reference = itemLeaf->reference;
        rank_t *sourceRank = reference ? reference->rank : *edgeRank;
        if(debug && reference) printf( "ref:%p\nnext:%p\nprev:%p\nleaf:%p\nrank:%p\n",
                                        (void *)reference, (void *)reference->next,
                                        (void *)reference->previous, (void *)reference->leaf,
                                        (void *)reference->rank );
        if(debug) printf("Get %ld %d\n", sourceRank - gRank, sold);
        rank_t *destinationRank = getRank( sourceRank, sold, 
                                            firstRank, endRank);
        if(debug) printf("->%ld\n", destinationRank - gRank);

        if( reference )
        {
            debug && printf("Moving to: %d %d\n", destinationRank->sold, destinationRank->noItems);
            moveItem( reference, destinationRank, endRank, soldRanked );
        }
        else
        {
            debug && printf("Adding\n");
            addItem( itemLeaf, destinationRank, noRankedItems, soldRanked );
        }

        debug && printf("Very\n");
        moveEdge( edgeRank, noRankedItems, count, soldRanked );
    }
    if(debug)
    {
        rank_t *pRank = *firstRank;
        printf("%d %d\n", *noRankedItems, *soldRanked);
        while( pRank )
        {
            printf( " %ld <-%ld-> %ld %d %d", 
                    pRank->higher ? pRank->higher - gRank : -1, 
                    pRank - gRank, 
                    pRank->lower ? pRank->lower - gRank : -1, 
                    pRank->sold, pRank->noItems );
            
            if(pRank == *firstRank) printf(" first");
            if(pRank == *edgeRank) printf(" edge");
            if(pRank == *endRank) printf(" end");
            printf("\n");

            item_t *itm = pRank->firstItem;
            while( itm )
            {
                char buffer[ MAX_ITEM_LEN ];
                getString( itm->leaf, buffer);
                printf("%s:%d ", buffer, itm->leaf->sold);
                assert( itm->rank == pRank );
                assert( itm->leaf->reference == itm );
                itm = itm->next;
            }
            printf("\n");
            pRank = pRank->lower;
        }
    }


}

leaf_t *getLeaf( leaf_t *stringTree, char *new_string)
{
    for(char *c = new_string; *c; c++)
    {
        if( !stringTree->kids[ treeIdx(*c) ] )
        {
            stringTree->kids[ treeIdx(*c) ] = 
                (leaf_t *)calloc( 1, sizeof(*stringTree) );
            
            stringTree->kids[ treeIdx(*c) ]->root = stringTree;
            stringTree->kids[ treeIdx(*c) ]->letter = *c;
        }
        stringTree = stringTree->kids[ treeIdx(*c) ];
    }
//    if( stringTree->word[0] == '\0' ) strcpy( stringTree->word, new_string );
    return stringTree;
}

int treeIdx(char c)
{
    return c - SMALLEST_CHAR;
}

rank_t *getRank( rank_t *reference, int newSold,
                  rank_t **firstRank, rank_t **endRank )
{
    rank_t *destination = reference;
    while( destination->higher && destination->higher->sold <= newSold )
    {
        destination = destination->higher;
    }
    if( destination->sold == newSold ) return destination;
    return createRank( destination, newSold, 
                        firstRank, endRank);

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

void moveItem( item_t *reference, rank_t *destRank, rank_t **endRank, int *soldRanked )
{
    rank_t *sourceRank = reference->rank;
    sourceRank->noItems--;
    ++*soldRanked;

    if( reference->previous ) reference->previous->next = reference->next;
    else sourceRank->firstItem = reference->next;
    if( !sourceRank->noItems )
    {
        assert( sourceRank->higher );
        sourceRank->higher->lower = sourceRank->lower;
        assert( sourceRank->lower );
        sourceRank->lower->higher = sourceRank->higher;

        sourceRank->higher = *endRank;
        sourceRank->lower = NULL;

        (*endRank)->lower = sourceRank;
        *endRank = sourceRank;
    }

    if( reference->next ) reference->next->previous = reference->previous;

    reference->rank = destRank;
    reference->previous = NULL;
    reference->next = destRank->firstItem;

    destRank->noItems++;
    if( destRank->firstItem ) destRank->firstItem->previous = reference;
    destRank->firstItem = reference;
}

void addItem( leaf_t *item, rank_t *destRank, int *noRankedItems, int *soldRanked )
{
    item_t *newItem = (item_t *)malloc( sizeof(*newItem) );
    newItem->previous = NULL;
    newItem->next = destRank->firstItem;
    newItem->leaf = item;
    newItem->rank = destRank;

    item->reference = newItem;

    if(destRank->firstItem) destRank->firstItem->previous = newItem;
    destRank->firstItem = newItem;
    destRank->noItems++;
    ++*noRankedItems;
    *soldRanked += item->sold;
}

void moveEdge( rank_t **edgeRank, int *noRankedItems, int count, int *soldRanked )
{
//    if( (*edgeRank)->lower && (*edgeRank)->lower->sold == 0 ) *edgeRank = (*edgeRank)->lower;
    while( *noRankedItems - (*edgeRank)->noItems >= count )
    {
        *noRankedItems -= (*edgeRank)->noItems;
        *soldRanked -= (*edgeRank)->noItems * (*edgeRank)->sold;
        *edgeRank = (*edgeRank)->higher;
    }
}

void printRank( rank_t *currRank, int rank )
{
    item_t *item = currRank->firstItem;
    char buffer[ MAX_ITEM_LEN ];
    while( item )
    {
        getString( item->leaf, buffer);
        if( currRank->noItems > 1)
        {
            printf( "%d.-%d. %s, %dx\n",
                     rank, rank + currRank->noItems - 1, 
                     buffer, currRank->sold );
        }
        else printf( "%d. %s, %dx\n", rank, buffer, currRank->sold );
        item = item->next;
    }
}

void printRanks( rank_t *firstRank, rank_t *edgeRank )
{
    int rank = 1;
    while( firstRank != edgeRank )
    {
        printRank( firstRank, rank );
        rank += firstRank->noItems;
        firstRank = firstRank->lower;
    }
    printRank( firstRank, rank );
}

void printItemCount( int soldRanked )
{
    printf( "Nejprodavanejsi zbozi: prodano %d kusu\n", soldRanked );
}

void getString( leaf_t *leaf, char *buffer)
{
    char *str = buffer;
    while ( leaf )
    {
        *str = leaf->letter;
        leaf = leaf->root;
        str++;
    }
    str -= 2;
    while ( buffer < str )
    {
        char temp = *buffer;
        *buffer = *str;
        *str = temp;
        buffer++;
        str--;
    }
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

void emptyList( rank_t *rank )
{
    while( rank )
    {
        emptyRank( rank );
        rank = rank->lower;
    }
}

void emptyTree( leaf_t *tree)
{
    if(!tree) return;
    for(int kid = 0; kid < PRINTABLE_CHARS; kid++)
    {
        emptyTree( tree->kids[kid] );
    }
    free( tree );
}
