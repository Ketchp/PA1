#include <stdio.h>
#include <stdlib.h>

#define MAX_ID 100000
#define MAX_QUERY 1000000

int add( int id, int *list, int *changes, int count );
int query( int from, int to, int *changes );

int main()
{
    int visit_count[ MAX_ID ] = {0};
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
                return 0;
            }

            ret = add( new_id, visit_count , change_list, count);
            count++;

            if( ret == 1 ) printf( "> prvni navsteva\n" );
            else printf( "> navsteva #%d\n", ret );

            break;

        case '?':
            if( scanf( "%d%c%d%c", &from, &space, &to, &operation ) != 4 || operation != '\n' || space != ' ' ||
                from < 0 || to >= count || to < from )
            {
                printf( "Nespravny vstup.\n" );
                return 0;
            }
            ret = query( from, to, change_list );
            printf( "> %d / %d\n", ret, to - from + 1 );
            break;

        default:
            printf( "Nespravny vstup.\n" );
            return 0;
        }
    }
    return 0;
}

int add( int id, int *list, int *changes, int count)
{
    list[id]++;
    changes[count] = id;
    return list[id];
}

int query(int from, int to, int *changes)
{
    static int used[MAX_ID] = {0};

    int id_count = 0;
    for(int i = from; i <= to; i++)
    {
        int id = changes[i];
        if(!used[id])
        {
            used[id] = 1;
            id_count++;
        }
    }
    for(int i = from; i <= to; i++)
    {
        int id = changes[i];
        used[id] = 0;
    }
    
    return id_count;
}