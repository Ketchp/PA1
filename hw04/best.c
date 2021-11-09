#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_ID 100000l
#define MAX_QUERY 1000000
#define BUCKET_SIZE 1000
#define BUCKET_COUNT (MAX_QUERY/BUCKET_SIZE)

int add( int id, int *list, int *changes, int count );
int query( int from, int to, int *list, int *bucket_diff, int *changes );

void printl(int *visit_count)
{
    for(int buc = 0; buc < 3; buc++)
    {
        for(int i = 0; i < 26; i++)
        {
            printf("%d ", visit_count[i + buc * MAX_ID]);
        }
        printf("\n");
    }
}

int main()
{
    int *visit_count = (int *)malloc( MAX_ID * ( BUCKET_COUNT + 1 ) * sizeof(*visit_count) );
    assert( visit_count != NULL );
    int *bucket_diff = (int *)calloc( ( BUCKET_COUNT + 1) * ( BUCKET_COUNT + 1 ), sizeof(*bucket_diff) );
    int change_list[ MAX_QUERY ];
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
                printf( "Nespravny vstup.\n" );
//                printl(visit_count);
                free(visit_count);
                return 0;
            }

            ret = add( new_id, visit_count, change_list, count);
            count++;

            if( ret == 1 ) printf( "> prvni navsteva\n" );
            else printf( "> navsteva #%d\n", ret );

            break;

        case '?':
            if( scanf( "%d%c%d%c", &from, &space, &to, &operation ) != 4 || operation != '\n' || space != ' ' ||
                from < 0 || to >= count || to < from )
            {
                printf( "Nespravny vstup.\n" );
//                printl(visit_count);
                free(visit_count);
                return 0;
            }
            to++;
            ret = query( from, to, visit_count, bucket_diff, change_list );
            printf( "> %d / %d\n", ret, to - from );
            break;

        default:
            printf( "Nespravny vstup.\n" );
//            printl(visit_count);
            free(visit_count);
            return 0;
        }
    }
//    printl(visit_count);
    free(visit_count);
    return 0;
}

int add( int id, int *list, int *changes, int count)
{
    int bucket = count / BUCKET_SIZE;
    if( count % BUCKET_SIZE == 0 && count != 0 )
    {
        memcpy(list + bucket * MAX_ID, list + (bucket -1) * MAX_ID, MAX_ID * sizeof(*list));
    }
    list[ MAX_ID * bucket + id ]++;
    changes[count] = id;
    return list[ MAX_ID * bucket + id ];
}

int query(int from, int to, int *list, int *bucket_diff, int *changes)
{
    static int used[MAX_ID] = {0};
    int id_count = 0;
    int bucket_start = from / BUCKET_SIZE;
    int bucket_end = to / BUCKET_SIZE - 1;
    if(bucket_start >= bucket_end)
    {
//        printf("B%d %d %d %d\n", from, to, bucket_start, bucket_end);
        for(int i = from; i < to; i++)
        {
            int id = changes[i];
            if(!used[id])
            {
                used[id] = 1;
                id_count++;
            }
        }
        for(int i = from; i < to; i++)
        {
            int id = changes[i];
            used[id] = 0;
        }
        return id_count;
    }
    if( bucket_diff[ bucket_start * (BUCKET_SIZE + 1) + bucket_end ] )
    {
        for(int i = 0; i < MAX_ID; i++)
        {
            int change = list[bucket_end * MAX_ID + i] - list[bucket_start * MAX_ID + i];
            if( change ) bucket_diff[ bucket_start * (BUCKET_SIZE + 1) + bucket_end ]++;
        }
    }
//    printf("S%d %d %d %d\n", from, to, bucket_start, bucket_end);
    id_count = bucket_diff[ bucket_start * (BUCKET_SIZE + 1) + bucket_end ];
    for(int i = from; i < ( bucket_start + 1 ) * BUCKET_SIZE; i++)
    {
        int id = changes[i];
        int change = list[bucket_end * MAX_ID + id] - list[bucket_start * MAX_ID + id];
        if( change ) continue;
        if(!used[id])
        {
            used[id] = 1;
            id_count++;
        }
    }

    for(int i = ( bucket_end + 1 ) * BUCKET_SIZE; i < to; i++) //?? <=
    {
        int id = changes[i];
        int change = list[bucket_end * MAX_ID + id] - list[bucket_start * MAX_ID + id];
        if( change ) continue;
        if(!used[id])
        {
            used[id] = 1;
            id_count++;
        }
    }

    for(int i = from; i < ( bucket_start + 1 ) * BUCKET_SIZE; i++)
    {
        int id = changes[i];
        used[id] = 0;
    }

    for(int i = ( bucket_end + 1 ) * BUCKET_SIZE; i < to; i++) //?? <=
    {
        int id = changes[i];
        used[id] = 0;
    }

    return id_count;
}