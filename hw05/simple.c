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
    int sold;
    char letter;
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
 * @brief translation from printable ASCII to indexes
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
 * @param top ptr to list of most selling items
 * @param bottom ptr to list of least selling items
 * @param noRankedItems items which are in scope
 * @param soldRanked count od items*solds in scope
 * @param count size of scope
 */
void add(leaf_t *stringTree, char *new_string,
          rank_t **top, rank_t **bottom,
          int *noRankedItems, int *soldRanked, int count );

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
rank_t *getRank( rank_t *reference, int newSold, rank_t **top );

/**
 * @brief Create a Rank object
 * 
 * @param position rank lower than new rank
 * @param sold sold count of newly created rank
 * @return rank_t* created rank
 */
rank_t *createRank( rank_t *position, int newSold, rank_t **top );

/**
 * @brief moves referenced item to destRank (updates edgeRank if necessary)
 */
void moveItem( item_t *reference, rank_t *destRank, rank_t **bottom, int *soldRanked );

/**
 * @brief delete and free rank from linked list
 * 
 * @param bottom updated if rank == *bottom
 */
void deleteRank( rank_t *rank, rank_t **bottom );

/**
 * @brief Creates new item reference in appropiate rank
 * 
 * @param item item to be added
 */
void addItem( leaf_t *item, rank_t *destRank, int *noRankedItems, int *soldRanked );

/**
 * @brief check wheather edge rank is set correctly and moves it if neccesary
 */
void moveBottom( rank_t **bottom, int *noRankedItems, int *soldRanked, int count );

/**
 * @brief Print all ranked items
 */
void printRanks( rank_t *top, int count );

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

void printDebug( rank_t **top, int *noRankedItems, int *soldRanked )
{
    if(debug)
    {
        rank_t *pRank = *top;
        printf("%d %d\n", *noRankedItems, *soldRanked);
        int rankC = 0;
        while( pRank )
        {
            printf( "%d. %p<=%p=>%p: %d %d %p\n", rankC++,
                    (void *)pRank->higher, (void *)pRank, (void *)pRank->lower,
                    pRank->sold, pRank->noItems, (void *)pRank->firstItem );

            item_t *itm = pRank->firstItem;
            while( itm )
            {
                char buffer[ MAX_ITEM_LEN ];
                getString( itm->leaf, buffer);
                printf("%s:%d ", buffer, itm->leaf->sold);
                if( itm->rank != pRank )
                    printf("WARNING1: %p != %p", (void *)itm->rank, (void *)pRank);
                if( itm->leaf->reference != itm )
                    printf("WARNING2: %p != %p", (void *)itm->leaf->reference, (void *)itm);
                itm = itm->next;
            }
            printf("\n");
            pRank = pRank->lower;
        }
    }

}

rank_t *gRank;
int main()
{
    int count;
    char operation, space, cariage_return;
    printf( "Pocet sledovanych:\n" );
    char *lineBuffer = NULL;
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
    rank_t *top, *bottom;
    top = bottom = (rank_t *)calloc( 1, sizeof(*top) );
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
                emptyList( top );
                emptyTree( stringTree );
                free( lineBuffer );
                return 0;
            }
        }
        else if( sscanf( lineBuffer, "%c%c", &operation, &cariage_return ) == 2)
        {
            if( (operation != '?' && operation != '#') || cariage_return != '\n' )
            {
                printf( "Nespravny vstup.\n" );
                emptyList( top );
                emptyTree( stringTree );
                free( lineBuffer );
                return 0;
            }
        }
        else
        {
            printf( "Nespravny vstup.\n" );
            emptyList( top );
            emptyTree( stringTree );
            free( lineBuffer );
            return 0;
        }
        
        switch( operation )
        {
        case '+':
            add( stringTree, newItem,
                 &top, &bottom,
                 &noRankedItems, &soldRanked, count );
            break;

        case '#':
            printRanks( top, count );
            printItemCount( soldRanked );
            break;

        case '?':
            printItemCount( soldRanked );
            break;

        }
    }
    emptyList( top );
    emptyTree( stringTree );
    free( lineBuffer );
    return 0;
}

void add(leaf_t *stringTree, char *new_string,
          rank_t **top, rank_t **bottom,
          int *noRankedItems, int *soldRanked, int count )
{
    leaf_t *itemLeaf = getLeaf( stringTree, new_string );
    if(debug)
    {
        char buffer[ MAX_ITEM_LEN ];
        getString( itemLeaf, buffer);
        printf("leaf found: %s,%d\n", buffer, itemLeaf->sold);
        printf(itemLeaf->reference ? "with reference\n" : "without ref\n");
    }
    int sold = ++itemLeaf->sold;
    if( (*bottom)->sold <= sold )
    {
        item_t *reference = itemLeaf->reference;
        rank_t *sourceRank = reference ? reference->rank : *bottom;
        if(debug) printf("crash\n");

        rank_t *destinationRank = getRank( sourceRank, sold, top );
        if(debug) printf("crash\n");

        if( reference )
        {
            moveItem( reference, destinationRank, bottom, soldRanked );
            if(debug) printf("crash\n");
        }
        else
        {
            addItem( itemLeaf, destinationRank, noRankedItems, soldRanked );
        }

        moveBottom( bottom, noRankedItems, soldRanked, count );
    }
    if(debug) printDebug(top, noRankedItems, soldRanked);

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

rank_t *getRank( rank_t *reference, int newSold, rank_t **top )
{
    rank_t *destination = reference;
    while( destination->higher && destination->higher->sold <= newSold )
    {
        destination = destination->higher;
    }
    if( destination->sold == newSold ) return destination;
    return createRank( destination, newSold, top );

}

rank_t *createRank( rank_t *position, int newSold, rank_t **top )
{
    rank_t *lowerRank = position;
    rank_t *higherRank = position->higher;
    rank_t *newRank = (rank_t *)calloc( 1, sizeof(*newRank) );
    if(debug) printf("Creating rank: %d:%p\n",
        newSold, (void *)newRank);

    newRank->higher = higherRank;
    newRank->lower = lowerRank;
    newRank->sold = newSold;

    lowerRank->higher = newRank;
    if( higherRank ) higherRank->lower = newRank;
    else *top = newRank;

    return newRank;
}

void moveItem( item_t *reference, rank_t *destRank, rank_t **bottom, int *soldRanked )
{
    if(debug) printf("Moving item %c:%p to %p\n",
     reference->leaf->letter, (void *)reference, (void*)destRank );

    rank_t *sourceRank = reference->rank;
    sourceRank->noItems--;
    if( reference->next ) reference->next->previous = reference->previous;
    if( reference->previous ) reference->previous->next = reference->next;
    else sourceRank->firstItem = reference->next;

    if( sourceRank->noItems == 0) deleteRank( sourceRank, bottom );

    reference->rank = destRank;
    reference->previous = NULL;
    reference->next = destRank->firstItem;

    destRank->noItems++;
    if( destRank->firstItem ) destRank->firstItem->previous = reference;
    destRank->firstItem = reference;

    ++*soldRanked;
}

void deleteRank( rank_t *rank, rank_t **bottom )
{
    if(debug) printf( "Deleting %p\n", (void *)rank );

    emptyRank( rank );
    if( rank->higher ) rank->higher->lower = rank->lower;
    if( rank->lower ) rank->lower->higher = rank->higher;
    else
    {
        assert( rank == *bottom );
        *bottom = rank->higher;
    }

    free( rank );
}

void addItem( leaf_t *item, rank_t *destRank, int *noRankedItems, int *soldRanked )
{
    if(debug) printf("Adding item %c to %p\n", item->letter, (void*)destRank );

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

void moveBottom( rank_t **bottom, int *noRankedItems, int *soldRanked, int count )
{
    while( *noRankedItems - (*bottom)->noItems >= count )
    {
        if(debug) printf("Bottom moved\n");
        *noRankedItems -= (*bottom)->noItems;
        *soldRanked -= (*bottom)->noItems * (*bottom)->sold;
        deleteRank( *bottom, bottom);
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

void printRanks( rank_t *top, int count )
{
    int rank = 1;
    while( top && rank <= count )
    {
        printRank( top, rank );
        rank += top->noItems;
        top = top->lower;
    }
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
    int a = -1, b = -1;
    if(debug) printDebug( &rank, &a, &b);
    
    while( rank )
    {
        rank_t *nextRank = rank->lower;
        emptyRank( rank );
        free( rank );
        rank = nextRank;
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
