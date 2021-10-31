#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#endif /* __PROGTEST__ */

/*
0 -> 9 -> 0 = 30
0 -> 5 -> 0 = 20
0 -> 2 -> 0 = 12
0 -> 3 -> 0 = 14
1 -> 8 -> 1 = 28
1 -> 9 -> 1 = 28
0 -> 1 -> 0 =  8
1 -> 2 -> 1 = 10
*/
//                                                                                        YYYY|MM|DD|hh|mm|ss
//                                                                                        0000|01|01|00|00|00
#define TEN_SEC_CONSP (30LL)                                              //       30     0000|01|01|00|00|10
#define MINUTE_CONSP (30LL*6 + 20)                                        //      200     0000|01|01|00|01|00
#define TEN_MINUTE_CONSP (200LL*10 + 30)                                  //     2030     0000|01|01|00|10|00
#define HOUR_CONSP (2030LL*6+20)                                          //    12200     0000|01|01|01|00|00
#define TEN_HOUR_CONSP (12200LL*10 + 30)                                  //   122030     0000|01|01|10|00|00
#define DAY_CONSP (122030LL*2 + 12 +12200*4 + 14)                         //   292886     0000|01|02|00|00|00
#define TEN_DAY_CONSP (292886LL*10 + 30)                                  //  2928890     0000|01|11|00|00|00
#define MONTH_28_CONSP (2928890LL*2 + 12 + 292886*8 + 28)                 //  8200908     0000|02|01|00|00|00
#define MONTH_29_CONSP (2928890LL*2 + 12 + 292886*9 + 28)                 //  8493794     0000|02|01|00|00|00
#define MONTH_30_CONSP (2928890LL*3 + 14)                                 //  8786684     0000|02|01|00|00|00
#define MONTH_31_CONSP (2928890LL*3 + 14 + 292886*1)                      //  9079570     0000|02|01|00|00|00
#define YEAR_N_CONSP (9079570LL*7 + 8786684*4 + 8200908*1 + 8 + 30 + 10)  //106904682     0001|01|01|00|00|00
#define YEAR_L_CONSP (9079570LL*7 + 8786684*4 + 8493794*1 + 8 + 30 + 10)  //107197568     0001|01|01|00|00|00

static long long decadic_prefix[] = {0,  4,  9, 11, 14, 17, 18, 23, 27, 28, 30};

static int monthDays[] =   {0,  31,  28,  31,  30,  31,  30,  31,  31,  30,  31,  30,  31};

int compareDate( int y1, int m1, int d1, int h1, int i1,
                 int y2, int m2, int d2, int h2, int i2);
int validateDate(int year, int month, int day);
int validateDayTime(int hour, int minute);
long long minuteConsumption(int i1, int i2);
long long hourConsumption(int h1, int h2);
long long dayConsumption(int d1, int d2); //Co ak feb 29 je d2 ??
long long monthConsumption(int m1, int m2, int leap);
long long yearConsumption(int y1, int m1, int y2);

long long digitConsumption(int from, int to);

int isLeapYear(int year);
int leapYearsTo(int year);
int leapYearsBetw(int from, int to);

int energyConsumption ( int y1, int m1, int d1, int h1, int i1,
                        int y2, int m2, int d2, int h2, int i2, long long int * consumption )
{
  /* todo */
  int valid = validateDate(y1, m1, d1) && 
              validateDate(y2, m2, d2) &&
              validateDayTime(h1, i1) &&
              validateDayTime(h2, i2);
  if(!valid) return 0;
  if(!compareDate(y1, m1, d1, h1, i1, y2 , m2, d2, h2, i2)) return 0;


  *consumption = yearConsumption( y1, m1, y2 );
  *consumption += monthConsumption( m1, m2, isLeapYear(y2) );
  *consumption += dayConsumption( d1, d2 ); //Co ak feb 29 je d2 ??
  *consumption += hourConsumption( h1, h2 );
  *consumption += minuteConsumption( i1, i2 );
  return 1;
}



#ifndef __PROGTEST__

long long increment(int *year, int *month, int *day, int *hour, int *minute)
{
  static long long decChange[] = {4, 5, 2, 3, 3, 1, 5, 4, 1, 2};
  long long consumption = 200;
  consumption += decChange[*minute%10];
  if(*minute%10 != 9)
  {
    ++*minute;
    return consumption;
  }
  if(*minute != 59)
  {
    consumption += decChange[*minute/10];
    ++*minute;
    return consumption;
  }
  *minute = 0;
  consumption += 3; // 5->0 + 9->0

  if(*hour % 10 != 9 && *hour != 23)
  {
    consumption += decChange[*hour%10];
    ++*hour;
    return consumption;
  }
  if(*hour % 10 == 9)
  {
    consumption += decChange[*hour%10] + decChange[*hour/10];
    ++*hour;
    return consumption;
  }
  *hour = 0;
  consumption += 3 + 3; // 2->0 + 3->0

  int maxDay = monthDays[*month];
  if(*month == 2 && isLeapYear(*year)) maxDay++;
  if(*day < maxDay && *day % 10 != 9)
  {
    consumption += decChange[*day%10];
    ++*day;
    return consumption;
  }
  if(*day < maxDay)
  {
    consumption += decChange[*day%10] + decChange[*day/10];
    ++*day;
    return consumption;
  }
  *day = 1;
  switch (maxDay)
  {
    case 28:
      consumption += 3 + 5; // 2->0 + 8->1
      break;
    case 29:
      consumption += 3 + 4; // 2->0 + 9->1
      break;
    case 30:
      consumption += 3 + 4; // 3->0 + 0->1
      break;
    case 31:
      consumption += 3 + 0; // 3->0 + 1->1
      break;
  }

  if(*month != 9 && *month != 12)
  {
    consumption += decChange[*month%10];
    ++*month;
    return consumption;
  }
  if(*month == 9)
  {
    consumption += 4 + 2; // 0->1 + 9->0
    *month = 10;
    return consumption;
  }
  consumption += 4 + 5; // 1->0 + 2->1
  *month = 1;

  int shifter = *year;
  while(shifter % 10 == 9){
    consumption += 2;
    shifter /= 10;
  }
  consumption += decChange[shifter%10];
  ++*year;

  return consumption;
}

long long reference(int y1, int m1, int d1, int h1, int i1,
                    int y2, int m2, int d2, int h2, int i2)
{
  long long consumption = 0;
  while(y1 != y2 || m1 != m2 || d1 != d2 || h1 != h2 || i1 != i2)
  {
    consumption += increment(&y1, &m1, &d1, &h1, &i1);
  }
  return consumption;
}

void test(int y1, int m1, int d1, int h1, int i1,
          int y2, int m2, int d2, int h2, int i2, long long expected)
{
  long long reported;
  int succes = energyConsumption( y1, m1, d1, h1, i1,
                                  y2, m2, d2, h2, i2, &reported);
  if(!succes){
    printf("%4d %02d.%02d. %02d:%02d to\n", y1, m1, d1, h1, i1);
    printf("%4d %02d.%02d. %02d:%02d\n", y2, m2, d2, h2, i2);
    assert( 0 );
  }
  if(expected != reported){
    printf("%4d %02d.%02d. %02d:%02d to\n", y1, m1, d1, h1, i1);
    printf("%4d %02d.%02d. %02d:%02d\n", y2, m2, d2, h2, i2);
    printf("%lld / %lld\n", expected, reported);
    assert( 0 );
  }
}

int main ( int argc, char * argv [] )
{
  assert( reference(1600, 1, 1, 0, 0,
                    1600, 1, 1, 0, 0) == 0LL);
  assert( reference(2021, 10,  1, 18, 10,
                    2021, 10,  1, 18, 11) == 204LL);
/*  printf("%lld\n", reference(2021, 10,  1, 00, 59,
                    2021, 10,  1, 01, 00));
*/  assert( reference(2021, 10,  1, 00, 59,
                    2021, 10,  1, 01, 00) == 209LL);

  int y1 = 2021, m1 = 1, d1 = 1, h1 = 0, i1 = 0;
  int y2 = 2021, m2 = 1;
  for(int d2 = 1; d2 <= 12; d2+=2)
  {
    for(int h2 = 0; h2 < 24; h2 = h2*2 + 1)
    {
      for(int i2 = 0; i2 < 60; i2 = (i2+1)*2 )
      {
        printf("%4d %02d.%02d. %02d:%02d to\n", y1, m1, d1, h1, i1);
        printf("%4d %02d.%02d. %02d:%02d\n", y2, m2, d2, h2, i2);
        long long ref = reference(y1, m1, d1, h1, i1,
                                  y2, m2, d2, h2, i2);
        test( y1, m1, d1, h1, i1,
              y2, m2, d2, h2, i2, ref);
      }
    }
  }
  
  assert( reference(2021, 10,  1, 13, 15,
                    2021, 10,  1, 18, 45) == 67116LL);

  assert( leapYearsTo(0) == 0 );
  assert( leapYearsTo(1) == 0 );
  assert( leapYearsTo(4) == 0 );
  assert( leapYearsTo(5) == 1 );
  assert( leapYearsTo(100) == 24 );
  assert( leapYearsTo(101) == 24 );
  assert( leapYearsTo(200) == 48 );
  assert( leapYearsTo(300) == 72 );
  assert( leapYearsTo(400) == 96 );
  assert( leapYearsTo(401) == 97 );
  assert( leapYearsTo(800) == 193);
  assert( leapYearsTo(801) == 194);
  assert( leapYearsTo(4000) == 969);
  assert( leapYearsTo(4001) == 969);

  assert( digitConsumption(0, 1) == 4);
  assert( digitConsumption(1, 2) == 5);
  assert( digitConsumption(2, 3) == 2);
  assert( digitConsumption(3, 4) == 3);
  assert( digitConsumption(4, 5) == 3);
  assert( digitConsumption(5, 6) == 1);
  assert( digitConsumption(6, 7) == 5);
  assert( digitConsumption(7, 8) == 4);
  assert( digitConsumption(8, 9) == 1);
  assert( digitConsumption(9, 10) == 4 + 2);
  assert( digitConsumption(10, 11) == 4);
  assert( digitConsumption(11, 0) == -38);
  assert( digitConsumption(0, 99) == 326);
  assert( digitConsumption(99, 0) == -326);
  assert( digitConsumption(4832, 7946) == 9+3+1+9+3*111*30+11*30+30);

  assert( digitConsumption( 1,  9) ==  24);
  assert( digitConsumption( 1, 10) ==  30);
  assert( digitConsumption( 1, 11) ==  34);
  assert( digitConsumption( 1, 12) ==  39);
  assert( digitConsumption(10, 11) ==   4);
  assert( digitConsumption(10, 12) ==   9);
  assert( digitConsumption(11, 11) ==   0);
  assert( digitConsumption(11, 12) ==   5);
  assert( digitConsumption(12,  1) == -39);
  


  long long int consumption;
  assert ( energyConsumption ( 2000, 1,  1, 00, 00,
                               2001, 1,  1, 00, 00, &consumption ) == 1
           && consumption == YEAR_L_CONSP + 4 );
  assert ( energyConsumption ( 2001, 1,  1, 00, 00,
                               2002, 1,  1, 00, 00, &consumption ) == 1
           && consumption == YEAR_N_CONSP + 5 );
  assert ( energyConsumption ( 2000, 2, 29, 00, 00,
                               2001, 2, 29, 00, 00, &consumption ) == 0 );
  assert ( energyConsumption ( 2000, 3,  1, 00, 00,
                               2001, 3,  1, 00, 00, &consumption ) == 1
           && consumption == YEAR_N_CONSP + 4 );
  assert ( energyConsumption ( 2000, 2, 28, 00, 00,
                               2001, 2, 28, 00, 00, &consumption ) == 1
           && consumption == YEAR_L_CONSP + 4 );



  test( 2021, 10,  1, 13, 15,
        2021, 10,  2, 13, 15, DAY_CONSP+5);

  test( 2021, 10,  1, 13, 15,
        2021, 10,  2, 11, 15, DAY_CONSP+5 - HOUR_CONSP*2 - 7);

  test( 2021, 10,  1, 13, 15,
        2021, 10,  2, 11, 20, DAY_CONSP+5 - HOUR_CONSP*2 - 7 + 5*MINUTE_CONSP + 5 + 13);

  test( 2021, 10,  1, 13, 15,
        2021, 10,  1, 13, 20, 5*MINUTE_CONSP + 5 + 13);

  test( 2021, 10,  1, 13, 15,
        2021, 10,  1, 14, 00, 45*MINUTE_CONSP + 3 + 5+11 + 13+4*30);

  test( 2021, 10,  1, 13, 15,
        2021, 10,  2, 00, 00, 10*HOUR_CONSP + 30 + 8 + 5 + 45*MINUTE_CONSP + 3 + 5+11 + 13+4*30);

  test( 2021, 10,  1, 13, 15,
        2021, 10,  2, 11, 00, 10*HOUR_CONSP + 30 + 8 + 5 + 45*MINUTE_CONSP + 3 + 5+11 + 13+4*30 + 11*HOUR_CONSP + 4 + 4 +30);

  test( 2021, 10,  1, 13, 15,
        2021, 10,  2, 11, 20, 10*HOUR_CONSP + 30 + 8 + 5 + 45*MINUTE_CONSP + 3 + 5+11 + 13+4*30 + 11*HOUR_CONSP + 4 + 4 +30 + 20*MINUTE_CONSP + 9 + 2*30);


  printf("%lld\n", reference(2021, 10,  1, 13, 15,
                             2021, 10,  2, 11, 20));
  printf("%lld\n", 10*HOUR_CONSP + 30 + 8 + 5 + 45*MINUTE_CONSP + 3 + 5+11 + 13+4*30 + 11*HOUR_CONSP + 4 + 4 +30 + 20*MINUTE_CONSP + 9 + 2*30);
  printf("%lld\n", DAY_CONSP+5 - HOUR_CONSP*2 - 7 + 5*MINUTE_CONSP + 5 + 13);

  test( 2021, 10,  1, 13, 15,
        2021, 10,  2, 11, 20, 269497LL);


  assert ( energyConsumption ( 2021, 10,  1, 13, 15,
                               2021, 10,  1, 18, 45, &consumption ) == 1
           && consumption == 67116LL );
  assert ( energyConsumption ( 2021, 10,  1, 13, 15,
                               2021, 10,  2, 11, 20, &consumption ) == 1
           && consumption == 269497LL );
  assert ( energyConsumption ( 2021,  1,  1, 13, 15,
                               2021, 10,  5, 11, 20, &consumption ) == 1
           && consumption == 81106033LL );
  assert ( energyConsumption ( 2024,  1,  1, 13, 15,
                               2024, 10,  5, 11, 20, &consumption ) == 1
           && consumption == 81398919LL );
  assert ( energyConsumption ( 1900,  1,  1, 13, 15,
                               1900, 10,  5, 11, 20, &consumption ) == 1
           && consumption == 81106033LL );
  assert ( energyConsumption ( 2021, 10,  1,  0,  0,
                               2021, 10,  1, 12,  0, &consumption ) == 1
           && consumption == 146443LL );
  assert ( energyConsumption ( 2021, 10,  1,  0, 15,
                               2021, 10,  1,  0, 25, &consumption ) == 1
           && consumption == 2035LL );
  assert ( energyConsumption ( 2021, 10,  1, 12,  0,
                               2021, 10,  1, 12,  0, &consumption ) == 1
           && consumption == 0LL );
  assert ( energyConsumption ( 2021, 10,  1, 12,  0,
                               2021, 10,  1, 12,  1, &consumption ) == 1
           && consumption == 204LL );
  assert ( energyConsumption ( 2021, 11,  1, 12,  0,
                               2021, 10,  1, 12,  0, &consumption ) == 0 );
  assert ( energyConsumption ( 2021, 10, 32, 12,  0,
                               2021, 11, 10, 12,  0, &consumption ) == 0 );
  assert ( energyConsumption ( 2100,  2, 29, 12,  0,
                               2100,  2, 29, 12,  0, &consumption ) == 0 );
  assert ( energyConsumption ( 2400,  2, 29, 12,  0,
                               2400,  2, 29, 12,  0, &consumption ) == 1
           && consumption == 0LL );
  return 0;
}
#endif /* __PROGTEST__ */



int compareDate( int y1, int m1, int d1, int h1, int i1,
                 int y2, int m2, int d2, int h2, int i2)
{
  if(y2 < y1) return 0;
  if(y2 > y1) return 1;

  if(m2 < m1) return 0;
  if(m2 > m1) return 1;

  if(d2 < d1) return 0;
  if(d2 > d1) return 1;

  if(h2 < h1) return 0;
  if(h2 > h1) return 1;

  if(i2 < i1) return 0;
  return 1;
}

int validateDate(int year, int month, int day)
{
  if(year < 1600) return 0;
  int leapYear = isLeapYear(year);
  return 1 <= month && month <= 12 && 
         1 <= day && day <= ((leapYear && month == 2) ? monthDays[month]+1:monthDays[month]);
}

int validateDayTime(int hour, int minute)
{
  return 0 <= hour && hour < 60 && 0 <= minute && minute < 60;
}

long long minuteConsumption(int i1, int i2)
{
  long long consumption = ( i2 - i1 ) * MINUTE_CONSP;
  consumption += digitConsumption(i1, i2);
  return consumption;
}
long long hourConsumption(int h1, int h2)
{
  long long consumption = ( h2 - h1 ) * HOUR_CONSP;
  consumption += digitConsumption(h1, h2);
  return consumption;
}
long long dayConsumption(int d1, int d2)  //Co ak feb 29 je d2 ??
{
  long long consumption = ( d2 - d1 ) * DAY_CONSP;
  consumption += digitConsumption(d1, d2);
  return consumption;
}
long long monthConsumption(int m1, int m2, int leap)
{
  long long consumption = 0, cns = 0;
  for(int month = 1; month < m1; month++)
  {
    switch( monthDays[month] )
    {
      case 28:
        cns = leap ? MONTH_29_CONSP : MONTH_28_CONSP;
        break;
      case 30:
        cns = MONTH_30_CONSP;
        break;
      case 31:
        cns = MONTH_31_CONSP;
        break;
    }
    consumption -= cns;
  }

  for(int month = 1; month < m2; month++)
  {
    switch( monthDays[month] )
    {
      case 28:
        cns = leap ? MONTH_29_CONSP : MONTH_28_CONSP;
        break;
      case 30:
        cns = MONTH_30_CONSP;
        break;
      case 31:
        cns = MONTH_31_CONSP;
        break;
    }
    consumption += cns;
  }
  consumption += digitConsumption(m1, m2);

  return consumption;
}

long long yearConsumption(int y1, int month, int y2)
{
  int years_norm = y2 - y1, years_leap = leapYearsBetw(y1, y2);

  if(isLeapYear(y1) && month > 2) years_leap--;
  if(isLeapYear(y2) && month > 2) years_leap++;

  years_norm -= years_leap;

  long long consumption = years_norm * YEAR_N_CONSP + years_leap * YEAR_L_CONSP;
  consumption += digitConsumption(y1, y2);  

  return consumption;
}

//including from, excluding to
int leapYearsBetw(int from, int to)
{
  return leapYearsTo(to) - leapYearsTo(from);
}

int leapYearsTo(int years)
{
  return (years-1)/4 - (years-1)/100 + (years-1)/400 - (years-1)/4000;
}

int isLeapYear(int year)
{
  return ((year % 4 == 0) && (year % 100 != 0)) || ((year % 400 == 0) && (year % 4000 != 0));
}

long long digitConsumption(int from, int to)
{
  long long consumption = 0;
  long long multip = 0;
  while(from != to)
  {
    consumption += decadic_prefix[to % 10] - decadic_prefix[from % 10];
    consumption += (to%10 - from%10) * multip * TEN_SEC_CONSP;
    multip = 10*multip + 1;
    from /= 10;
    to /= 10;
  }
  return consumption;
}
