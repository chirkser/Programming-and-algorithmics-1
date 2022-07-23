#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#define MAX_BRANCHES             3
#define DECORATION_NONE          0
#define DECORATION_CANDLE        1
#define DECORATION_SPARKLER      2


typedef struct TNode
{
    struct TNode   * m_Parent;
    struct TNode   * m_Branches[MAX_BRANCHES];
    int              m_Decoration;
} TNODE;
#endif /* __PROGTEST__ */


void               destroyTree        ( TNODE           * root )
{
    if (! root) return;
    for ( int i = 0; i < 3; i++){
        if(root->m_Branches[i])
            destroyTree(root->m_Branches[i]);
    }
    free(root);
}


int                easyToCatchFire    ( TNODE           * root )
{
    if ( !root ) {
        return 0;
    }
    int antiDecoration1, antiDecoration2;
    int result = 0;
    if( root -> m_Decoration == DECORATION_SPARKLER){
        antiDecoration1 = DECORATION_CANDLE;
    }
    else if(root -> m_Decoration == DECORATION_CANDLE){
        antiDecoration1 = DECORATION_SPARKLER;
    } else {
        antiDecoration1 = -1;
    }

    /*-----------------------------------------------------------------*\
     *  Check root(1||2) and level lower
     *   //       root -> Decoration ( 1 || 2 )
     *   //       /           |                \
     *   //   (1 || 2)       (1 || 2)       (1 || 2)
     */

    for (int i = 0; i < 3; i++) {
        if (root -> m_Branches[i] != NULL) {
            if (root -> m_Branches[i] -> m_Decoration == antiDecoration1) {
                return 1;
            }
        }
    }

    /*------------------------------------------------------------------*\
     * check one level
     *           /                |                 \
     *        (1||2)             ?                  ?
     */

    for (int j = 0; j < 3; j++) {
        if (root -> m_Branches[j] != NULL) {
            if (root -> m_Branches[j] -> m_Decoration == DECORATION_SPARKLER) {
                antiDecoration2 = DECORATION_CANDLE;
            } else if (root -> m_Branches[j] -> m_Decoration == DECORATION_CANDLE) {
                antiDecoration2 = DECORATION_SPARKLER;
            } else {
                antiDecoration2 = -1;
            }
            if (j == 0 || j == 2) {
                if (root -> m_Branches[1] != NULL) {
                    if (root -> m_Branches[1] -> m_Decoration == antiDecoration2) {
                        return 1;
                    }
                }
            } else if (j == 1) {
                if (root -> m_Branches[0] != NULL) {
                    if (root -> m_Branches[0] -> m_Decoration == antiDecoration2) {
                        return 1;
                    }
                } else if (root -> m_Branches[2] != NULL) {
                    if (root -> m_Branches[2] -> m_Decoration == antiDecoration2) {
                        return 1;
                    }
                }
            }
        }
    }

    /*--------------------------------------------------------------\
     * go down the branches
     */
    for (int m = 0; m < 3; m++) {
        if(root -> m_Branches[m] != NULL)
            result += easyToCatchFire(root -> m_Branches[m]);
    }
    return result;
}

int                cutBranch          ( TNODE          ** root,
                                        const char      * path )
{
    if(!*root)
        return 0;
    TNODE * actuallyLevel = * root;  // create root or branch if it was not created
    size_t len = strlen(path);
    for (size_t i = 0; i < len; i++) {
        if( actuallyLevel -> m_Branches[path[i] - '0'])                 // If this is the desired branch
            actuallyLevel = actuallyLevel -> m_Branches[path[i] - '0']; // then go up
        else
            return 0;
    }
    if(actuallyLevel->m_Parent!=NULL)
        actuallyLevel->m_Parent->m_Branches[path[len-1]-'0']=NULL;
    destroyTree(actuallyLevel);
    if(!strlen(path))  // if path 0
        *root = NULL;
    return 1;
}

int                setDecoration      ( TNODE          ** root,
                                        const char      * path,
                                        int               decor )
{
    size_t len = strlen(path);
    for (size_t i = 0; i < len; i++) {   // Invalid data
        if (path[i] != '0' &&
            path[i] != '1' &&
            path[i] != '2' ) {
            return 0;
        }
    }

    if(! * root ){
        TNODE * start =(TNODE *) malloc (sizeof ( TNODE ) ); // create root if it was not created
        start -> m_Parent = NULL;
        start -> m_Decoration = DECORATION_NONE;
        for (int i = 0; i < 3; i++) {
            start -> m_Branches[i] = NULL;
        }
        * root = start;
    }

    TNODE * actuallyLevel = * root;
    if (! len ) {
        actuallyLevel->m_Decoration = decor;
    }
    for (size_t i = 0; i < len; i++) {
        if (!actuallyLevel -> m_Branches [path[i] - '0']) {               // If no branches have been created =>
            TNODE *newLevel = (TNODE *) malloc(1 * sizeof(TNODE));
            newLevel -> m_Parent = actuallyLevel;
            newLevel -> m_Decoration = DECORATION_NONE;
            for (int j = 0; j < 3; j++) {                                       // create
                newLevel -> m_Branches[j] = NULL;
            }
            actuallyLevel -> m_Branches[path[i] - '0'] = newLevel;
        }
        actuallyLevel = actuallyLevel -> m_Branches[path[i] - '0'];
    }
    actuallyLevel -> m_Decoration = decor;
    return 1;
}


#ifndef __PROGTEST__
int                main               ( void )
{
    TNODE * r;

    r = NULL;
    assert ( setDecoration( &r, "0123", DECORATION_CANDLE) == 0);
    assert ( setDecoration ( &r, "000", DECORATION_SPARKLER ) == 1 );
    assert ( r != NULL
             && r -> m_Parent == NULL
             && r -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] != NULL
             && r -> m_Branches[1] == NULL
             && r -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Parent == r
             && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_SPARKLER
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL );

    assert ( setDecoration ( &r, "001", DECORATION_SPARKLER ) == 1 );
    assert ( r != NULL
             && r -> m_Parent == NULL
             && r -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] != NULL
             && r -> m_Branches[1] == NULL
             && r -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Parent == r
             && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] != NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_SPARKLER
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_SPARKLER
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] == NULL );
    assert ( setDecoration ( &r, "002", DECORATION_SPARKLER ) == 1 );
    assert ( r != NULL
             && r -> m_Parent == NULL
             && r -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] != NULL
             && r -> m_Branches[1] == NULL
             && r -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Parent == r
             && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] != NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] != NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_SPARKLER
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_SPARKLER
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Decoration == DECORATION_SPARKLER
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[2] == NULL );
    assert ( setDecoration ( &r, "1", DECORATION_CANDLE ) == 1 );
    assert ( r != NULL
             && r -> m_Parent == NULL
             && r -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] != NULL
             && r -> m_Branches[1] != NULL
             && r -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Parent == r
             && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] != NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] != NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_SPARKLER
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_SPARKLER
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Decoration == DECORATION_SPARKLER
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[2] == NULL
             && r -> m_Branches[1] -> m_Parent == r
             && r -> m_Branches[1] -> m_Decoration == DECORATION_CANDLE
             && r -> m_Branches[1] -> m_Branches[0] == NULL
             && r -> m_Branches[1] -> m_Branches[1] == NULL
             && r -> m_Branches[1] -> m_Branches[2] == NULL );
    assert ( setDecoration ( &r, "01", DECORATION_NONE ) == 1 );
    assert ( r != NULL
             && r -> m_Parent == NULL
             && r -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] != NULL
             && r -> m_Branches[1] != NULL
             && r -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Parent == r
             && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[1] != NULL
             && r -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] != NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] != NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_SPARKLER
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_SPARKLER
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Decoration == DECORATION_SPARKLER
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] == NULL
             && r -> m_Branches[1] -> m_Parent == r
             && r -> m_Branches[1] -> m_Decoration == DECORATION_CANDLE
             && r -> m_Branches[1] -> m_Branches[0] == NULL
             && r -> m_Branches[1] -> m_Branches[1] == NULL
             && r -> m_Branches[1] -> m_Branches[2] == NULL );
    assert ( setDecoration ( &r, "", DECORATION_CANDLE ) == 1 );
    assert ( r != NULL
             && r -> m_Parent == NULL
             && r -> m_Decoration == DECORATION_CANDLE
             && r -> m_Branches[0] != NULL
             && r -> m_Branches[1] != NULL
             && r -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Parent == r
             && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[1] != NULL
             && r -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] != NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] != NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_SPARKLER
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_SPARKLER
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Decoration == DECORATION_SPARKLER
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] == NULL
             && r -> m_Branches[1] -> m_Parent == r
             && r -> m_Branches[1] -> m_Decoration == DECORATION_CANDLE
             && r -> m_Branches[1] -> m_Branches[0] == NULL
             && r -> m_Branches[1] -> m_Branches[1] == NULL
             && r -> m_Branches[1] -> m_Branches[2] == NULL );
    assert ( easyToCatchFire ( r ) == 0 );
    destroyTree ( r );

    r = NULL;
    assert ( setDecoration ( &r, "000", DECORATION_SPARKLER ) == 1 );
    assert ( r != NULL
             && r -> m_Parent == NULL
             && r -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] != NULL
             && r -> m_Branches[1] == NULL
             && r -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Parent == r
             && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_SPARKLER
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL );
    assert ( setDecoration ( &r, "002", DECORATION_CANDLE ) == 1 );
    assert ( r != NULL
             && r -> m_Parent == NULL
             && r -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] != NULL
             && r -> m_Branches[1] == NULL
             && r -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Parent == r
             && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] != NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_SPARKLER
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Decoration == DECORATION_CANDLE
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[2] == NULL );
    assert ( setDecoration ( &r, "2", DECORATION_CANDLE ) == 1 );
    assert ( r != NULL
             && r -> m_Parent == NULL
             && r -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] != NULL
             && r -> m_Branches[1] == NULL
             && r -> m_Branches[2] != NULL
             && r -> m_Branches[0] -> m_Parent == r
             && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] != NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_SPARKLER
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Decoration == DECORATION_CANDLE
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[2] == NULL
             && r -> m_Branches[2] -> m_Parent == r
             && r -> m_Branches[2] -> m_Decoration == DECORATION_CANDLE
             && r -> m_Branches[2] -> m_Branches[0] == NULL
             && r -> m_Branches[2] -> m_Branches[1] == NULL
             && r -> m_Branches[2] -> m_Branches[2] == NULL );
    assert ( easyToCatchFire ( r ) == 0 );
    destroyTree ( r );

    r = NULL;
    assert ( setDecoration ( &r, "0001", DECORATION_SPARKLER ) == 1 );
    assert ( r != NULL
             && r -> m_Parent == NULL
             && r -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] != NULL
             && r -> m_Branches[1] == NULL
             && r -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Parent == r
             && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] != NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_SPARKLER
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] == NULL );
    assert ( setDecoration ( &r, "000", DECORATION_CANDLE ) == 1 );
    assert ( r != NULL
             && r -> m_Parent == NULL
             && r -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] != NULL
             && r -> m_Branches[1] == NULL
             && r -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Parent == r
             && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_CANDLE
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] != NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_SPARKLER
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] == NULL );
    assert ( easyToCatchFire ( r ) == 1 );
    destroyTree ( r );
    r = NULL;
    assert ( setDecoration ( &r, "012001", DECORATION_SPARKLER ) == 1 );
    assert ( r != NULL
             && r -> m_Parent == NULL
             && r -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] != NULL
             && r -> m_Branches[1] == NULL
             && r -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Parent == r
             && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[1] != NULL
             && r -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] != NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Parent == r -> m_Branches[0] -> m_Branches[1]
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2]
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] != NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_SPARKLER
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] == NULL );
    assert ( setDecoration ( &r, "012002", DECORATION_CANDLE ) == 1 );
    assert ( r != NULL
             && r -> m_Parent == NULL
             && r -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] != NULL
             && r -> m_Branches[1] == NULL
             && r -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Parent == r
             && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[1] != NULL
             && r -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] != NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Parent == r -> m_Branches[0] -> m_Branches[1]
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2]
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] != NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] != NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] != NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_SPARKLER
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Parent == r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Decoration == DECORATION_CANDLE
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[2] == NULL );

    assert ( easyToCatchFire ( r ) == 1 );
    assert ( cutBranch ( &r, "0120" ) == 1 );
    assert ( r != NULL
             && r -> m_Parent == NULL
             && r -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] != NULL
             && r -> m_Branches[1] == NULL
             && r -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Parent == r
             && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[1] != NULL
             && r -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] != NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Parent == r -> m_Branches[0] -> m_Branches[1]
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[2] == NULL );
    assert ( easyToCatchFire ( r ) == 0 );
    destroyTree ( r );

    r = NULL;
    assert ( setDecoration ( &r, "0123", DECORATION_SPARKLER ) == 0 );
    assert ( r == NULL );
    assert ( cutBranch ( &r, "012" ) == 0 );
    assert ( r == NULL );
    assert ( easyToCatchFire ( r ) == 0 );
    destroyTree ( r );

    r = NULL;
    assert ( setDecoration ( &r, "012", DECORATION_SPARKLER ) == 1 );
    assert ( r != NULL
             && r -> m_Parent == NULL
             && r -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] != NULL
             && r -> m_Branches[1] == NULL
             && r -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Parent == r
             && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[1] != NULL
             && r -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] != NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Parent == r -> m_Branches[0] -> m_Branches[1]
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Decoration == DECORATION_SPARKLER
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[2] == NULL );
    assert ( setDecoration ( &r, "011", DECORATION_CANDLE ) == 1 );
    assert ( r != NULL
             && r -> m_Parent == NULL
             && r -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] != NULL
             && r -> m_Branches[1] == NULL
             && r -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Parent == r
             && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[1] != NULL
             && r -> m_Branches[0] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0]
             && r -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_NONE
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] != NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] != NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] -> m_Parent == r -> m_Branches[0] -> m_Branches[1]
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] -> m_Decoration == DECORATION_CANDLE
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] -> m_Branches[2] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Parent == r -> m_Branches[0] -> m_Branches[1]
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Decoration == DECORATION_SPARKLER
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[1] == NULL
             && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[2] == NULL );
    assert ( easyToCatchFire ( r ) == 1 );
    assert ( cutBranch ( &r, "" ) == 1 );
    assert ( r == NULL );
    assert ( easyToCatchFire ( r ) == 0 );
    assert ( cutBranch ( &r, "" ) == 0 );
    assert ( r == NULL );
    destroyTree ( r );


    return 0;
}
#endif /* __PROGTEST__ */