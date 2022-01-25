#ifndef SINGLEPLAYER_H_
#define SINGLEPLAYER_H_

#include <stdbool.h>
#include <ncurses.h>

/* ---------------------------------------------------------- defining title parameters */
#define TITLE_H          3
#define TITLE_W         51

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

#define MATRIX_H_PREV    5

/* defining scoring board parameters */
#define SCORE_H          8
#define SCORE_W         21

/* defining saveing window parameters */
#define SAVE_H           5
#define SAVE_W          21

/* defining pieces preview parameters */
#define PREVIEW_H        8
#define PREVIEW_W       21

#define CMDS_H           3
#define CMDS_W          51

#define HELP_H          20
#define HELP_W          64

#define GAMEOVER_H       9
#define GAMEOVER_W      54

/* ---------------------------------------------------------- END defining title parameters */

/* tetramino cells */
#define T_CELL 4

/* number of tetraminos */
#define T_NUM  7

/* number of possible orientation of tetramino */
#define T_ORI  4

/* the number of each tetramino */
#define T_PIECES  20

//* Define the movements *****************************************************/

/* Arrow Keys */
#define LEFT          1
#define RIGHT         2
#define DOWN          3

/* Letter commands */
#define NEXT          4
#define PREVIOUS      5
#define ROTATE        6
#define XRAY          7
#define HELP          8
#define QUIT          9

//* End of the commands definition *******************************************/

#define TOP_LINE      5

#define TETS_CELL     4

#define FIRST_PLAYER     1

#define SECOND_PLAYER    0

#define NO_SCORE         0

#define NO_HAND          0

/* gameover true or false */
#define GAMEOVER_TRUE        1

#define G_FALSE       0

/* if you finish all the pieces  */
#define GAMEOVER_FINISH_PIECES  3

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
    int gameover;
} player;

extern tet_location TETROMINOS[T_NUM][T_ORI][T_CELL];

int singlePlayer();

#endif /* SINGLEPLAYER_H_ */
