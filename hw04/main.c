#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ID     100000
#define MAX_QUERY 1000000
#define INTERVAL   131072    // 2^17 > MAX_ID

typedef struct linkList
{
    int value;
    struct linkList *next;
} link_t;

typedef struct intervalTree
{
    int from, to;
    link_t *data;
} tree_t;

typedef struct vector
{
    link_t **list;
    int size, capacity;
} vector_t;

int add( int id, int *list, tree_t *tree, int count );
int query( int from, int to, tree_t *tree );

link_t *insertLink( link_t *list, int num );
void getIntervals( int from, int to, tree_t *tree, int index, vector_t *intervals );
void pushVector( vector_t *vector, link_t *ptr );
int linkLen( link_t *list );
void linkFree( link_t *list);
void freeAll(tree_t *tree);

#ifndef __PROGTEST__
#include <assert.h>
#include <unistd.h>

void printlink( link_t *list)
{
    fprintf( stderr, "Linked list:    value ->   next\n");
    while (list != NULL)
    {
        fprintf( stderr, "%p:   %d   -> %p\n", (void *)list, list->value, (void *)list->next);
        list = list->next;
//        sleep(1);
    }
    fprintf( stderr, "end\n" );
}

void test()
{

    //insert link / linkLen tests
    {
    link_t *linkedList = NULL;
    assert( linkLen( linkedList ) == 0 );

    linkedList = insertLink( linkedList, 5);
    assert( linkLen( linkedList ) == 1 );

    link_t *newHandle = insertLink( linkedList, 6);
    assert( linkLen( linkedList ) == 2 );

    newHandle = insertLink( linkedList, 3);

    assert( linkLen( linkedList ) == 3 );
    assert( linkLen( newHandle ) == 3 );

    newHandle = insertLink( linkedList, 4);
    assert( linkLen( linkedList ) == 4 );
    assert( linkLen( newHandle ) == 3 );

    newHandle = insertLink( linkedList, 4);
    assert( linkLen( linkedList ) == 4 );
    assert( linkLen( newHandle ) == 3 );

    assert( insertLink( linkedList, 8)->value == 8);
    assert( insertLink( linkedList, 7)->value == 7);
    assert( insertLink( linkedList, 6)->value == 6);
    assert( insertLink( linkedList, 0)->value == 0);
    assert( insertLink( linkedList, 1)->value == 1);
    assert( insertLink( linkedList, 2)->value == 2);
    assert( insertLink( linkedList, 5)->value == 5);
    assert( insertLink( linkedList, 4)->value == 4);
    assert( insertLink( linkedList, 8)->value == 8);
    assert( insertLink( linkedList, 9)->value == 9);
    assert( insertLink( linkedList, 12)->value == 12);
    assert( insertLink( linkedList, 11)->value == 11);
    assert( linkLen( linkedList ) == 12 );

    //printlink(linkedList);
    //printlink(newHandle);

    linkFree(linkedList);
    }

    //pushVector tests
    {
    vector_t intervals = { (link_t **)malloc(sizeof(link_t *)), 0, 1 };

    //data set
    link_t *list_0 = insertLink( NULL, 0);
    link_t *list_1 = insertLink( NULL, 1);
    link_t *list_2 = insertLink( NULL, 2);
    link_t *list_3 = insertLink( NULL, 3);
    link_t *list_4 = insertLink( NULL, 4);
    {
    insertLink( list_0, 8);
    insertLink( list_0, 5);

    insertLink( list_1, 9);
    insertLink( list_1, 10);
    insertLink( list_1, 12);
    insertLink( list_1, 11);

    insertLink( list_3, 2);
    insertLink( list_3, 1);

    insertLink( list_4, 3);
    insertLink( list_4, 5);
    insertLink( list_4, 2);
    insertLink( list_4, 6);
    insertLink( list_4, 1);
    insertLink( list_4, 7);
    }
    pushVector( &intervals, list_0);
    pushVector( &intervals, list_0);
    pushVector( &intervals, list_1);
    pushVector( &intervals, list_0);
    pushVector( &intervals, list_2);
    pushVector( &intervals, list_0);
    pushVector( &intervals, list_3);
    pushVector( &intervals, list_0);
    pushVector( &intervals, list_4);
    pushVector( &intervals, list_0);

    assert( intervals.list[0] == list_0);
    assert( intervals.list[1] == list_0);
    assert( intervals.list[2] == list_1);
    assert( intervals.list[3] == list_0);
    assert( intervals.list[4] == list_2);
    assert( intervals.list[5] == list_0);
    assert( intervals.list[6] == list_3);
    assert( intervals.list[7] == list_0);
    assert( intervals.list[8] == list_4);
    assert( intervals.list[9] == list_0);

    linkFree(list_0);
    linkFree(list_1);
    linkFree(list_2);
    linkFree(list_3);
    linkFree(list_4);
    free(intervals.list);
    //printf("%d %d\n", intervals.size, intervals.capacity);
    }


/*
    int add( int id, int *list, tree_t *tree, int count );
    int query( int from, int to, tree_t *tree );

    void getIntervals( int from, int to, tree_t *tree, int index, vector_t *intervals );
*/
}

#endif /* __PROGTEST__ */

int main()
{
    #ifndef __PROGTEST__
    
    test();
    
    #endif /* __PROGTEST__ */


    tree_t *tree = (tree_t *)calloc( 2*INTERVAL, sizeof(tree_t) );
    int visit_count[ MAX_ID ] = {0};
    printf( "Pozadavky:\n" );
    char operation, space, cariage_return;
    int new_id, ret, from, to, count = 0;
    while( scanf( "%c ", &operation ) != EOF )
    {
        switch( operation )
        {
        case '+':
            if( scanf( "%d%c", &new_id, &cariage_return ) != 2 || cariage_return != '\n' || 
                new_id < 0 || new_id >= MAX_ID || count == MAX_QUERY )
            {
                freeAll( tree );
                printf( "Nespravny vstup.\n" );
                return 0;
            }

            ret = add( new_id, visit_count , tree, count);
            count++;

            if( ret == 1 ) printf( "> prvni navsteva\n" );
            else printf( "> navsteva #%d\n", ret );

            break;

        case '?':
            if( scanf( "%d%c%d%c", &from, &space, &to, &operation ) != 4 || 
                operation != '\n' || space != ' ' || from < 0 || to >= count || to < from )
            {
                freeAll( tree );
                printf( "Nespravny vstup.\n" );
                return 0;
            }
            ret = query( from, to, tree );
            printf( "> %d / %d\n", ret, to - from + 1 );
            break;

        default:
            freeAll( tree );
            printf( "Nespravny vstup.\n" );
            return 0;
        }
    }

    freeAll( tree );
    return 0;
}

int add( int id, int *list, tree_t *tree, int count)
{
    int interval_start = count;
    int size = 1;
    count += INTERVAL;
    while(count)
    {
        if(tree[count].data == NULL)
        {
            tree[count].from = interval_start;
            tree[count].to = interval_start + size;
            tree[count].data = insertLink(tree[count].data, id);
        }
        insertLink(tree[count].data, id);
//        fprintf( stderr, "%d to %d:\n", tree[count].from, tree[count].to );
//        printlink(tree[count].data);
        count >>= 1;
        size <<=1;
    }
    list[id]++;
    return list[id];
}

int query(int from, int to, tree_t *tree)
{
    vector_t intervals = { (link_t **)malloc(sizeof(link_t *)), 0, 1 };
    getIntervals(from, to+1, tree, 1, &intervals);
//    fprintf( stderr, "%d\n", intervals.size);
    link_t *merger = insertLink( NULL, intervals.list[0]->value);
    for(int i = 0; i < intervals.size; i++)
    {
//        printlink(intervals.list[i]);
        link_t *handle = merger;
        link_t *newInterval = intervals.list[i];
        while(newInterval != NULL)
        {
            handle = insertLink( handle, newInterval->value );
            newInterval = newInterval->next;
        }
    }
//    printlink(merger);
    int length = linkLen( merger );
    linkFree( merger );
    free(intervals.list);

    return length;
}

link_t *insertLink( link_t *list, int num )
{
    if(list == NULL)
    {
        list = (link_t *)malloc( sizeof(link_t) );
        list->value = num;
        list->next = NULL;
        return list;
    }
    while( list->value < num && list->next != NULL ) list = list->next;
    if (list->value < num )
    {
        list = list->next = (link_t *)malloc( sizeof(link_t) );
        list->value = num;
        list->next = NULL;
        return list;        
    }
    if (list->value == num ) return list;

    link_t *handle = list->next;
    list->next = (link_t *)malloc( sizeof(link_t) );
    list->next->value = list->value;
    list->next->next = handle;
    list->value = num;
    
    return list;
}

void getIntervals( int from, int to, tree_t *tree, int index, vector_t *intervals )
{
//    fprintf(stderr, "search: %d - %d\ntree: %d - %d\n", from, to, tree[index].from, tree[index].to);
    if(to <= tree[index].from || from >= tree[index].to) return;
    if( from <= tree[index].from && tree[index].to <= to) pushVector( intervals, tree[index].data );
    else
    {
        getIntervals( from, to, tree, index * 2,     intervals ); 
        getIntervals( from, to, tree, index * 2 + 1, intervals ); 
    }
    
}

void pushVector( vector_t *vector, link_t *ptr )
{
    if( vector->capacity == vector->size )
    {
        vector->capacity <<= 1;
        vector->list = (link_t **)realloc( vector->list, vector->capacity * sizeof(link_t *) );
    }
    vector->list[vector->size] = ptr;
    vector->size++;
}

int linkLen( link_t *list )
{
    int length = 0;
    while(list != NULL)
    {
        list = list->next;
        length++;
    }
    return length;
}

void linkFree( link_t *list)
{
    while(list != NULL)
    {
        link_t *temp = list->next;
        free(list);
        list = temp;
    }
}

void freeAll(tree_t *tree)
{
    for(int i = 0; i < INTERVAL*2; i++)
    {
        linkFree(tree[i].data);
    }
    free(tree);
}
