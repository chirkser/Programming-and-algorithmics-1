#include <stdio.h>
#include <stdlib.h>

#define MAX 10000

int isTriangle (long long int a,long long int b, long long int c) {
    return a + b > c;
}

int forQsort (const long long int * a, const long long  int * b) {
    return *a - *b;
}

int main() {
    printf("Delky nosniku:\n");
    long long int * arr = NULL;
    long long int num;
    long long int cnt = 0, maxsize = 0;
    while (1) {
        if (scanf (" %lld",&num) != 1 || num <= 0 ) {
            if (feof(stdin)) {
                break;
            }
            printf ("Nespravny vstup.\n");
            return 1;
        }
        if (cnt >= maxsize) {
            maxsize+= (maxsize < 100) ? 10 : maxsize/2;
        }
        arr =(long long int *)realloc(arr,maxsize * sizeof(arr));
        arr[cnt] = num;
        cnt++;
    }
    if (cnt < 3 || cnt > 10000) {
        printf ("Nespravny vstup.\n");
        return 1;
    }
    maxsize = 0;
    qsort(arr,cnt,sizeof(long long int),(int(*) (const void *, const void *)) forQsort);
    long long int streak = 0;
    long long int * newArr = NULL;
    long long int newCNT = 0;
    for (int i = 0; i < cnt; ++i) {
        if (i > 0 && arr[i] == arr[i-1]) {
            streak++;
        } else {
            streak = 0;
        }
        if (streak != 4) {
            if (newCNT >= maxsize) {
                maxsize+= (maxsize < 100) ? 10 : maxsize/2;
            }
            newArr = (long long int *)realloc(newArr, maxsize * sizeof(newArr));
            newArr[newCNT]=arr[i];
            newCNT++;
        }
    }
    long long int result = 0;
    for (long long int i = 0; i < newCNT - 2; ++i ) {
        if (i > 0 && newArr[i] == newArr[i-1]) {
            continue;
        }
        for (long long int j = i + 1;  j < newCNT - 1; ++j) {
            if (j >  i + 1 && newArr[j] == newArr[j-1]) {
                continue;
            }
            for (long long int z = j + 1; z < newCNT; ++z) {
                if (z > j + 1 && newArr[z] == newArr[z-1]) {
                    continue;
                }
                if (isTriangle(newArr[i],newArr[j],newArr[z])) {
                    //printf ("arr[%d] = %d, arr[%d] =  %d, arr[%d] =  %d - je to trojuhelnik.\n",i,arr[i],j,arr[j],z,arr[z]);
                    result++;
                }
            }
        }
    }
    free(arr);
    free(newArr);
    printf ("Trojuhelniku: %lld\n",result);
    return 0;
}
