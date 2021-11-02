#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#endif /* __PROGTEST__ */

int gcd(int a, int b)
{
  int temp;
  if(a < b)
  {
    temp = b;
    b = a;
    a = temp;
  }

  while(b != 0)
  {
    temp = a % b;
    a = b;
    b = temp;
  }

  return a;
}

void rotateArray ( int array [], int arrayLen, int rotateBy )
{
  /* TODO: Your code here */
  rotateBy = ( ( rotateBy % arrayLen ) + arrayLen ) % arrayLen;
  int rotations = gcd( arrayLen, rotateBy ), temp, carry;
  for(int start = 0; start < rotations; start++)
  {
    int curr = start;
    carry = array[curr];
    do{
      curr = ( curr + rotateBy ) % arrayLen;
      temp = array[curr];
      array[curr] = carry;
      carry = temp;

    }while( curr != start );
  }
}

#ifndef __PROGTEST__
int identicalArrays ( const int a[], const int b[], int cnt )
{
  /* TODO: Your code here */
  for(int i = 0; i < cnt; i++)
  {
    if( a[i] != b[i] )
    {
      for(i = 0; i < cnt; i++)
      {
        printf("%2d ", a[i]);
      }
      printf("\n");
      for(i = 0; i < cnt; i++)
      {
        printf("%2d ", b[i]);
      }
      printf("\n");
      return 0;
    }
  }
  return 1;
}
int main (int argc, char * argv [])
{
  assert( 15 == gcd( 15 * 3, 15 * 4 ) );
  assert(  6 == gcd( 36, 42 ) );
  assert( 15 == gcd( 15 * 7, 15 * 13 ) );
  assert( 15 == gcd( 15 * 143, 15 * 144 ) );

  int in0 []  = { 1, 2, 3, 4, 5 };
  int out0 [] = { 5, 1, 2, 3, 4 };

  int in1 []  = { 1, 2, 3, 4, 5 };
  int out1 [] = { 2, 3, 4, 5, 1 };

  int in2 []  = { 1, 2, 3, 4, 5, 6 };
  int out2 [] = { 4, 5, 6, 1, 2, 3 };

  int in3 []  = { 1, 2, 3, 4, 5, 6, 7 };
  int out3 [] = { 3, 4, 5, 6, 7, 1, 2 };

  int in4 []  = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  int out4 [] = { 2, 3, 4, 5, 6, 7, 8, 9, 1 };

  rotateArray ( in0, 5, 1 );
  assert ( identicalArrays ( in0, out0, 5 ) );
  rotateArray ( in1, 5, -1 );
  assert ( identicalArrays ( in1, out1, 5 ) );
  rotateArray ( in2, 6, -3 );
  assert ( identicalArrays ( in2, out2, 6 ) );
  rotateArray ( in3, 7, 12 );
  assert ( identicalArrays ( in3, out3, 7 ) );
  rotateArray ( in4, 9, -100 );
  assert ( identicalArrays ( in4, out4, 9 ) );
  return 0;
}
#endif /* __PROGTEST__ */

