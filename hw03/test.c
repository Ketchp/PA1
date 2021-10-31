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
//                                                                  YYYY|MM|DD|hh|mm|ss
//                                                                  0000|01|01|00|00|00
#define TEN_SEC_CONSP (30LL)                        //       30     0000|01|01|00|00|10
#define MINUTE_CONSP (30LL*6 + 20)                  //      200     0000|01|01|00|01|00
#define TEN_MINUTE_CONSP (200LL*10 + 30)            //     2030     0000|01|01|00|10|00
#define HOUR_CONSP (2030LL*6+20)                    //    12200     0000|01|01|01|00|00
#define TEN_HOUR_CONSP (12200LL*10 + 30)            //   122030     0000|01|01|10|00|00
#define DAY_CONSP (122030LL*2 + 12 +12200*4 + 14)   //   292886     0000|01|02|00|00|00

int compareDate( int y1, int m1, int d1, int h1, int i1,
                 int y2, int m2, int d2, int h2, int i2);
int validateDate(int year, int month, int day);
int validateDayTime(int hour, int minute);

long long dateConsumption(int y1, int m1, int d1, int y2, int m2, int d2);
long long timeConsumption(int h1, int i1, int h2, int i2);
long long daysCount(int year, int month, int day);

long long digitConsumption(int from, int to);

int isLeapYear(int year);
int leapYearsTo(int year);

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

  *consumption = dateConsumption( y1, m1, d1, y2, m2, d2);
  *consumption += timeConsumption(h1, i1, h2, i2);
  return 1;
}



#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
  assert( isLeapYear(0) == 0);
  assert( isLeapYear(1) == 0);
  assert( isLeapYear(4) == 1);
  assert( isLeapYear(100) == 0);
  assert( isLeapYear(400) == 1);
  assert( isLeapYear(4000) == 0);
  assert( isLeapYear(4400) == 1);

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

  assert( daysCount(     0,  1,  1) ==     1 );
  assert( daysCount(     0,  1,  2) ==     2 );
  assert( daysCount(     0,  2,  1) ==    32 );
  assert( daysCount(     0,  2, 28) ==    59 );
  assert( daysCount(     0,  3,  1) ==    60 );
  assert( daysCount(     1,  1,  1) ==   366 );
  assert( daysCount(     2,  1,  1) ==   731 );
  assert( daysCount(     4,  1,  1) ==  1461 );
  assert( daysCount(     4,  2,  1) ==  1492 );
  assert( daysCount(     4,  2, 28) ==  1519 );
  assert( daysCount(     4,  2, 29) ==  1520 );
  assert( daysCount(     4,  3,  1) ==  1521 );
  assert( daysCount(     8,  3,  1) ==  2982 );
  assert( daysCount(    12,  3,  1) ==  4443 );
  assert( daysCount(    96,  3,  1) == 35124 );
  assert( daysCount(   100,  2, 28) == 36583 );
  assert( daysCount(   100,  3,  1) == 36584 );
  


  long long int consumption;

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


long long dateConsumption(int year1, int month1, int day1, int year2, int month2, int day2)
{
  return ( daysCount(year2, month2, day2) - daysCount(year1, month1, day1) ) * DAY_CONSP;
}

long long daysCount(int year, int month, int day)
{
  static int monthPrefix[] = {0,   0,  31,  59,  90, 120, 151, 181, 212, 243, 273, 304, 334};
  return year*365ll + leapYearsTo(year) + monthPrefix[month] + day + (month > 2)*isLeapYear(year);
}

long long timeConsumption(int hour1, int minute1, int hour2, int minute2)
{
  long long consumption = ( minute2 - minute1 ) * MINUTE_CONSP;
  consumption += digitConsumption( minute1, minute2);

  consumption += ( hour2 - hour1 ) * HOUR_CONSP;
  consumption += digitConsumption( hour1, hour2);

  return consumption;
}



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
  static int monthDays[] =   {0,  31,  28,  31,  30,  31,  30,  31,  31,  30,  31,  30,  31};
  if(year < 1600) return 0;
  int leapYear = isLeapYear(year);
  return 1 <= month && month <= 12 && 
         1 <= day && day <= ((leapYear && month == 2) ? monthDays[month]+1:monthDays[month]);
}

int validateDayTime(int hour, int minute)
{
  return 0 <= hour && hour < 24 && 0 <= minute && minute < 60;
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
  static long long decadic_prefix[] = {0,  4,  9, 11, 14, 17, 18, 23, 27, 28, 30};
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
