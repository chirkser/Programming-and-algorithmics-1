#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// array for function "convertT9"
const char * T9[10] = {
        "<>/`~.,?':;]}[{=+-)(*&^%$#@!0",
        " 1",
        "abc2ABC",
        "def3DEF",
        "ghi4GHI",
        "jkl5JKL",
        "mno6MNO",
        "pqrs7PQRS",
        "tuv8TUV",
        "wxyz9WXYZ"
};

typedef struct book {
    char * phoneNumber;
    char * name;
    char * T9name;
} Tbook;

/*---------------------------------------------------------------------------*/
/** The comparator function for another functions
 *
 * @param a  pointer to the left element to compare
 * @param b  pointer to the right element to compare
 * @return 0 if elements are equal, 1 otherwise
 */
int compare (const void * a, const void * b ) {
    Tbook * data1 = (Tbook *)a;
    Tbook * data2 = (Tbook *)b;
    int result = strcmp(data1->phoneNumber, data2->phoneNumber);
    if(result)  return 1;
    return 0;
}
/*---------------------------------------------------------------------------*/
/** Phone number search function in database
 *
 * @param arr  array of structures
 * @param data  struct
 * @param globalCount  telephone number counter
 * @return 1 if the number was found, 0 otherwise
 */
int searchForDelete (Tbook ** arr,Tbook * data, int globalCount) {
    int flag = 0;
    for (int i = 0; i < globalCount; i++) {
        if (arr[i] != NULL) {
            int result = compare(data, arr[i]);
            if (result == 0) {
                printf("DELETED\n");
                flag = 1;
                free(arr[i]->name);
                free(arr[i]->phoneNumber);
                free(arr[i]->T9name);
                free(arr[i]);
                arr[i] = NULL;
                return 1;
            }
        }
    }
    if (!flag) {
        printf ("NOT FOUND\n");
    }
    return 0;
}
/*---------------------------------------------------------------------------*/
/** Function for checking if a phone number exists in the database
 *
 * @param arr  array of structures
 * @param data  structure
 * @param globalCount  telephone number counter
 * @return 1 if phone number exists, 0 not exists
 */
int checkDataBase (Tbook ** arr,Tbook * data, int globalCount) {
    int flag = 0;
    for (int i = 0; i < globalCount; i++) {
        if (arr[i] != NULL) {
            int result = compare(data, arr[i]);
            if (result == 0) {
                arr[i]->name = data->name;
                arr[i]->T9name = data->T9name;
                flag = 1;
                printf("UPDATED\n");
                return 1;
            }
        }
    }
    if (!flag) {
        printf ("NEW\n");
    }
    return 0;
}
/*---------------------------------------------------------------------------*/
/** The comparator function for another functions
 *
 * @param a  pointer to the left element to compare
 * @param b  pointer to the right element to compare
 * @return 0 if elements are equal, 1 otherwise
 */
int compareForSearch(const void * a, const void * b ) {
    Tbook * data1 = (Tbook *)a;
    Tbook * data2 = (Tbook *)b;
    int result1 = strcmp(data1->phoneNumber, data2->phoneNumber);
    int result2 = strcmp(data1->phoneNumber,data2->T9name);
    if(result1 == 0 || result2 == 0)  return 1;
    return 0;
}
/*---------------------------------------------------------------------------*/
/** Phone number search function in database
 *
 * The function will print: "NOT FOUND"
 * - if  if phone number not exists
 * The function will print: "FOUND" and structure data
 * - if one match was found
 * The function will print: "AMBIGUOUS" and count number of matches
 * - if more than one match was found
 * @param arr  array of structures
 * @param data  structure
 * @param globalCount  telephone number counter
 * @return 0 always
 */
int search (Tbook ** arr, Tbook * data, int globalCount) {
    int cntDuplicate = 0;
    int forFound = 0;
    for (int j = 0 ; j < globalCount; j++) {
        if (arr[j]!= NULL) {
            int result = compareForSearch(data, arr[j]);
            if (result == 1) {
                cntDuplicate++;
                forFound = j;
            }
        }
    }
    if (cntDuplicate == 0) {
        printf ("NOT FOUND\n");
    } else if (cntDuplicate == 1) {
        printf ("FOUND %s (%s)\n",arr[forFound]->phoneNumber,arr[forFound]->name);
    } else {
        printf ("AMBIGUOUS (%d matches)\n",cntDuplicate);
    }
    return 0;
}
/*---------------------------------------------------------------------------*/
/** Function converting from string to T9
 *  @param[in] name  string with name
 *  @param[in] T9name  empty string for conversion result
 */
void convertT9 (char * name, char * T9name) {
    int i = 0;
    while (1) {
        if (name[i] == '\0') {
            return;
        }
        for (int j = 0; j < 10; j++) {
            int k = 0;
            while (1) {
                if (T9[j][k] == '\0') {
                    break;
                }
                if (name[i] == T9[j][k]) {
                    T9name[i] = j + '0';
                }
                k++;
            }
        }
        i++;
    }
}
/*---------------------------------------------------------------------------*/
/** Function for reading data
 *
 * The data is read,
 * split into two different strings and put into the structure.
 * Reading is not successful:
 * - if "first symbol" is not valid char
 * - if "first string" is not valid char
 * - if "Second string" is not valid char
 *
 * @param[in] data  structure
 * @return -1 if reading was not successful
 * @return 0 if need to add data
 * @return 1 if need to search data
 * @return 2 if need to delete data
 */
int readLine (Tbook * data) {
    char * input = NULL, * number = NULL, * name = NULL, * T9name = NULL;
    int cnt = 0, result;
    size_t len = 1;
    char symbol,mark1,mark2,end,space1,space2;
    cnt = getline(&input, &len, stdin);
    number = (char *)malloc(cnt * sizeof(char));
    name = (char *)malloc(cnt * sizeof(char));
    T9name = (char *)malloc(cnt * sizeof(char));
    result = sscanf(input, "%c%c %[0-9]%c %c%[^\"]%c%c",&symbol,&space1,number,&space2,&mark1,name,&mark2,&end);
    if (symbol == '+') {
        if (!( mark1 == '"' && mark2 == '"' && end == '\n' && space1 == ' ' && space2 == ' ' )) {
            free(name);
            free(number);
            free(T9name);
            return -1;
        }
        if (result == 8) {
            convertT9(name,T9name);
            data->phoneNumber = number;
            data->name = name;
            data->T9name = T9name;
            return 0;
        }
    } else if (symbol == '?') {
        if (!( space1 == ' ' && space2 == '\n' )) {
            free(name);
            free(number);
            free(T9name);
            return -1;
        }
        if (result == 4) {
            data->phoneNumber = number;
            return 1;
        }
    } else if (symbol == '-') {
        if (!(space1 == ' ' && space2 == '\n')) {
            free(name);
            free(number);
            free(T9name);
            return -1;
        }
        if (result == 4) {
            data->phoneNumber = number;
            return 2;
        }
    }
    free(name);
    free(number);
    free(T9name);
    return -1;
}
/*---------------------------------------------------------------------------*/
int main(){
    Tbook ** arr = NULL;
    arr = (Tbook **)malloc(sizeof(Tbook *));
    int globalCnt = 0;
    printf ("PBX configuration (+ = set, - = delete, ? = test, EOF = quit):\n");
    while (1) {
        Tbook * data = (Tbook *)malloc(1 * sizeof(*data));
        switch (readLine(data)) {
            case 0:
                if (checkDataBase(arr,data,globalCnt)==0) {
                    globalCnt++;
                    arr = (Tbook **)realloc(arr,globalCnt * sizeof(Tbook*));
                    arr[globalCnt - 1] = data;
                }
                break;
            case 1:
                search(arr,data,globalCnt);
                free(data->phoneNumber);
                free(data);
                break;
            case 2:
                searchForDelete(arr,data,globalCnt);
                free(data->phoneNumber);
                free(data);
                break;
            default:
                if (feof(stdin)) {
                    for (int i = 0; i < globalCnt; i++) {
                        if(arr[i]!=NULL) {
                            free(arr[i]->name);
                            free(arr[i]->phoneNumber);
                            free(arr[i]->T9name);
                            free(arr[i]);
                        }
                    }
                    free(arr);
                    return 0;
                } else {
                    free(data);
                    printf("INVALID COMMAND\n");
                    break;
                }
        }
    }
}