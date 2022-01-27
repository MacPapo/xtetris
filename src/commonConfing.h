#ifndef COMMONCONFIG_H_
#define COMMONCONFIG_H_

#include <ncurses.h>

/* box vertical & horizontal lines  */
#define V_LINES     ACS_VLINE
#define H_LINES     ACS_HLINE

/* defining field parameters */
#define FIELD_Y         10
#define FIELD_X         10

#define FIELD_H         27
#define FIELD_W         22

/* defining matrix parameters */
#define MATRIX_H        25
#define MATRIX_W        10

#define MATRIX_H_PREV   5

#define HELP_H          20
#define HELP_W          64

#define GAMEOVER_H       9
#define GAMEOVER_W      54

/* defining scoring board parameters */
#define SCORE_H          8
#define SCORE_W         21

/* defining pieces preview parameters */
#define PREVIEW_H        8
#define PREVIEW_W       21

/* defining saveing window parameters */
#define SAVE_H           5
#define SAVE_W          21

#define MQUIT_H         9
#define MQUIT_W        54

#define RTMENU_H        9
#define RTMENU_W        54

/* tetramino cells */
#define T_CELL 4

/* number of tetraminos */
#define T_NUM  7

/* number of possible orientation of tetramino */
#define T_ORI  4

/* the number of each tetramino */
#define T_PIECES  20

#define TOP_LINE      5

#define TETS_CELL     4

typedef struct {
    int tet;
    int ori;
} tet;

typedef struct {
    int row;
    int col;
} tet_location;

typedef struct {
    int score;
    int gameField[MATRIX_H][MATRIX_W];
    WINDOW* window;
} player;

extern tet_location TETROMINOS[T_NUM][T_ORI][T_CELL];

#endif 