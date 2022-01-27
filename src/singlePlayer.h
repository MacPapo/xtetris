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

#define MQUIT_H         9
#define MQUIT_W        54

#define RTMENU_H        9
#define RTMENU_W        54
/* ---------------------------------------------------------- END defining title parameters */

/* tetramino cells */
#define T_CELL 4

/* number of tetraminos */
#define T_NUM  7

/* number of possible orientation of tetramino */
#define T_ORI  4

/* the number of each tetramino */
#define T_PIECES  20

//* End of the commands definition *******************************************/

#define TOP_LINE         5

#define TETS_CELL        4

#define FIRST_PLAYER     1

#define SECOND_PLAYER    0

#define NO_SCORE         0

#define NO_HAND          0

/* gameover true or false */
#define GAMEOVER_TRUE        1

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
    int gameField[MATRIX_H][MATRIX_W];
    WINDOW* window;
} player;

extern tet_location TETROMINOS[T_NUM][T_ORI][T_CELL];

/*-- init field function ---*/
void initSinglePlayerTitle(WINDOW* title);
void initSinglePlayerField(WINDOW* field);
void initSinglePlayerPreview(WINDOW* preview);
void initSinglePlayerScore(WINDOW* score);
void initSinglePlayerScore(WINDOW* save);
void initSinglePlayerCmds(WINDOW* cmds);
void initField(WINDOW* title, WINDOW* field, WINDOW* preview, WINDOW* score, WINDOW* save, WINDOW* cmds);
void initGameOver(int score);
void initTopLine(WINDOW *field);
void initQuit(int score);
int initReturnToMenu(int score);


void changePiece(WINDOW* score);
/*--- init field variable ---*/
void initGameMatrix(int gameField[][MATRIX_W]);
void initTetVector();

/*--- print info ---*/
void printMatrix(int gamefield[][MATRIX_W]);

/*--- repaint */
void colorField(player *pg);
void resetPreview();

/*--- refresh function ---*/
void refreshPreview(WINDOW* s_preview, tet* preview_piece);
void refreshGameField(int* x, tet* current_piece, player *pg);
void refreshScore(WINDOW* s_score ,int pieces, int score);

void playerChoices(player *player, tet* current_piece, int* pieces, int gamefield[][MATRIX_W]);
int singlePlayerCommands(WINDOW* field);

/*--- main function for gamefield ---*/
int calculateScoring(int rows);
int checkDeleteRows(player *pg);
int checkGameOver(int gamefield[][MATRIX_W]);
int smallerIntervall(int row, int col, int gamefield[][MATRIX_W]);

void goDownTetramini(int row, int gamefield[][MATRIX_W]);
void fallingPiece(player *pg);

/*--- piece movement ---*/
void rotatingPiece(tet* current_piece);
void nextPiece(tet* current_piece, tet* preview_piece);
void backPiece(tet* current_piece, tet* preview_piece);

/*--- single Player function --*/
int singlePlayer();

#endif /* SINGLEPLAYER_H_ */
