#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>
#include <assert.h>
#endif /* __PROGTEST__ */

typedef struct domino {
    char * edice;
    int a,b,c,d;
}Tdomino;


int countCollectible ( const char * list )
{
    const char * example = "Progtest Limited";
    const char * ptr = list;
    int len = strlen(list);
    Tdomino ** arr = (Tdomino ** ) malloc (1 * sizeof (Tdomino *));
    int cntForArr = 0;
    while ( * ptr != list[len]) {
        int cntApos = 0, cnt = 0, maxsize = 100;
        char * str = NULL;
        str = (char *)malloc(maxsize * sizeof(char));
        while (1) {
            if (* ptr == list[len]) break;
            if (*ptr == '\'') {
                cntApos++;
            }
            if (cntApos == 2) {
                str[cnt] = '\0';
                ptr++;
                break;
            }
            if (cnt + 1 == maxsize) {
                maxsize *= 2;
                str = (char *)realloc(str,maxsize * sizeof (char));
            }
            if (cntApos == 1 ) {
                if (*ptr != '\'') {
                    str[cnt] = *ptr;
                    cnt++;
                }
            }
            ptr++;
        }
        if (strcmp(example,str) == 0) {
            Tdomino * data = NULL;
            data = (Tdomino *) malloc ( 1 * sizeof (Tdomino));
            data->edice = str;
            sscanf(ptr, " ; [ %d | %d ] ; [ %d | %d ] } ",&data->a,&data->b,&data->c,&data->d);
            if ( ((data->a + data->b == 67) || (data->c + data->d == 67)) && ((data->a + data->b != 67) || (data->c + data->d != 67)) ) {
                cntForArr++;
                arr = (Tdomino **) realloc (arr, cntForArr * sizeof (Tdomino *));
                arr[cntForArr - 1] = data;
            } else {
                free(str);
                free(data);
            }
            while (* ptr != '}') { ptr++; }
        } else {
            free(str);
        }
    }
    for (int j = 0; j < cntForArr; j++) {
        free(arr[j]->edice);
        free(arr[j]);
    }
    free(arr);
    return cntForArr;
}

int minInt (int a ,int b ) {
    if (a > b) {
        return b;
    }
    return a;
}

int maxInt (int a ,int b ) {
    if (a > b) {
        return a;
    }
    return b;
}

void compareForComp (Tdomino * data) {
    int maxX,maxY,minX,minY;
    maxX = maxInt(data->a,data->b);
    maxY = maxInt(data->c,data->d);
    minX = minInt(data->a,data->b);
    minY = minInt(data->c,data->d);
    if (minX < minY) {
        data->a = minX;
        data->b = maxX;
        data->c = minY;
        data->d = maxY;
    } else if (minX > minY) {
        data->a = minY;
        data->b = maxY;
        data->c = minX;
        data->d = maxX;
    } else {
        if (maxX < maxY) {
            data->a = minX;
            data->b = maxX;
            data->c = minY;
            data->d = maxY;
        } else {
            data->a = minY;
            data->b = maxY;
            data->c = minX;
            data->d = maxX;
        }
    }
}


int compare(const void *a, const void *b)
{
    Tdomino * data1 = (Tdomino *)a;
    Tdomino * data2 = (Tdomino *)b;
    int result = strcmp(data1->edice, data2->edice);
    if (result) return result;
    if (data1->a != data2->a) {
        return data1->a - data2->a;
    }
    if (data1->b != data2->b) {
        return data1->b - data2->b;
    }
    if (data1->c != data2->c) {
        return data1->c - data2->c;
    }
    if (data1->d != data2->d) {
        return data1->d - data2->d;
    }
    return 0;
}

int search (Tdomino * array, int cnt) {
    Tdomino * prev = &array[0];
    int result = 1;
    for (int i = 0; i < cnt; i++ ) {
        Tdomino * tmp = &array[i];
        if (compare(prev,tmp)) {
            result++;
            prev = &array[i];
        }
    }
    return result;
}

int countUnique  ( const char * list )
{
    int maxsize2 = 1000;
    const char * ptr = list;
    int len = strlen(list);
    Tdomino * arr = (Tdomino * ) malloc (maxsize2 * sizeof (Tdomino ));
    int cntForArr = 0;
    while ( * ptr != list[len]) {
        int cntApos = 0, cnt = 0, maxsize = 1000;
        char *str = NULL;
        str = (char *) malloc(maxsize * sizeof(char));
        while (1) {
            if (*ptr == list[len]) break;
            if (*ptr == '\'') {
                cntApos++;
            }
            if (cntApos == 2) {
                str[cnt] = '\0';
                ptr++;
                break;
            }
            if (cnt + 1 == maxsize) {
                maxsize *= 2;
                str = (char *) realloc(str, maxsize * sizeof(char));
            }
            if (cntApos == 1) {
                if (*ptr != '\'') {
                    str[cnt] = *ptr;
                    cnt++;
                }
            }
            ptr++;
        }
        if (cnt != 0) {
            cntForArr++;
            if (cntForArr + 1 == maxsize2) {
                maxsize2 *= 2;
                arr = (Tdomino *) realloc(arr, maxsize2 * sizeof(Tdomino));
            }
            //Tdomino * data = NULL;
            //data = (Tdomino *) malloc(1 * sizeof(Tdomino));
            //data->edice = str;
            sscanf(ptr, " ; [ %d | %d ] ; [ %d | %d ] } ", &arr[cntForArr - 1].a,&arr[cntForArr - 1].b, &arr[cntForArr - 1].c, &arr[cntForArr - 1].d);
            arr[cntForArr - 1].edice = str;
            compareForComp(&arr[cntForArr - 1]);
            //arr[cntForArr - 1] = data;
            while (*ptr != '}') { ptr++; }
        } else {
            free(str);
        }
    }
    qsort( arr,cntForArr, sizeof(Tdomino), compare);
    int res = search(arr,cntForArr);
    for (int j = 0; j < cntForArr; j++) {
        free(arr[j].edice);
    }
    free(arr);
    return res;
}

uint64_t countTowers ( const char * list )
{
    return 0; /* TODO */
}

#ifndef __PROGTEST__
int main ( void )
{
    const char * str1 =
            " { 'Progtest Exam' ; [ 1 | 2 ] ; [ 3 | 4 ] }{'PA1 2020/2021';[2|2];[3|1]}\n"
            "{'Progtest Exam' ; [ 2 | 1 ] ; [ 3 | 4 ] }\n"
            "{'Progtest Exam' ; [ 2 | 3 ] ; [ 1 | 4 ] }\n";
    const char * str2 =
            "{'Crash';  [1|2];[3|4]}\n"
            "{'MemLeak';[1|2];[3|4]}\n"
            "{'MemLeak';[2|3];[3|1]}\n"
            "{'MemLeak';[1|3];[3|2]}\n"
            "{'MemLeak';[1|4];[1|5]}\n"
            "{'MemLeak';[4|1];[1|5]}\n"
            "{'MemLeak';[4|1];[5|1]}\n"
            "{'MemLeak';[1|4];[5|1]}\n"
            "{'MemLeak';[1|5];[1|4]}\n"
            "{'MemLeak';[5|1];[1|4]}\n"
            "{'MemLeak';[5|1];[4|1]}\n"
            "{'MemLeak';[1|5];[4|1]}\n";
    const char * str3 =
            "{'-Wall -pedantic -Werror -Wno-long-long -O2';[2|2];[3|3]}\n"
            "{'-Wall -pedantic -Werror -Wno-long-long -O2';[4|4];[5|5]}\n"
            "{'-Wall -pedantic -Werror -Wno-long-long -O2';[3|4];[4|5]}\n";
    const char * str4 =
            "{'-fsanitize=address -g';[1|5];[5|7]}\n"
            "{'-fsanitize=address -g';[3|7];[1|9]}\n"
            "{'-fsanitize=address -g';[2|2];[4|7]}\n"
            "{'-fsanitize=address -g';[3|9];[2|6]}\n"
            "{'-fsanitize=address -g';[2|2];[2|2]}\n";

    assert ( countCollectible ( str1 ) == 0 );
    assert ( countUnique ( str1 ) == 3 );
    assert ( countUnique ( str2 ) == 4 );
    assert ( countUnique ( str3 ) == 3 );
    assert ( countUnique ( str4 ) == 5 );
    assert ( countTowers ( str3 ) == 10 );
    assert ( countTowers ( str4 ) == 114 );
    return 0;
}
#endif /* __PROGTEST__ */