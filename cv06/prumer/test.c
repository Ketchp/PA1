#ifndef __PROGTEST__
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <stdint.h>
#endif /* __PROGTEST__ */

long long avg3 ( long long a, long long b, long long c )
{
  /* TODO: Your code here */
  long long mod = a%3 + b%3 + c%3;
  long long whole = a/3 + b/3 + c/3 + mod/3;
  if(whole > 0 && mod%3 < 0){
    whole--;
  }
  if(whole < 0 && mod%3 > 0){
    whole++;
  }
  return whole;
}

#ifndef __PROGTEST__
int main (int argc, char * argv [])
{
  assert ( avg3 ( 1, 2, 3 ) == 2 );
  assert ( avg3 ( -100, 100, 30 ) == 10 );
  assert ( avg3 ( 1, 2, 2 ) == 1 );
  assert ( avg3 ( -1, -2, -2 ) == -1 );
  assert ( avg3 ( LLONG_MAX, LLONG_MAX, LLONG_MAX ) == LLONG_MAX );

  for(int a = -10; a < 10; a++){
    for(int b = -10; b < 10; b++){
      for(int c = -10; c < 10; c++){
        if(avg3(a, b, c) != (a+b+c)/3){
          printf("%d %d %d\n",a,b,c);
        }
        assert( avg3(a, b, c) == (a+b+c)/3 );
      }
    }
  }
  assert ( avg3 ( LLONG_MIN, LLONG_MIN, LLONG_MIN ) == LLONG_MIN );
  assert ( avg3 ( LLONG_MAX, LLONG_MAX, LLONG_MIN ) == LLONG_MAX/3 );
  assert ( avg3 ( LLONG_MAX, LLONG_MIN, 0 ) == (LLONG_MAX+LLONG_MIN)/3 );
  assert (avg3 ( 9223372036854775800, 9223372036854775800, -8 ) == 6148914691236517197);
  return 0;
}
#endif /* __PROGTEST__ */

