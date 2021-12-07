#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct array
{
    int *array;
    int capacity;
    int size;
} array_t;

typedef struct tree
{
    struct tree *left, *right;
    int key, value, height;
} tree_t;

typedef struct cache
{
    tree_t *values;
} cache_t;

void walkForest( tree_t *root, int top );

void uniq( array_t *arr )
{
    return;
    int wr = 0, read = 0;
    while( read < arr->size )
    {
        arr->array[ wr ] = arr->array[ read ];
    }
}

void initArray( array_t *arr )
{
    arr->array = NULL;
    arr->capacity = 0;
    arr->size = 0;
}

void push( array_t *arr, int x )
{
    if( arr->size == arr->capacity )
    {
        arr->capacity = 2 * arr->capacity + 1;
        arr->array = (int *)realloc( arr->array, arr->capacity * sizeof( *(arr->array) ) );
    }
    arr->array[ arr->size++ ] = x;
}

int getLength( array_t *arr )
{
    return arr->size;
}

int getItem( array_t *arr, int idx )
{
    return arr->array[ idx ];
}

void freeArray( array_t *arr )
{
    free( arr->array );
    arr->array = NULL;
    arr->capacity = 0;
    arr->size = 0;
}

int compInt( const int *a, const int *b )
{
    return (*a < *b) - (*a > *b);
}

int setHeight( tree_t *root )
{
    if( !root ) return 0;
    int heightL = 0, heightR = 0;
    if( root->left ) heightL = root->left->height;
    if( root->right ) heightR = root->right->height;
    root->height = 1 + ( heightR > heightL ?
                                   heightR :
                                   heightL );
    return heightL - heightR;
}

/*
   A         B
  / \         \
 B   D  -->    A
  \           / \
   C         C   D
*/
tree_t *rotateRight( tree_t *nodeA )
{
    tree_t *nodeB = nodeA->left;
    tree_t *nodeC = nodeB->right;
    nodeB->right = nodeA;
    nodeA->left = nodeC;
    setHeight( nodeA );
    setHeight( nodeB );
    return nodeB;
}

/*
   A            B
  / \          /
 D   B  -->   A
    /        / \
   C        D   C
*/
tree_t *rotateLeft( tree_t *nodeA )
{
    tree_t *nodeB = nodeA->right;
    tree_t *nodeC = nodeB->left;
    nodeB->left = nodeA;
    nodeA->right = nodeC;
    setHeight( nodeA );
    setHeight( nodeB );
    return nodeB;
}

tree_t *createNode( int key, int value )
{
    tree_t *node = (tree_t *)malloc( sizeof( *node ) );
    node->left = node->right = NULL;
    node->key = key;
    node->value = value;
    node->height = 1;
    return node;
}

tree_t *insertItem( tree_t *root, const int key, const int value )
{
    if( !root ) return createNode( key, value );
    assert( key != root->key );
    if( key > root->key ) root->left = insertItem( root->left, key, value);
    if( key < root->key ) root->right = insertItem( root->right, key, value );

    int balance = setHeight( root );
//    if(debug) printf( "B:%d\n", balance );
    // right left
    if( balance < -1 && root->right->key < key )
    {
//        if(debug) printf( "RL %d:%d\n", root->key, root->value );
        root->right = rotateRight( root->right );
//        if(debug) walkForest( root, 1 );
        return rotateLeft( root );
    }

    // right right
    if( balance < -1 && root->right->key > key )
    {
//        if(debug) printf( "RR %d:%d\n", root->key, root->value );
        return rotateLeft( root );
    }

    // left left
    if( balance > 1 && root->left->key < key )
    {
//        if(debug) printf( "LL %d:%d\n", root->key, root->value );
        return rotateRight( root );
    }

    // left right
    if( balance > 1 && root->left->key > key )
    {
//        if(debug) printf( "LR %d:%d\n", root->key, root->value );
        root->left = rotateLeft( root->left );
        return rotateRight( root );
    }

    return root;
}

/**
 * @brief Get value in position key
 * 
 * @return int value of key ( -1 if key does not exist )
 */
int getValue( tree_t *root, int key )
{
    if( !root ) return -1;
    if( key > root->key ) return getValue( root->left, key );
    if( key < root->key ) return getValue( root->right, key );
    return root->value;
}

void cutForest( tree_t *root )
{
    if( !root ) return;
    cutForest( root->left );
    cutForest( root->right );
    free( root );
}

void initCache( cache_t *cache )
{
    cache->values = NULL;
}

int getCache( cache_t *cache, int key )
{
    return getValue( cache->values, key );
}
void setCache( cache_t *cache, int key, int value )
{
    cache->values = insertItem( cache->values, key, value );
}

void deleteCache( cache_t *cache )
{
    cutForest( cache->values );
    cache->values = NULL;
}

int assemble( int amount, array_t *coins, cache_t *cache, int depth )
{
//    for(int i = 0; i < depth; i++) printf(">");
//    printf( "%d", amount );
    int cached = getCache( cache, amount );
    if( cached != -1 ) return cached;
    if( !amount ) return 0;
    if( amount < 0 ) return __INT_MAX__;
    int coinCount = getLength( coins );
    int min = __INT_MAX__;
    for( int idx = 0; idx < coinCount; idx++ )
    {
        int coin = getItem( coins, idx );
        int count = assemble( amount - coin, coins, cache, depth + 1 );
        if( count == __INT_MAX__ ) continue;
        min = min > count + 1 ? count + 1 : min;
    }
    setCache( cache, amount, min );
//    printf( "%d:%d\n", amount, min );
    return min;
}

int main()
{
    printf( "Mince:\n" );
    array_t coins;
    initArray( &coins );
    int coin = 1;
    while( scanf("%d ", &coin) == 1 )
    {
        if( coin <= 0 ) break;
        push( &coins, coin );
    }
    if( coin || !getLength( &coins ) )
    {
        printf( "Nespravny vstup.\n" );
        freeArray( &coins );
        return EXIT_SUCCESS;
    }

    qsort( coins.array, getLength( &coins ), sizeof( *coins.array ), ( int(*)( const void *, const void *) )compInt );
    uniq( &coins );

/*    
    for( int i = 0; i < getLength( &coins ); i++ )
    {
        printf("%d ", getItem( &coins, i ) );
    }
    printf( "\n" );
*/

    printf( "Castky:\n" );
    cache_t cache;
    initCache( &cache );
    int amount;
    while( scanf(" %d ", &amount) == 1 )
    {
        if( amount < 0 )
        {
            freeArray( &coins );
            deleteCache( &cache );
            printf("Nespravny vstup.\n");
            return EXIT_SUCCESS;
        }
        int count = assemble( amount, &coins, &cache, 0 );

        if( count == __INT_MAX__ ) printf( "= nema reseni\n" );
        else                       printf( "= %d\n", count );
    }
    freeArray( &coins );
    deleteCache( &cache );
    if( feof( stdin ) ) return EXIT_SUCCESS;
    printf("Nespravny vstup.\n");
    return EXIT_SUCCESS;
}

void walkForest( tree_t *root, int top )
{
    if( top ) printf( "---Forest---\n" );
    if( !root ) return;
    printf( "%d> %d:%d\n", root->height, root->key, root->value );
    walkForest( root->left, 0 );
    walkForest( root->right, 0 );
}

