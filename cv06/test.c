#ifndef __PROGTEST__
#include <stdio.h>
#include <assert.h>
#endif /* __PROGTEST__ */

int isLeapYear(int year){
    return ((year % 4 == 0) && (year % 100 != 0)) || ((year % 400 == 0) && (year % 4000 != 0));
}

int dateToIndex(int day, int month, int year, int *idx) {
    /* TODO: Your code here */
    if(year < 2000 || month < 1 || month > 12 || day < 1) return 0;
    int leapYear = isLeapYear(year);
    int monthDays[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if(leapYear) monthDays[1] = 29;
    if(monthDays[month-1] < day) return 0;
    *idx = 0;
    for(int i = 1; i < month; i++)
        *idx += monthDays[i-1];
    *idx += day;
    return 1;
}


#ifndef __PROGTEST__
int main (int argc, char * argv []) {
    int idx = -1;
    assert(isLeapYear(2000) == 1);
    assert(isLeapYear(2001) == 0);
    assert(isLeapYear(2002) == 0);
    assert(isLeapYear(2003) == 0);
    assert(isLeapYear(2004) == 1);
    assert(isLeapYear(2100) == 0);
    assert(isLeapYear(2400) == 1);
    assert(isLeapYear(4000) == 0);
    assert(dateToIndex( 1,  1, 2000, &idx) == 1 && idx == 1);
    assert(dateToIndex( 1,  2, 2000, &idx) == 1 && idx == 32);
    assert(dateToIndex(29,  2, 2000, &idx) == 1 && idx == 60);
    assert(dateToIndex(29,  2, 2001, &idx) == 0);
    assert(dateToIndex( 1, 12, 2000, &idx) == 1 && idx == 336);
    assert(dateToIndex(31, 12, 2000, &idx) == 1 && idx == 366);
    assert(dateToIndex( 1,  1, 1999, &idx) == 0);
    assert(dateToIndex( 6,  7, 3600, &idx) == 1 && idx == 188);
    assert(dateToIndex(29,  2, 3600, &idx) == 1 && idx == 60);
    assert(dateToIndex(29,  2, 4000, &idx) == 0);
    return 0;
}
#endif /* __PROGTEST__ */

