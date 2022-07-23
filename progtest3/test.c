#ifndef __PROGTEST__
#include <stdio.h>
#include <assert.h>

#endif /* __PROGTEST__ */

int isYearLeap (int year) {
    if (year % 4000 == 0) return 0;
    if (year % 400 == 0) return 1;
    if (year % 100 == 0) return 0;
    if (year % 4 == 0) return 1;
    return 0;
}

int daysInMonth(int year, int month) {
    switch (month) {
        case 1: return 31;
        case 2: return 28 + isYearLeap(year);
        case 3: return 31;
        case 4: return 30;
        case 5: return 31;
        case 6: return 30;
        case 7: return 31;
        case 8: return 31;
        case 9: return 30;
        case 10: return 31;
        case 11: return 30;
        case 12: return 31;
    }
    return 0;
}

int checkInput (int y,int m,int d,int h,int min) {
    if ( (h >= 0 && h<= 23 ) && (m >= 0 && m<=12) &&  (min >= 0 && min <= 59) && (d >= 1 && d<=daysInMonth(y,m)) && (y >= 1600)) {
        return 1;
    } else {
        return 0;
    }
}

int counterCuckoo (int flag, int h,int min) {
    int result = 0;
    if (flag == 1) {
        for (int i = h * 60 + min + 1; i < 1440; i++) {
            //printf ("time = (%d:%d), i = %d, result = %d\n",i/60,i%60,i,result);
             if (i % 60 == 0 && i != 0) {
                if ((i / 60) % 12 == 0) {
                    result+=12;
                } else {
                    result += (i / 60) % 12;
                }
            } else if (i % 60 == 30 && i != 0) {
                result += 1;
            }
        }
    } else if (flag == 0) {
        for (int i = 0; i < h * 60 + min; i++) {
            //printf ("time = (%d:%d), i = %d, result = %d\n",i/60,i%60,i,result);
            if (i == 0) {
                result += 12;
            } else if (i % 60 == 0 ) {
                if ((i / 60) % 12 == 0) {
                    result+=12;
                } else {
                    result += (i / 60) % 12;
                }
            } else if (i % 60 == 30) {
                result += 1;
            }
        }
    }
    return result;
}

int counterOfDays ( int flag, int y,int m, int d) {
    int days = 0;
    if (flag == 1) {
        for (int i = m + 1 ; i <= 12; i++) {
            days+= daysInMonth(y,i);
        }
        days+=daysInMonth(y,m) - d;
    } else if (flag == 0 ) {
        for (int i = 1; i < m; i++) {
            days+=daysInMonth(y,i);
        }
        d--;
        days+=d;
    }
    return days;
}




int cuckooClock ( int y1, int m1, int d1, int h1, int i1,
                  int y2, int m2, int d2, int h2, int i2, long long int * cuckoo ) {
    //printf("h2 = %d, m2 = %d\n",h2,i2);
    long long int days;
    long long int result = 0;
    long long int x1 = y1 * 365 * 525600LL+ m1 * 31LL * 1440 + d1 * 24LL * 60 + h1 * 60LL + i1;
    long long int x2 = y2 * 365 * 525600LL + m2 * 31LL * 1440 + d2 * 24LL * 60 + h2 * 60LL + i2;
    if (checkInput(y1,m1,d1,h1,i1) == 1 && checkInput(y2,m2,d2,h2,i2) == 1 && x1 <= x2 ) {
        if (y1 < y2) {
            result =180 - counterCuckoo(0,h1,i1);
            days = counterOfDays(1,y1,m1,d1);
            y1++;
            while (y1!=y2) {
                if (isYearLeap(y1)) {
                    days += 366;
                    y1++;
                } else {
                    days += 365;
                    y1++;
                }
            }
            result +=180 - counterCuckoo(1,h2,i2);
            //printf ("do %lld\n",result);
            days+=counterOfDays(0,y2,m2,d2);
            //printf ("days = %lld\n",days);
            result +=(long long)days * 180ll;
        } else if (y1 == y2) {
            if (m1 == m2) {
                if (d1 == d2) {
                    result =180 - counterCuckoo(1,h2,i2) - counterCuckoo(0,h1,i1);
                } else if (d1 < d2) {
                    //printf ("do %lld\n",result);
                    result = 180 - counterCuckoo(0,h1,i1);
                    //printf ("+Left %lld\n",result);
                    result+= 180 - counterCuckoo(1,h2,i2);
                    //printf ("+Right %lld\n",result);
                    days = d2 - d1 - 1;
                    result+=(long long) days * 180LL;
                }
            } else if (m1 < m2) {
                //printf ("do %lld\n",result);
                result+=180 - counterCuckoo(0,h1,i1);
                //printf ("+Left %lld\n",result);
                result+=180 - counterCuckoo(1,h2,i2);
                //printf ("+Right %lld\n",result);
                int days1 = d1;
                int days2 = d2;
                for (int i = 1; i < m1; i++) {
                    days1+=daysInMonth(y1,i);
                }
                for (int j = 1; j < m2; j++) {
                    days2+=daysInMonth(y2,j);
                }
                days = days2 - days1 - 1;
                result+=(long long) days * 180;
            }
        }
        //printf("%lld\n",result);
        * cuckoo = result;

        return 1;
    }
    return 0;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
    long long int cuckoo;
    assert ( cuckooClock(2015, 6, 21 ,8, 37,
                         2172, 5 ,16 ,7, 36, &cuckoo) == 1 && cuckoo == 10315431);

    assert ( cuckooClock(2000,2,1,0,0,
                         2000,3,1,0,0,&cuckoo) == 1 && cuckoo == 5232);
    assert ( cuckooClock(2000,1,1,0,0,
                         2000,1,1,0,30, &cuckoo) == 1 && cuckoo == 13 );
    assert ( cuckooClock(2010,1,1,0,0,
                         2010,1,2,0,0,&cuckoo) == 1 && cuckoo == 192);
    //assert ( cuckooClock(2010,1,1,0,0,2013,1,1,0,0, & cuckoo) == 1 && cuckoo == 197292);
    //assert ( cuckooClock(2010,1,1,0,0,2011,1,1,0,30, & cuckoo) == 1 && cuckoo == 65713);
    assert ( cuckooClock(2013, 12, 31, 23, 25,
                         2014, 1, 1, 0, 30, &cuckoo) == 1  && cuckoo == 14);

    assert ( cuckooClock ( 2013, 10,  1, 13, 15,
                           2013, 10,  1, 18, 45, &cuckoo ) == 1 && cuckoo == 26 );
    assert ( cuckooClock ( 2013, 10,  1, 13, 15,
                           2013, 10,  2, 11, 20, &cuckoo ) == 1 && cuckoo == 165 );
    assert ( cuckooClock ( 2013,  1,  1, 13, 15,
                           2013, 10,  5, 11, 20, &cuckoo ) == 1 && cuckoo == 49845 );
    assert ( cuckooClock ( 2012,  1,  1, 13, 15,
                           2012, 10,  5, 11, 20, &cuckoo ) == 1 && cuckoo == 50025 );
    assert ( cuckooClock ( 1900,  1,  1, 13, 15,
                           1900, 10,  5, 11, 20, &cuckoo ) == 1 && cuckoo == 49845 );
    assert ( cuckooClock ( 2013, 10,  1,  0,  0,
                           2013, 10,  1, 12,  0, &cuckoo ) == 1 && cuckoo == 102 );
    assert ( cuckooClock ( 2013, 10,  1,  0, 15,
                           2013, 10,  1,  0, 25, &cuckoo ) == 1 && cuckoo == 0 );
    assert ( cuckooClock ( 2013, 10,  1, 12,  0,
                           2013, 10,  1, 12,  0, &cuckoo ) == 1 && cuckoo == 12 );
    assert ( cuckooClock ( 2013, 11,  1, 12,  0,
                           2013, 10,  1, 12,  0, &cuckoo ) == 0 );
    assert ( cuckooClock ( 2013, 10, 32, 12,  0,
                           2013, 11, 10, 12,  0, &cuckoo ) == 0 );
    assert ( cuckooClock ( 2100,  2, 29, 12,  0,
                           2100,  2, 29, 12,  0, &cuckoo ) == 0 );
    assert ( cuckooClock ( 2400,  2, 29, 12,  0,
                           2400,  2, 29, 12,  0, &cuckoo ) == 1 && cuckoo == 12 );
    assert ( cuckooClock ( 2020, 11, 1, 12, 0, 2020, 10, 1, 12, 0, &cuckoo ) == 0);
    //assert ( cuckooClock ( 1939,  4, 14, 2, 15, 2051452918, 5 , 23, 11, 32, &cuckoo) == 1 && cuckoo == 134869782847393);
    assert ( cuckooClock ( 1824, 1, 1, 0, 0, 1824, 1, 6, 0, 0, &cuckoo) == 1 && cuckoo == 912);
    return 0;
}
#endif /* __PROGTEST__ */