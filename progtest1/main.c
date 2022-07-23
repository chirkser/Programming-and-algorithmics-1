#include <stdio.h>
#include <math.h>
#include <float.h>

#define M_PI 3.14159265358979323846
//// kružnice se protínají
double intersection(double r1,double r2,double distance) {
    if (distance > fabs(r1-r2) && distance < (r1+r2)) {
        return 1;
    }
    return 0;
}
//// kružnice splývají
double equality (double r1, double r2, double distance) {
    if (fabs(r1 - r2) < 100 * DBL_EPSILON * fmax(r1,r2) && distance == 0) {
        return 1;
    }
    return 0;
}
//// jedna kružnice leží zcela uvnitř druhé
double in_area (double r1, double r2, double distance) {
    if (distance < (r1-r2)) {
        return 1;
    }
    return 0;
}
//// jedna kružnice se zevnitř dotýká druhé
double touch_in (double r1,double r2,double distance) {
    if (fabs(distance - (r1-r2)) < 100 * DBL_EPSILON * distance) {
        return 1;
    }
    return 0;
}
//// kružnice se dotýkají zvenku
double touch_out (double r1,double r2,double distance) {
    if (fabs(distance - (r1 + r2)) < 1000 * DBL_EPSILON * distance) {
        return 1;
    }
    return 0;
}

//// plocha kruznice
double area (double r) {
    return M_PI * r * r;
}

int main() {
    double x1,y1,r1;
    printf ("Zadejte parametry kruznice #1:\n");
    if (scanf ("%lf %lf %lf",&x1,&y1,&r1)!=3
        || r1 <= 0 ) {
        printf ("Nespravny vstup.\n");
        return 1;
    }
    printf ("Zadejte parametry kruznice #2:\n");
    double x2,y2,r2;
    if (scanf ("%lf %lf %lf",&x2,&y2,&r2)!=3
        || r2 <= 0 ) {
        printf ("Nespravny vstup.\n");
        return 1;
    }
    double S1 = area(r1);
    double S2 = area(r2);
    double distance = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
    double F1 = r1 * r1 * acos((distance*distance + r1 * r1 - r2 * r2)/(2 * distance * r1));
    double F2 = r2 * r2 * acos((distance*distance + r2 * r2 - r1 * r1)/(2 * r2 * distance));
    double F3 = sqrt((-distance+r1+r2)*(distance+r1-r2)*(distance-r1+r2)*(distance+r1+r2))/2;
    //printf ("distance = %lf\nsumr1r2 = %lf\nRozdil = %g\n",distance,r1+r2,distance-(r1+r2));
    if (equality(r1,r2,distance)) {
        printf ("Kruznice splyvaji, prekryv: %lf\n",S1);
        return 1;
    }
    if (touch_out(r1,r2,distance)) {
        printf ("Vnejsi dotyk, zadny prekryv.\n");
        return 1;
    }
    if (in_area(r1,r2,distance) && !touch_in(r1,r2,distance)) {
        printf ("Kruznice #2 lezi uvnitr kruznice #1, prekryv: %lf\n",S2);
        return 1;
    }
    if (in_area(r2,r1,distance) && !touch_in(r2,r1,distance)) {
        printf ("Kruznice #1 lezi uvnitr kruznice #2, prekryv: %lf\n",S1);
        return 1;
    }
    if (touch_in(r1,r2,distance)) {
        printf ("Vnitrni dotyk, kruznice #2 lezi uvnitr kruznice #1, prekryv: %lf\n",S2);
        return 1;
    }
    if (touch_in(r2,r1,distance)) {
        printf ("Vnitrni dotyk, kruznice #1 lezi uvnitr kruznice #2, prekryv: %lf\n",S1);
        return 1;
    }
    if (intersection(r1,r2,distance)) {
        printf("Kruznice se protinaji, prekryv: %lf\n", F1 + F2 - F3);
        return 1;
    }
    if (!intersection(r1,r2,distance)) {
        printf ("Kruznice lezi vne sebe, zadny prekryv.\n");
        return 1;
    }

    return 0;
}