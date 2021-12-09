#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct TItem
{
  struct TItem * m_Next;
  struct TItem * m_Prev;
  int      m_Val;
} TITEM;

typedef struct TData
{
  TITEM * m_First;
  TITEM * m_Last;
} TDATA;

#endif /* __PROGTEST__ */

void insertStart ( TDATA * l, int x )
{
  TITEM *newItem = (TITEM *)malloc( sizeof( *newItem ) );
  newItem->m_Val = x;
  newItem->m_Next = l->m_First;
  newItem->m_Prev = NULL;

  if( l->m_First ) l->m_First->m_Prev = newItem;

  l->m_First = newItem;
  if(!l->m_Last) l->m_Last = newItem;
}

void insertEnd   ( TDATA * l, int x )
{
  TITEM *newItem = (TITEM *)malloc( sizeof( *newItem ) );
  newItem->m_Val = x;
  newItem->m_Next = NULL;
  newItem->m_Prev = l->m_Last;

  if( l->m_Last ) l->m_Last->m_Next = newItem;

  l->m_Last = newItem;
  if( !l->m_First ) l->m_First = newItem;
}

int  findMax     ( TDATA * l )
{
  int max = -__INT_MAX__ - 1;
  TITEM *item = l->m_First;
  while( item )
  {
    max = item->m_Val > max ? item->m_Val : max;
    item = item->m_Next;
  }
  return max;
}

int  removeMax   ( TDATA * l )
{
  int max = findMax( l );
  int removed = 0;
  TITEM *item = l->m_First;
  while( item && item->m_Val == max )
  {
    TITEM *tmp = item->m_Next;
    free( item );
    item = tmp;
    if( item ) item->m_Prev = NULL;
    removed++;
  }
  l->m_First = item;
  if( !item )
  {
    l->m_Last = NULL;
    return removed;
  }
  while( item->m_Next )
  {
    if( item->m_Next->m_Val == max )
    {
      TITEM *tmp = item->m_Next;
      item->m_Next = item->m_Next->m_Next;
      if( item->m_Next )
      {
        item->m_Next->m_Prev = item;
      }
      free( tmp );
      removed++;
      continue;
    }
    item = item->m_Next;
  }
  l->m_Last = item;
  return removed;
}

void destroyAll  ( TDATA * l )
{
  TITEM *item = l->m_First;
  while ( item )
  {
    TITEM *tmp = item->m_Next;
    free( item );
    item = tmp;
  }
}

#ifndef __PROGTEST__

void printList( TDATA *l )
{
  TITEM *item = l->m_First;
  printf("<%p:%p> ", (void *)l->m_First, (void *)l->m_Last);
  while( item )
  {
    printf( "%d -> ", item->m_Val );
    item = item->m_Next;
  }
  printf( "%p \n", (void *)item );
}

int main ( void )
{
  TDATA a;
  a . m_First = a . m_Last = NULL;
  insertEnd ( &a, 1 );
  insertEnd ( &a, 2 );
  insertEnd ( &a, 1 );
  insertEnd ( &a, 3 );
  insertEnd ( &a, 4 );
  insertEnd ( &a, 2 );
  insertEnd ( &a, 5 );
  assert ( a . m_First != NULL
           && a . m_First -> m_Val == 1
           && a . m_First -> m_Next != NULL
           && a . m_First -> m_Prev == NULL
           && a . m_First -> m_Next -> m_Val == 2
           && a . m_First -> m_Next -> m_Next != NULL
           && a . m_First -> m_Next -> m_Prev == a . m_First
           && a . m_First -> m_Next -> m_Next -> m_Val == 1
           && a . m_First -> m_Next -> m_Next -> m_Next != NULL
           && a . m_First -> m_Next -> m_Next -> m_Prev == a . m_First -> m_Next
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Val == 3
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next != NULL
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Prev == a . m_First -> m_Next -> m_Next
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Val == 4
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next != NULL
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Prev == a . m_First -> m_Next -> m_Next -> m_Next
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Val == 2
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next != NULL
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Prev == a . m_First -> m_Next -> m_Next -> m_Next -> m_Next
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Val == 5
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Prev == a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
           && a . m_Last == a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( removeMax ( &a ) == 1 );
  assert ( a . m_First != NULL
           && a . m_First -> m_Val == 1
           && a . m_First -> m_Next != NULL
           && a . m_First -> m_Prev == NULL
           && a . m_First -> m_Next -> m_Val == 2
           && a . m_First -> m_Next -> m_Next != NULL
           && a . m_First -> m_Next -> m_Prev == a . m_First
           && a . m_First -> m_Next -> m_Next -> m_Val == 1
           && a . m_First -> m_Next -> m_Next -> m_Next != NULL
           && a . m_First -> m_Next -> m_Next -> m_Prev == a . m_First -> m_Next
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Val == 3
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next != NULL
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Prev == a . m_First -> m_Next -> m_Next
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Val == 4
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next != NULL
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Prev == a . m_First -> m_Next -> m_Next -> m_Next
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Val == 2
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Prev == a . m_First -> m_Next -> m_Next -> m_Next -> m_Next
           && a . m_Last == a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
  destroyAll ( &a );

  a . m_First = a . m_Last = NULL;
  insertEnd ( &a, 1 );
  insertEnd ( &a, 2 );
  insertEnd ( &a, 3 );
  insertEnd ( &a, 0 );
  insertEnd ( &a, 2 );
  insertEnd ( &a, 3 );
  insertStart ( &a, 1 );
  insertStart ( &a, 2 );
  insertStart ( &a, 3 );
  assert ( a . m_First != NULL
           && a . m_First -> m_Val == 3
           && a . m_First -> m_Next != NULL
           && a . m_First -> m_Prev == NULL
           && a . m_First -> m_Next -> m_Val == 2
           && a . m_First -> m_Next -> m_Next != NULL
           && a . m_First -> m_Next -> m_Prev == a . m_First
           && a . m_First -> m_Next -> m_Next -> m_Val == 1
           && a . m_First -> m_Next -> m_Next -> m_Next != NULL
           && a . m_First -> m_Next -> m_Next -> m_Prev == a . m_First -> m_Next
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Val == 1
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next != NULL
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Prev == a . m_First -> m_Next -> m_Next
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Val == 2
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next != NULL
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Prev == a . m_First -> m_Next -> m_Next -> m_Next
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Val == 3
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next != NULL
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Prev == a . m_First -> m_Next -> m_Next -> m_Next -> m_Next
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Val == 0
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next != NULL
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Prev == a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Val == 2
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next != NULL
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Prev == a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Val == 3
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Prev == a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
           && a . m_Last == a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( removeMax ( &a ) == 3 );
  assert ( a . m_First != NULL
           && a . m_First -> m_Val == 2
           && a . m_First -> m_Next != NULL
           && a . m_First -> m_Prev == NULL
           && a . m_First -> m_Next -> m_Val == 1
           && a . m_First -> m_Next -> m_Next != NULL
           && a . m_First -> m_Next -> m_Prev == a . m_First
           && a . m_First -> m_Next -> m_Next -> m_Val == 1
           && a . m_First -> m_Next -> m_Next -> m_Next != NULL
           && a . m_First -> m_Next -> m_Next -> m_Prev == a . m_First -> m_Next
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Val == 2
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next != NULL
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Prev == a . m_First -> m_Next -> m_Next
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Val == 0
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next != NULL
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Prev == a . m_First -> m_Next -> m_Next -> m_Next
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Val == 2
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL
           && a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Prev == a . m_First -> m_Next -> m_Next -> m_Next -> m_Next
           && a . m_Last == a . m_First -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( removeMax ( &a ) == 3 );
  assert ( a . m_First != NULL
           && a . m_First -> m_Val == 1
           && a . m_First -> m_Next != NULL
           && a . m_First -> m_Prev == NULL
           && a . m_First -> m_Next -> m_Val == 1
           && a . m_First -> m_Next -> m_Next != NULL
           && a . m_First -> m_Next -> m_Prev == a . m_First
           && a . m_First -> m_Next -> m_Next -> m_Val == 0
           && a . m_First -> m_Next -> m_Next -> m_Next == NULL
           && a . m_First -> m_Next -> m_Next -> m_Prev == a . m_First -> m_Next
           && a . m_Last == a . m_First -> m_Next -> m_Next );
  assert ( removeMax ( &a ) == 2 );
  assert ( a . m_First != NULL
           && a . m_First -> m_Val == 0
           && a . m_First -> m_Next == NULL
           && a . m_First -> m_Prev == NULL
           && a . m_Last == a . m_First );
  destroyAll ( &a );

  a . m_First = a . m_Last = NULL;
  insertEnd ( &a, -1 );
  insertEnd ( &a, -2 );
  insertEnd ( &a, -10000 );
  insertEnd ( &a, -1 );
  insertEnd ( &a, -300 );
  assert ( removeMax ( &a ) == 2 );
  assert ( a . m_First != NULL
           && a . m_First -> m_Val == -2
           && a . m_First -> m_Next != NULL
           && a . m_First -> m_Prev == NULL
           && a . m_First -> m_Next -> m_Val == -10000
           && a . m_First -> m_Next -> m_Next != NULL
           && a . m_First -> m_Next -> m_Prev == a . m_First
           && a . m_First -> m_Next -> m_Next -> m_Val == -300
           && a . m_First -> m_Next -> m_Next -> m_Next == NULL
           && a . m_First -> m_Next -> m_Next -> m_Prev == a . m_First -> m_Next
           && a . m_Last == a . m_First -> m_Next -> m_Next );
  destroyAll ( &a );

  a . m_First = a . m_Last = NULL;
  assert ( removeMax ( &a ) == 0 );

  insertEnd ( &a, 0 );
  printList( &a );
  assert ( removeMax ( &a ) == 1 );
  printList( &a );

  insertEnd ( &a, 0 );
  insertEnd ( &a, 0 );
  insertEnd ( &a, 0 );
  assert ( removeMax ( &a ) == 3 );

  insertStart ( &a, 0 );
  insertStart ( &a, 0 );
  insertStart ( &a, 0 );
  assert ( removeMax ( &a ) == 3 );

  insertStart ( &a, 0 );
  insertEnd ( &a, 0 );
  insertStart ( &a, 0 );
  assert ( removeMax ( &a ) == 3 );

  insertEnd ( &a, 0 );
  insertStart ( &a, 0 );
  insertEnd ( &a, 0 );
  assert ( removeMax ( &a ) == 3 );

  insertStart ( &a, 1 );
  insertStart ( &a, 1 );
  insertStart ( &a, 0 );
  assert ( removeMax ( &a ) == 2 );
  assert ( removeMax ( &a ) == 1 );

  insertStart ( &a, 0 );
  insertStart ( &a, 1 );
  insertStart ( &a, 1 );
  assert ( removeMax ( &a ) == 2 );
  assert ( removeMax ( &a ) == 1 );

  insertStart ( &a, 1 );
  insertStart ( &a, 0 );
  insertStart ( &a, 1 );
  assert ( removeMax ( &a ) == 2 );
  assert ( removeMax ( &a ) == 1 );


  return 0;
}
#endif /* __PROGTEST__ */
