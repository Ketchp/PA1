#include <stdio.h>
#include <stdlib.h>

#define MAX_ID 100000
#define MAX_QUERY 1000000

/**
 * @brief Add's new ID record
 * 
 * @param visits[out] list of IDs in order
 * @param idCount[in,out] count of each ID
 * @param id[in] new ID
 * @param visitCount[in] count of made visits
 * @return int count of ID visits
 */
int add( int *visits, int *idCount, int id, int visitCount );

/**
 * @brief Calculates unique ID on interval
 * 
 * @param visits[in] list of IDs in order
 * @param from[in] interval start
 * @param to[in] interval end
 * @return int unique ID on [from, to]
 */
int query( int *visits, int from, int to );

int main()
{
    int idCount[ MAX_ID ] = {0};
    int visits[ MAX_QUERY ];
    int visitCount = 0;
    char operation, space, cariage_return;
    printf( "Pozadavky:\n" );
    while( scanf( "%c ", &operation ) != EOF )
    {
        int newID, from, to, repetition;
        switch( operation )
        {
        case '+':
            if( scanf( "%d%c", &newID, &cariage_return ) != 2 || cariage_return != '\n' ||
                 newID < 0 || newID >= MAX_ID || visitCount == MAX_QUERY )
            {
                printf( "Nespravny vstup.\n" );
                return 0;
            }

            repetition = add( visits, idCount, newID , visitCount);
            visitCount++;

            if( repetition == 1 ) printf( "> prvni navsteva\n" );
            else printf( "> navsteva #%d\n", repetition );

            break;

        case '?':
            if( scanf( "%d%c%d%c", &from, &space, &to, &cariage_return ) != 4 ||
                 cariage_return != '\n' || space != ' ' ||
                 from < 0 || to >= visitCount || to < from )
            {
                printf( "Nespravny vstup.\n" );
                return 0;
            }

            repetition = query( visits, from, to );
            printf( "> %d / %d\n", repetition, to - from + 1 );
            break;

        default:
            printf( "Nespravny vstup.\n" );
            return 0;
        }
    }
    return 0;
}

int add( int *visits, int *idCount, int id, int visitCount )
{
    idCount[id]++;
    visits[visitCount] = id;
    return idCount[id];
}

int query( int *visits, int from, int to )
{
    int visited[MAX_ID] = {0};

    int id_count = 0;
    for( int time = from; time <= to; time++ )
    {
        int id = visits[ time ];
        if( !visited[id] )
        {
            visited[id] = 1;
            id_count++;
        }
    }
    
    return id_count;
}