#ifndef __PROGTEST__
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct TResult
{
  struct TResult * m_Next;
  int              m_ID;
  char           * m_Name;
} TRESULT;
#endif /* __PROGTEST__ */

typedef struct person
{
  struct person *father, *mother;
  char *name;
  int id;
  int stamp;
} person_t;

typedef struct tree
{
  struct tree *left, *right;
  person_t *person;
  int height; 
} tree_t;

typedef struct TDatabase
{
  tree_t *tree;
  int stamp;
} TDATABASE;

void      initAll          ( TDATABASE       * db )
{
  db->tree = NULL;
  db->stamp = 0;
}

void freeTree( tree_t *node )
{
  if( !node ) return;
  freeTree( node->left );
  freeTree( node->right );
  free( node->person->name );
  free( node->person );
  free( node );
}

void      doneAll          ( TDATABASE       * db )
{
  freeTree( db->tree );
}

char *copyStr( const char *name )
{
  int length = strlen( name ) + 1;
  char *buffer = (char *)malloc( length * sizeof( *buffer ) );
  strcpy( buffer, name );
  return buffer;
}

/**
 * @brief Finds ID in tree
 * 
 * @return person_t* returns ptr to person or NULL if ID does not exist
 */
person_t *findPerson( tree_t *root, int id )
{
//  printf( "in %p for %d\n", (void *)root, id );
  if( !root ) return NULL;
  person_t *person = root->person;
  if( person->id == id ) return person;
  return ( id < person->id ) ?
         findPerson( root->left, id ) :
         findPerson( root->right, id );
}

tree_t *createNode( person_t *newPerson )
{
  tree_t *node = (tree_t *)malloc( sizeof( *node ) );
  node->left = NULL;
  node->right = NULL;
  node->height = 1;
  node->person = newPerson;
  return node;
}

int setHeight( tree_t *node )
{
  if( !node ) return 0;
  int L = 0;
  int R = 0;
  if( node->left ) L = node->left->height;
  if( node->right ) R = node->right->height;
  node->height = 1 + ( L > R ? L : R );
  return R - L;
}

/*
   a             c
  / X           / \
 b   c   -->   a   e
    X \       / \
   d   e     b   d
*/
tree_t *rotateLeft( tree_t *node_a )
{
  tree_t *node_c = node_a->right;
  tree_t *node_d = NULL;
  if( node_c )
  {
    node_d = node_c->left;
    node_c->left = node_a;
  }
  node_a->right = node_d;

  setHeight( node_a );
  setHeight( node_c );

  return node_c;
}

/*
     a         b
    X \       / \
   b   c --> d   a
  / X           / \
 d   e         e   c
*/
tree_t *rotateRight( tree_t *node_a )
{
  tree_t *node_b = node_a->left;
  tree_t *node_e = NULL;
  if( node_b )
  {
    node_e = node_b->right;
    node_b->right = node_a;
  }
  node_a->left = node_e;

  setHeight( node_a );
  setHeight( node_b );

  return node_b;
}

tree_t *insertPerson( tree_t *root, person_t *newPerson )
{
  if( root == NULL )
  {
    return createNode( newPerson );
  }

  int nodeID = root->person->id;
  if( newPerson->id < nodeID )
  {
    root->left = insertPerson( root->left, newPerson );
  }
  else if( nodeID < newPerson->id )
  {
    root->right = insertPerson( root->right, newPerson );
  }
  else
    return root;

  int balance = setHeight( root );

  //left left
  if( balance < -1 && newPerson->id <= root->left->person->id )
    return rotateRight( root );

  //right right
  if( balance > 1 && newPerson->id >= root->right->person->id )
    return rotateLeft( root );

  //left right
  if( balance < -1 && newPerson->id >= root->left->person->id )
  {
    root->left = rotateLeft( root->left );
    return rotateRight( root );
  }

  //right left
  if( balance > 1 && newPerson->id <= root->right->person->id )
  {
    root->right = rotateRight( root->right );
    return rotateLeft( root );
  }

  return root;
}

int       addPerson        ( TDATABASE       * db,
                             int               id,
                             const char      * name,
                             int               id1,
                             int               id2 )
{
  if( id1 == id2 && id1 ) return 0;

  person_t *father = ( id1 ? findPerson( db->tree, id1 ) : NULL );
  if( id1 && !father ) return 0;

  person_t *mother = ( id2 ? findPerson( db->tree, id2 ) : NULL );
  if( id2 && !mother ) return 0;

  if( findPerson( db->tree, id ) ) return 0;

  person_t *newPerson = (person_t *)malloc( sizeof( *newPerson ) );
  newPerson->father = father;
  newPerson->mother = mother;
  newPerson->id = id;
  newPerson->name = copyStr( name );
  newPerson->stamp = 0;

  db->tree = insertPerson( db->tree, newPerson );
  return 1;
}

void getAncestors( person_t *person, TRESULT **first, TRESULT **last, int stamp, int reqStamp, int ignore )
{
  if( !person || person->stamp == stamp ) return;
//  printf( "%s: %p -> %p\n", person->name, (void *)*first, (void *)*last );

  int good = ( person->stamp == reqStamp || !reqStamp );
  person->stamp = stamp;
  getAncestors( person->father, first, last, stamp, reqStamp, 0 );
  getAncestors( person->mother, first, last, stamp, reqStamp, 0 );

  if( ignore || !good ) return;

  TRESULT *newAnc = (TRESULT *)malloc( sizeof( *newAnc ) );
  newAnc->m_ID = person->id;
  newAnc->m_Next = NULL;
  newAnc->m_Name = copyStr( person->name );

  if( !(*last) )
  {
    *first = newAnc;
    *last = newAnc;
    return;
  }
  (*last)->m_Next = newAnc;
  *last = newAnc;
}

TRESULT * ancestors        ( TDATABASE       * db,
                             int               id )
{
  person_t *person = findPerson( db->tree, id );
  if( !person ) return NULL;
  TRESULT *first = NULL, *last = NULL;
  db->stamp++;
  getAncestors( person, &first, &last, db->stamp, 0, 1 );
  return first;
}

void setStamp( person_t *person, int stamp, int first )
{
  if( !person || person->stamp == stamp ) return;
  if( !first ) person->stamp = stamp;
  setStamp( person->father, stamp, 0 );
  setStamp( person->mother, stamp, 0 );
}

TRESULT * commonAncestors  ( TDATABASE       * db,
                             int               id1,
                             int               id2 )
{
  person_t *person1 = findPerson( db->tree, id1 );
  person_t *person2 = findPerson( db->tree, id2 );
  if( !person1 || !person2 ) return NULL;

  db->stamp++;
  setStamp( person1, db->stamp, 1 );

  db->stamp++;
  TRESULT *first = NULL, *last = NULL;
  getAncestors( person2, &first, &last, db->stamp, db->stamp - 1, 1 );
  return first;
}

void      freeResult       ( TRESULT         * res )
{
  while( res != NULL )
  {
    TRESULT *tmp = res->m_Next;
    free( res->m_Name );
    free( res );
    res = tmp;
  }
}

#ifndef __PROGTEST__

void printTree( tree_t *node )
{
  if( !node ) return;
  printf( "%p/%d: %d-%s -> %p + %p\n", (void *)node, node->height, node->person->id,
           node->person->name, (void *)node->left, (void *)node->right );
  printTree( node->left );
  printTree( node->right );
}

void printfRes( TRESULT *res )
{
  printf("---Res---\n");
  while( res != NULL )
  {
    printf( "%d: %s -> %p\n", res->m_ID, res->m_Name, (void *)(res->m_Next) );
    res = res->m_Next;
  }
}

int main                   ( int               argc,
                             char            * argv [] )
{
  char      name[100];
  TDATABASE a, b;
  TRESULT * l;

  initAll ( &a );
  assert ( addPerson ( &a, 1, "John", 0, 0 ) == 1 );
  strncpy ( name, "Jane", sizeof ( name ) );
  assert ( addPerson ( &a, 2, name, 0, 0 ) == 1 );
  assert ( addPerson ( &a, 3, "Caroline", 0, 0 ) == 1 );
  assert ( addPerson ( &a, 4, "Peter", 0, 0 ) == 1 );
  assert ( addPerson ( &a, 5, "George", 1, 2 ) == 1 );
  assert ( addPerson ( &a, 6, "Martin", 1, 2 ) == 1 );
  assert ( addPerson ( &a, 7, "John", 3, 4 ) == 1 );
  assert ( addPerson ( &a, 8, "Sandra", 3, 4 ) == 1 );
  assert ( addPerson ( &a, 9, "Eve", 1, 2 ) == 1 );
  assert ( addPerson ( &a, 10, "Douglas", 1, 4 ) == 1 );
  strncpy ( name, "Phillipe", sizeof ( name ) );
  assert ( addPerson ( &a, 11, name, 6, 8 ) == 1 );
  strncpy ( name, "Maria", sizeof ( name ) );
  assert ( addPerson ( &a, 12, name, 5, 8 ) == 1 );
  l = ancestors ( &a, 11 );
  assert ( l );
  assert ( l -> m_ID == 1 );
  assert ( ! strcmp ( l -> m_Name, "John" ) );
  assert ( l -> m_Next );
  assert ( l -> m_Next -> m_ID == 2 );
  assert ( ! strcmp ( l -> m_Next -> m_Name, "Jane" ) );
  assert ( l -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_ID == 6 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Name, "Martin" ) );
  assert ( l -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_ID == 3 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "Caroline" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 4 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 8 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Sandra" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  freeResult ( l );

  l = ancestors ( &a, 3 );
  assert ( l == NULL );

  l = ancestors ( &a, 13 );
  assert ( l == NULL );

  l = commonAncestors ( &a, 11, 12 );
  assert ( l );
  assert ( l -> m_ID == 1 );
  assert ( ! strcmp ( l -> m_Name, "John" ) );
  assert ( l -> m_Next );
  assert ( l -> m_Next -> m_ID == 2 );
  assert ( ! strcmp ( l -> m_Next -> m_Name, "Jane" ) );
  assert ( l -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_ID == 3 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Name, "Caroline" ) );
  assert ( l -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_ID == 4 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 8 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Sandra" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  freeResult ( l );

  l = commonAncestors ( &a, 10, 9 );
  assert ( l );
  assert ( l -> m_ID == 1 );
  assert ( ! strcmp ( l -> m_Name, "John" ) );
  assert ( l -> m_Next == NULL );
  freeResult ( l );

  assert ( commonAncestors ( &a, 7, 6 ) == NULL );

  l = commonAncestors ( &a, 7, 10 );
  assert ( l );
  assert ( l -> m_ID == 4 );
  assert ( ! strcmp ( l -> m_Name, "Peter" ) );
  assert ( l -> m_Next == NULL );
  freeResult ( l );

  assert ( addPerson ( &a, 13, "Quido", 12, 11 ) == 1 );
  printTree( a.tree );

  l = ancestors ( &a, 13 );
  assert ( l );
  assert ( l -> m_ID == 1 );
  assert ( ! strcmp ( l -> m_Name, "John" ) );
  assert ( l -> m_Next );
  assert ( l -> m_Next -> m_ID == 2 );
  assert ( ! strcmp ( l -> m_Next -> m_Name, "Jane" ) );
  assert ( l -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_ID == 5 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Name, "George" ) );
  assert ( l -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_ID == 3 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "Caroline" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 4 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 8 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Sandra" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 12 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Maria" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 6 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Martin" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 11 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Phillipe" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  freeResult ( l );

  l = commonAncestors ( &a, 9, 12 );
  assert ( l );
  assert ( l -> m_ID == 1 );
  assert ( ! strcmp ( l -> m_Name, "John" ) );
  assert ( l -> m_Next );
  assert ( l -> m_Next -> m_ID == 2 );
  assert ( ! strcmp ( l -> m_Next -> m_Name, "Jane" ) );
  assert ( l -> m_Next -> m_Next == NULL );
  freeResult ( l );

  assert ( addPerson ( &a, 1, "Francois", 0, 0 ) == 0 );

  initAll ( &b );
  assert ( addPerson ( &b, 10000, "John", 0, 0 ) == 1 );
  assert ( addPerson ( &b, 10000, "Peter", 0, 0 ) == 0 );
  assert ( addPerson ( &b, 20000, "Jane", 10000, 0 ) == 1 );
  assert ( addPerson ( &b, 30000, "Maria", 10000, 10000 ) == 0 );
  assert ( addPerson ( &b, 40000, "Joe", 10000, 30000 ) == 0 );
  assert ( addPerson ( &b, 50000, "Carol", 50000, 20000 ) == 0 );
  assert ( addPerson ( &b, 12, "Maria", 20000, 10000 ) == 1 );
//  printf( "-----\n" );
//  printTree( b.tree );

  l = ancestors ( &a, 12 );
  printfRes( l );
  assert ( l );
  assert ( l -> m_ID == 1 );
  assert ( ! strcmp ( l -> m_Name, "John" ) );
  assert ( l -> m_Next );
  assert ( l -> m_Next -> m_ID == 2 );
  assert ( ! strcmp ( l -> m_Next -> m_Name, "Jane" ) );
  assert ( l -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_ID == 5 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Name, "George" ) );
  assert ( l -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_ID == 3 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "Caroline" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 4 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 8 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Sandra" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  freeResult ( l );
  l = ancestors ( &b, 12 );
  assert ( l );
  assert ( l -> m_ID == 10000 );
  assert ( ! strcmp ( l -> m_Name, "John" ) );
  assert ( l -> m_Next );
  assert ( l -> m_Next -> m_ID == 20000 );
  assert ( ! strcmp ( l -> m_Next -> m_Name, "Jane" ) );
  assert ( l -> m_Next -> m_Next == NULL );
  freeResult ( l );
  assert ( ancestors ( &a, 20000 ) == NULL );
  assert ( commonAncestors ( &b, 7, 10 ) == NULL );
  doneAll ( &b );

  doneAll ( &a );

  return 0;
}
#endif /* __PROGTEST__ */
