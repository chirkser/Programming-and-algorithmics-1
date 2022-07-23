#include <stdio.h>
#include <math.h>
#include <values.h>

int FreeNum (int x) {
    if (x % 2 == 0) {
        x/=2;
    }
    if (x % 2 == 0) {
        return 0;
    }
    for (int i = 3; i <= x; i += 2) {
        if (x % i == 0) {
            x /= i;
            if (x % i == 0) {
                return 0;
            }
        }
    }
    return 1;
}


int main() {
    int x,cnt=0,y=0;
    printf ("Poradi:\n");
    while (1) {
        if (scanf(" %d",&x)!=1 || x <= 0) {
            if (feof(stdin)) {
                return 0;
            }
            printf ("Nespravny vstup.\n");
            return 1;
        }
        //printf ("x = %d\n",x);
        while (cnt != x) {
            y++;
            if (FreeNum(y)) {
                cnt++;
            }
            //printf ("Kol-vo: %d\n",cnt);
            //printf ("\n");
        }
        printf ("= %d\n",y);
        cnt = 0;
        y = 0;
        x = 0;
    }
    return 0;
}
