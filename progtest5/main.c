#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


int mins(char * mon,int day,int h,int min) {
    if (strcmp(mon,"Jan")==0) {
        return day * 1440 + h * 60 + min;
    } else if (strcmp(mon,"Feb")==0) {
        return (31 + day) * 1440 + h * 60 + min;
    } else if (strcmp(mon,"Mar")==0) {
        return (59 + day) * 1440 + h * 60 + min;
    } else if (strcmp(mon,"Apr")==0) {
        return (90 + day) * 1440 + h * 60 + min;
    } else if (strcmp(mon,"May")==0) {
        return (120 + day) * 1440 + h * 60 + min;
    } else if (strcmp(mon,"Jun")==0) {
        return (151 + day) * 1440 + h * 60 + min;
    } else if (strcmp(mon,"Jul")==0) {
        return (181 + day) * 1440 + h * 60 + min;
    } else if (strcmp(mon,"Aug")==0) {
        return (212 + day) * 1440 + h * 60 + min;
    } else if (strcmp(mon,"Sep")==0) {
        return (243 + day) * 1440 + h * 60 + min;
    } else if (strcmp(mon,"Oct")==0) {
        return (273 + day) * 1440 + h * 60 + min;
    } else if (strcmp(mon,"Nov")==0) {
        return (304 + day) * 1440 + h * 60 + min;
    }else if (strcmp(mon,"Dec")==0) {
        return (334 + day) * 1440 + h * 60 + min;
    }
    return 0;
}


typedef struct system{
    int M_id;
    char M_RZ[1001];
    char M_mon[4];
    int M_day;
    int M_h;
    int M_min;
    int M_time;
}Tsystem;

Tsystem * data = NULL;
int cnt;

Tsystem * readData (int * cnt) {
    int id;
    char RZ[1001];
    char mon[4];
    int day;
    int h;
    int min;
    int maxsize = 0;
    Tsystem * data = NULL;
    * cnt = 0;
    int input;
    char s1,s2;
    if (scanf (" %c",&s1)!=1 || !(s1 == '{')) {
        return NULL;
    }
    while ( (input = scanf (" %d : %1000s %3s %d %d : %d %c",&id,RZ,mon,&day,&h,&min,&s2)) == 7 ) {
        if (day < 1  || min < 0 || min > 59
            || h < 0 || h > 23
            || !(s2==',' || s2 == '}')
            || !(     (strcmp(mon,"Jan")==0 && day <= 31)
                      ||   (strcmp(mon,"Feb")==0 && day <= 28)
                      ||   (strcmp(mon,"Mar")==0 && day <= 31)
                      ||   (strcmp(mon,"Apr")==0 && day <= 30)
                      ||   (strcmp(mon,"May")==0 && day <= 31)
                      ||   (strcmp(mon,"Jun")==0 && day <= 30)
                      ||   (strcmp(mon,"Jul")==0 && day <= 31)
                      ||   (strcmp(mon,"Aug")==0 && day <= 31)
                      ||   (strcmp(mon,"Sep")==0 && day <= 30)
                      ||   (strcmp(mon,"Oct")==0 && day <= 31)
                      ||   (strcmp(mon,"Nov")==0 && day <= 30)
                      ||   (strcmp(mon,"Dec")==0 && day <= 31))) {
            return NULL;
        }
        if (*cnt >= maxsize) {
            maxsize += (maxsize < 10) ? 10 : maxsize / 2;
            data = (Tsystem *) realloc(data, maxsize * sizeof(*data));
        }
        data[*cnt].M_id = id;
        strncpy(data[*cnt].M_RZ, RZ, sizeof(data[*cnt].M_RZ));
        strncpy(data[*cnt].M_mon, mon, sizeof(data[*cnt].M_mon));
        data[*cnt].M_day = day;
        data[*cnt].M_h = h;
        data[*cnt].M_min = min;
        data[*cnt].M_time = mins(mon,day,h,min);
        (*cnt)++;
        if (s2 == '}') {
            break;
        }
    }
    return data;
}
int compareForSearch(const void * a, const void * b ) {
    Tsystem * data1 = (Tsystem *)a;
    Tsystem * data2 = (Tsystem *)b;
    int result = strcmp(data1->M_RZ, data2->M_RZ);
    if (result) return result;
    result = data1->M_time - data2->M_time;
    if (result) return result;
    return 0;
}

int countDuplicate ( int index ) {
    int result = 0;
    for (int i = 0; i < cnt; i++) {
        if (compareForSearch(&data[index],&data[i])== 0 ) {
            result++;
        }
    }
    return result;
}

int search (Tsystem * data, int cnt) {
    char RZ[1001];
    char mon[4];
    int day,h,min;
    while (1) {
        if (scanf (" %1000s %3s %d %d:%d",RZ,mon,&day,&h,&min)!=5 || day < 1 || min < 0 || min > 59
            || h < 0 || h > 23
            || !(     (strcmp(mon,"Jan")==0 && day <= 31)
                 ||   (strcmp(mon,"Feb")==0 && day <= 28)
                 ||   (strcmp(mon,"Mar")==0 && day <= 31)
                 ||   (strcmp(mon,"Apr")==0 && day <= 30)
                 ||   (strcmp(mon,"May")==0 && day <= 31)
                 ||   (strcmp(mon,"Jun")==0 && day <= 30)
                 ||   (strcmp(mon,"Jul")==0 && day <= 31)
                 ||   (strcmp(mon,"Aug")==0 && day <= 31)
                 ||   (strcmp(mon,"Sep")==0 && day <= 30)
                 ||   (strcmp(mon,"Oct")==0 && day <= 31)
                 ||   (strcmp(mon,"Nov")==0 && day <= 30)
                 ||   (strcmp(mon,"Dec")==0 && day <= 31))) {
            if (feof(stdin)) {
                return 0;
            }
            printf ("Nespravny vstup.\n");
            return 1;
        }
        //Tsystem * temp = (Tsystem*) malloc (1 * sizeof(Tsystem));
        Tsystem temp ;
        strncpy(temp.M_RZ, RZ, sizeof(temp.M_RZ));
        strncpy(temp.M_mon, mon, sizeof(temp.M_mon));
        temp.M_day = day;
        temp.M_h = h;
        temp.M_min = min;
        temp.M_time = mins(mon,day,h,min);
        int d = 0;
        bool flagPresne = false;
        bool flagPredchaz = false;
        bool flagPozdejici = false;
        int indexPresne = -1;
        int indexPred = -1;
        int indexPozd = -1;
        for (int i = 0; i < cnt; i++) {
            if (strcmp(temp.M_RZ, data[i].M_RZ) == 0) {
                int t = compareForSearch(&data[i],&temp);
                if (t == 0) {
                    if (!flagPresne) {
                        indexPresne = i;
                        flagPresne = true;
                    }
                    d++;
                } else if (t < 0) {
                    if (i > indexPred) {
                        indexPred = i;
                    }
                    flagPredchaz = true;
                } else if (t > 0) {
                    indexPozd = i;
                    if (i>0) {
                        if (strcmp(data[i].M_RZ,data[i-1].M_RZ)==0) {
                            indexPred = i - 1;
                            flagPredchaz = true;
                        }
                    }
                    flagPozdejici = true;
                    break;
                }
            }
        }
        if (flagPresne) {
            printf ("> Presne: %s %d %02d:%02d, %dx ",temp.M_mon,temp.M_day,temp.M_h,temp.M_min,d);
            printf ("[%d",data[indexPresne].M_id);
            for (int i = indexPresne + 1; i < indexPresne + d; i++) {
                printf (", %d",data[i].M_id);
            }
            printf ("]\n");
        } else {
            //printf ("%d %d %d\n",flagPresne,flagPredchaz,flagPozdejici);
            if (!flagPredchaz && flagPozdejici) {
                int cnt1 = countDuplicate(indexPozd);
                printf ("> Predchazejici: N/A\n");
                printf ("> Pozdejsi: %s %d %02d:%02d, %dx ",data[indexPozd].M_mon,
                        data[indexPozd].M_day,
                        data[indexPozd].M_h,
                        data[indexPozd].M_min,
                        cnt1);
                printf ("[%d",data[indexPozd].M_id);
                for (int i = indexPozd + 1; i < indexPozd + cnt1; i++) {
                    printf (", %d",data[i].M_id);
                }
                printf ("]\n");
            } else if (flagPredchaz && !flagPozdejici) {
                int cnt2 = countDuplicate(indexPred);
                //printf ("cnt2 = %d\n",cnt2);
                printf ("> Predchazejici: %s %d %02d:%02d, %dx ",data[indexPred].M_mon,
                        data[indexPred].M_day,
                        data[indexPred].M_h,
                        data[indexPred].M_min,
                        cnt2);
                printf ("[%d",data[indexPred- cnt2 + 1].M_id);
                for (int i = indexPred - cnt2 + 2; i <= indexPred; i++) {
                    printf (", %d",data[i].M_id);
                }
                printf ("]\n");
                printf ("> Pozdejsi: N/A\n");
            } else if (flagPredchaz && flagPozdejici) {
                int cnt1 = countDuplicate(indexPred);
                int cnt2 = countDuplicate(indexPozd);
                printf("> Predchazejici: %s %d %02d:%02d, %dx ", data[indexPred].M_mon,
                       data[indexPred].M_day,
                       data[indexPred].M_h,
                       data[indexPred].M_min,
                       cnt1);
                printf ("[%d",data[indexPred- cnt1 + 1].M_id);
                for (int i = indexPred - cnt1 + 2; i <= indexPred; i++) {
                    printf (", %d",data[i].M_id);
                }
                printf ("]\n");
                printf("> Pozdejsi: %s %d %02d:%02d, %dx ", data[indexPozd].M_mon,
                       data[indexPozd].M_day,
                       data[indexPozd].M_h,
                       data[indexPozd].M_min,
                       cnt2);
                printf("[%d", data[indexPozd].M_id);
                for (int i = indexPozd + 1; i < indexPozd + cnt2; i++) {
                    printf(", %d", data[i].M_id);
                }
                printf("]\n");
            } else if (!flagPresne && !flagPredchaz && !flagPozdejici) {
                printf ("> Automobil nenalezen.\n");
            }
        }

    }
    return 0;
}


int compare(const void *a, const void *b)
{
    Tsystem * data1 = (Tsystem *)a;
    Tsystem * data2 = (Tsystem *)b;
    int result = strcmp(data1->M_RZ, data2->M_RZ);
    if (result) return result;
    result = data1->M_time - data2->M_time;
    if (result) return result;
    return data1->M_id - data2->M_id;
}



int main() {
    printf("Data z kamer:\n");
    data = readData(&cnt);
    if ( ! data )
    {
        printf ( "Nespravny vstup.\n" );
        return 1;
    }
    qsort(data,cnt, sizeof(Tsystem ), compare);
    /*printf ("    SORTING...   \n");

    for (int i = 0; i < cnt; i++) {
        printf("%d: %s %s %d %d:%d %d\n", data[i].M_id,data[i].M_RZ,data[i].M_mon,data[i].M_day,data[i].M_h,data[i].M_min,data[i].M_time);
    } */
    printf ("Hledani:\n");
    search(data,cnt);
    free(data);
    return 0;
}