#include "singlePlayer.h"
#include <ncurses.h>
#include <unistd.h>

#define HCENTER  ((LINES - FIELD_H) / 2)
#define WCENTER  ((COLS  - FIELD_W) / 2)

//* Begin the Prototyping ****************************************************/

void initGameMatrix(int gameField[][MATRIX_W]);
void initTetVector();
//* Prototype the init field funcs  ******************************************/

void initSinglePlayerTitle(WINDOW* title);
void initSinglePlayerField(WINDOW* field);
void initSinglePlayerPreview(WINDOW* preview);
void initSinglePlayerScore(WINDOW* score);
void initSinglePlayerScore(WINDOW* save);
void initSinglePlayerCmds(WINDOW* cmds);
void initField(WINDOW* title, WINDOW* field, WINDOW* preview, WINDOW* score, WINDOW* save, WINDOW* cmds);
void initGameOver(int score);

//* End the prototyping of field funcs ***************************************/

void colorField(int gameField[][MATRIX_W], WINDOW* win);
void resetPreview();

void refreshPreview(WINDOW* s_preview, tet* preview_piece);
void refreshGameField(int* x, tet* current_piece, int gamefield[][MATRIX_W], WINDOW *win);
void refreshScore(WINDOW* s_score ,int pieces, int* score);
void printMatrix(int gamefield[][MATRIX_W]);

void playerChoices(player *player, tet* current_piece, int* pieces, int gamefield[][MATRIX_W]);
int singlePlayerCommands(WINDOW* field);

//* Prototype the basic movements ********************************************/
void rotatingPiece(tet* current_piece);

//* End of the movements funcs ***********************************************/
void nextPiece(tet* current_piece, tet* preview_piece);

void backPiece(tet* current_piece, tet* preview_piece);

//* End the Prototyping ******************************************************/

// * Go down tetramini *************//

void fallingPiece(int gamefield[][MATRIX_W], WINDOW* win);

int smallerIntervall(int row, int col, int gamefield[][MATRIX_W]);

int checkDeleteRows(int gamefield[][MATRIX_W], WINDOW* field);

int checkGameOver(int gamefield[][MATRIX_W]);

void goDownTetramini(int row, int gamefield[][MATRIX_W]);

int calculateScoring(int rows);

/* current piece selected by player */
tet current_piece = {0,0};
tet preview_piece = {0,0};

/* store in a variable x and y of current block */
tet_location cell = {0,0};

/* tridimensional array for tetramini */
tet_location TETROMINOS[T_NUM][T_ORI][T_CELL] =
{
  // I
  { { {0, 0}, {0, 1}, {0, 2}, {0, 3} },
    { {0, 0}, {1, 0}, {2, 0}, {3, 0} },
    { {0, 0}, {0, 1}, {0, 2}, {0, 3} },
    { {0, 0}, {1, 0}, {2, 0}, {3, 0} } },
  // J
  { { {0, 0}, {0, 1}, {0, 2}, {1, 2} },
    { {0, 0}, {0, 1}, {1, 0}, {2, 0} },
    { {0, 0}, {1, 0}, {1, 1}, {1, 2} } ,
    { {0, 1}, {1, 1}, {2, 0}, {2, 1} } },
  // L
  { { {1, 0}, {0, 0}, {0, 1}, {0, 2} },
    { {2, 1}, {0, 0}, {1, 0}, {2, 0} },
    { {0, 2}, {1, 2}, {1, 1}, {1, 0} },
    { {0, 0}, {0, 1}, {1, 1}, {2, 1} } },
  // O
  { { {0, 0}, {0, 1}, {1, 0}, {1, 1} },
    { {0, 0}, {0, 1}, {1, 0}, {1, 1} },
    { {0, 0}, {0, 1}, {1, 0}, {1, 1} },
    { {0, 0}, {0, 1}, {1, 0}, {1, 1} } },
  // S
  { { {0, 1}, {0, 2}, {1, 0}, {1, 1} },
    { {0, 0}, {1, 0}, {1, 1}, {2, 1} },
    { {0, 1}, {0, 2}, {1, 0}, {1, 1} },
    { {0, 0}, {1, 0}, {1, 1}, {2, 1} } },
  // T
  { { {0, 0}, {0, 1}, {1, 1}, {0, 2} },
    { {1, 0}, {0, 1}, {1, 1}, {2, 1} },
    { {1, 0}, {0, 1}, {1, 1}, {1, 2} },
    { {0, 0}, {1, 0}, {1, 1}, {2, 0} } },
  // Z
  { { {0, 0}, {0, 1}, {1, 1}, {1, 2} },
    { {1, 0}, {2, 0}, {0, 1}, {1, 1} },
    { {0, 0}, {0, 1}, {1, 1}, {1, 2} },
    { {1, 0}, {2, 0}, {0, 1}, {1, 1} } },
};

/* x position of selected block */
int x = 0;
int *position_x = &x;

int tet_pieces[T_NUM];

int preview_gamefield[MATRIX_H_PREV][MATRIX_W];

player addPlayer() {
    player player;
    player.score = 0;
    player.gameover = 0;
    return player;
}

int singlePlayer()
{
    /* declaring the game windows */
    WINDOW *w_title = NULL, *w_field = NULL, *w_preview = NULL, *w_score = NULL, *w_save = NULL, *w_cmds = NULL;
    WINDOW *s_field, *s_preview, *s_score;

    /* initialize the variables and the matrix */
    int fieldY,   fieldX;
    int previewY, previewX;
    int scoreY,   scoreX;
    int pieces = T_NUM * T_PIECES;
    int gameField[MATRIX_H][MATRIX_W];
    int p_score;
    int key = 0; //*KEY é di prova********************************************/
    player pg = addPlayer();
    p_score = pg.score;

    /* centering the refreshable windows */
    {
        fieldY   = HCENTER + 1;
        fieldX   = WCENTER - SCORE_W/2 + 1;
        previewY = HCENTER + 1;
        previewX = WCENTER - SCORE_W/2 + FIELD_W + 3;
        scoreY   = HCENTER + PREVIEW_H + 2;
        scoreX   = WCENTER - SCORE_W/2 + FIELD_W + 3;
    }

    /* Clear the screen to visualize a new window */
    clear();
    cbreak();

    /* initialize the game matrix & init the base screen & creating the refreshable windows */
    initGameMatrix(gameField);
    initTetVector();
    initField(w_title, w_field, w_preview, w_score, w_save, w_cmds);

    /* automate these commands */
    {
        /* creating the refreshable windows */
        s_field = newwin(FIELD_H - 2, FIELD_W - 2, fieldY, fieldX);
        s_preview = newwin(PREVIEW_H - 2, PREVIEW_W - 2, previewY, previewX);
        s_score = newwin(SCORE_H - 2, SCORE_W - 2, scoreY, scoreX);

        /* assign the proper color scheme */
        wbkgd(s_field, COLOR_PAIR(0));
        wbkgd(s_preview, COLOR_PAIR(0));
        wbkgd(s_score, COLOR_PAIR(0));

        /* refresh those windows */
        wrefresh(s_preview);
        wrefresh(s_field);
        wrefresh(s_score);
    }

    /* Single player routine */
    
    preview_piece.tet = current_piece.tet + 1;
    {
        refreshPreview(s_preview ,&preview_piece);
        refreshGameField(position_x, &current_piece, gameField, s_field);
        refreshScore(s_score ,tet_pieces[current_piece.tet], &p_score);
    }

    do
    {
        keypad(s_field, TRUE);
        key = singlePlayerCommands(s_field);
        colorField(gameField, s_field);

        /* if Arrow pressed!  */
        if(key == LEFT)
            *position_x -= 2;
            
        if(key == RIGHT)
            *position_x += 2;

        if(key == DOWN)
        {
            fallingPiece(gameField, s_field);
            tet_pieces[current_piece.tet] -= 1;
            pieces -= 1; 
            p_score += checkDeleteRows(gameField, s_field);
            refreshScore(s_score, tet_pieces[current_piece.tet], &p_score);
        }

        /* if Letter pressed! */
        if(key == NEXT)
        {
            nextPiece(&current_piece, &preview_piece);
            refreshScore(s_score, tet_pieces[current_piece.tet], &p_score);
        }
            
        if(key == PREVIOUS)
        {
            backPiece(&current_piece, &preview_piece);
            refreshScore(s_score, tet_pieces[current_piece.tet], &p_score);
        }
        
        if(key == ROTATE)
        {
            rotatingPiece(&current_piece);
        }

        if(key == XRAY)
        {
            endwin();
            printMatrix(gameField);
            printf("\n\n %d", current_piece.tet + 1);
        }

        if(key == HELP)
            mvwprintw(s_field, 11, 2, "HELP!!");

        if (checkGameOver(gameField))
        {
            return 0;
        }

        refreshPreview(s_preview, &preview_piece);
        refreshGameField(position_x, &current_piece, gameField, s_field);
    }
    while(pieces && key != QUIT);

    return 5;
}

void initGameMatrix(int gameField[][MATRIX_W])
{
    int row, cols;
    for(row = 0; row < MATRIX_H; ++row)
        for(cols = 0; cols < MATRIX_W; ++cols)
        {
            gameField[row][cols] = 0;
            if(row < MATRIX_H_PREV)
                preview_gamefield[row][cols] = 0;
        }
}

void initTetVector()
{  
    int i;
    for (i = 0; i < T_NUM; i++)  
        tet_pieces[i] = T_PIECES;
}

//* Init the base field funcs ************************************************/

void initSinglePlayerTitle(WINDOW* title)
{
    int titleY, titleX;

    titleY = HCENTER - TITLE_H - 1;
    titleX = WCENTER - 3 - SCORE_W/2;

    title = newwin(TITLE_H, TITLE_W, titleY, titleX);
    box(title, V_LINES, H_LINES);
    wbkgd(title, COLOR_PAIR(2));
    mvwprintw(title, 1, 20, "SINGLE PLAYER");
    wrefresh(title);
}

void initSinglePlayerField(WINDOW* field)
{
    int fieldY, fieldX;

    fieldY = HCENTER;
    fieldX = WCENTER - SCORE_W/2;

    field = newwin(FIELD_H, FIELD_W, fieldY, fieldX);
    box(field, V_LINES, H_LINES);
    wbkgd(field, COLOR_PAIR(2));
    wrefresh(field);
}

void initSinglePlayerPreview(WINDOW* preview)
{
    int previewY, previewX;

    previewY = HCENTER;
    previewX = WCENTER - SCORE_W/2 + FIELD_W + 2;

    preview = newwin(PREVIEW_H, PREVIEW_W, previewY, previewX);
    box( preview, V_LINES, H_LINES);
    wbkgd(preview, COLOR_PAIR(2));
    mvwprintw(preview , 0 , 5 , "| PREVIEW |");
    wrefresh(preview);
}

void initSinglePlayerScore(WINDOW* score)
{
    int scoreY, scoreX;

    scoreY = HCENTER + PREVIEW_H + 1;
    scoreX = WCENTER - SCORE_W/2 + FIELD_W + 2;

    score = newwin(SCORE_H, SCORE_W, scoreY, scoreX);
    box(score , V_LINES, H_LINES);
    wbkgd(score, COLOR_PAIR(2));
    mvwprintw(score , 0 , 6 , "| SCORE |");
    wrefresh(score);
}

void initSinglePlayerSave(WINDOW *save)
{
    int saveY, saveX;

    saveY = HCENTER + PREVIEW_H + SCORE_H + 3;
    saveX = WCENTER - SCORE_W/2 + FIELD_W + 2;

    save = newwin(SAVE_H, SAVE_W, saveY, saveX);
    box(save, V_LINES, H_LINES);
    wbkgd(save, COLOR_PAIR(3));
    mvwprintw(save, 0, 6, "| SAVE |");
    mvwprintw(save, 1, 1, "'H' to help page");
    mvwprintw(save, 2, 1, "'S' to save game");
    mvwprintw(save, 3, 1, "'Q' return to menu");
    wrefresh(save);
}

void initSinglePlayerCmds(WINDOW* cmds)
{
   int cmdsY, cmdsX;

   cmdsY = HCENTER + FIELD_H + 1;
   cmdsX = WCENTER - SCORE_W/2 - 3;

   cmds = newwin(CMDS_H, CMDS_W, cmdsY, cmdsX);
   box(cmds, V_LINES, H_LINES);
   wbkgd(cmds, COLOR_PAIR(2));
   mvwprintw(cmds, 0, 19, "| COMMANDS |");
   mvwprintw(cmds, 1, 9, "[ 'R' ] [ 'N' ] [ < ] [ v ] [ > ]");
   wrefresh(cmds);
}

void initField(WINDOW* title, WINDOW* field, WINDOW* preview, WINDOW* score, WINDOW* save, WINDOW* cmds)
{

    refresh();
    initSinglePlayerTitle(title);
    initSinglePlayerField(field);
    initSinglePlayerPreview(preview);
    initSinglePlayerScore(score);
    initSinglePlayerSave(save);
    initSinglePlayerCmds(cmds);
    refresh();
}

//* End the base field funcs *************************************************/

void colorField(int gameField[][MATRIX_W], WINDOW* field)
{
    int i, j;
    werase(field);
    for(i = 0; i < MATRIX_H; i++)
    {
        for(j = 0; j < MATRIX_W; j++)
        {
            if(gameField[i][j] != 0)
                wattron(field, COLOR_PAIR(gameField[i][j] + 9));
                mvwprintw(field, i, j*2, "  ");
                wattroff(field, COLOR_PAIR(gameField[i][j] + 9));
        }
    }
    wrefresh(field);
}

void resetPreview()
{
    int row;
    int col;

    for( row = 0; row < MATRIX_H_PREV; row++)
    {
        for( col = 0; col < MATRIX_W; col++)
        {
            if(preview_gamefield[row][col] != 0)
                preview_gamefield[row][col] = 0;
        }
    }
}

void refreshPreview(WINDOW* preview , tet* preview_piece)
{
    int i;
    int preview_tet = preview_piece->tet;
    int preview_ori = preview_piece->ori;

    /* Clearing the previeus box */
    werase(preview);
    for(i = 0; i < TETS_CELL; ++i)
    {
        wattron(preview, COLOR_PAIR(preview_tet + 10));
        cell = TETROMINOS[preview_tet][preview_ori][i];
        mvwprintw(preview, cell.row + 2, 1 + (cell.col + 2) * 2, "  ");
        wattroff(preview, COLOR_PAIR(preview_tet + 10));
    }
    wrefresh(preview);
}

void refreshGameField(int* x, tet* current_piece, int gamefield[][MATRIX_W], WINDOW* win)
{
    int i;
    resetPreview();
    wrefresh(win);

    int current_tet = current_piece->tet;
    int current_ori = current_piece->ori;

    for(i = 0; i < TETS_CELL; ++i)
    {
        wattron(win, COLOR_PAIR(current_tet + 10));
        cell = TETROMINOS[current_tet][current_ori][i];
        mvwprintw(win, cell.row, (cell.col) * 2 + *x, "  ");
        wattroff(win, COLOR_PAIR(current_tet + 10));

        if(cell.col + *x < 0)
        {
            colorField(gamefield, win);
            *x += 2;
            refreshGameField(x, current_piece, gamefield, win);
        }

        if( (cell.col) * 2 + *x > FIELD_W - 3)
        {
            colorField(gamefield, win);
            *x -= 2;
            refreshGameField(x, current_piece, gamefield, win);
        }
        preview_gamefield[cell.row][cell.col + (*x / 2)] = current_tet + 1;
    }
    wrefresh(win);
}

void refreshScore(WINDOW* s_score ,int pieces, int* score)
{
    werase(s_score);
    mvwprintw(s_score, 1, 1, "Disponibili: ");
    mvwprintw(s_score, 1, 14, "%d", pieces);
    mvwprintw(s_score, 3, 1, "Punteggio: ");
    mvwprintw(s_score, 3, 14, "%d", *score);
    wrefresh(s_score);
}

void printMatrix(int gameField[][MATRIX_W])
{
    int i,j;

    for(i = 0; i < MATRIX_H; i++)
    {
        for(j = 0; j < MATRIX_W; j++)
        {
            printf("%d" , gameField[i][j]);
        }
        printf("\n");
    }

    printf("\n\n");
    for ( i = 0; i < 7; i++)
    {
        printf("tetramino %d ha pezzi rimanenti = %d\n", i, tet_pieces[i]);
    }
}

int singlePlayerCommands(WINDOW* field)
{
    int moveMe = 0;
    moveMe = wgetch(field);
    switch (moveMe)
    {
        /* Arrow cases */
        case KEY_LEFT:
            return LEFT;
            break;

        case KEY_RIGHT:
            return RIGHT;
            break;

        case KEY_DOWN:
            return DOWN;
            break;

        /* Letter commands */
        case 'n':
            return NEXT;
            break;

        case 'b':
            return PREVIOUS;
            break;

        case 'r':
            return ROTATE;
            break;

        case 'x':
            return XRAY;
            break;

        case 'h':
            return HELP;
            break;

        case 'q':
            return QUIT;
            break;

        default:
            return 0;
    }
}

//* End the movement func ****************************************************/

void nextPiece(tet* current_piece, tet* preview_piece)
{
    current_piece->tet += 1;
    if(current_piece->tet < 6)
        preview_piece->tet = current_piece->tet + 1;
    if(current_piece->tet == 6)
        preview_piece->tet = 0;
    if(current_piece->tet > 6)
    {
        current_piece->tet = 0;
        preview_piece->tet = current_piece->tet + 1;
    }
}

void rotatingPiece(tet* current_piece)
{
    if(current_piece->ori == 3)
        current_piece->ori = 0;
    else
        current_piece->ori += 1;
}

void backPiece(tet* current_piece, tet* preview_piece)
{
    current_piece->tet -= 1;
    if ( current_piece->tet < 0 )
    {
        current_piece->tet = 6;
        preview_piece->tet  = 0;
    } else
        preview_piece->tet = current_piece->tet + 1;
}

void fallingPiece(int gamefield[][MATRIX_W], WINDOW* field)
{

    int row, col;
    int counter = 0;
    for(row = 0; row < TOP_LINE; row++)
    {
        for(col = 0; col < MATRIX_W; col++)
        {
            if(preview_gamefield[row][col] != 0)
            {   
                if(counter == 0 || counter > smallerIntervall(row, col, gamefield))
                    counter = smallerIntervall(row, col, gamefield);
            }
        }   
    }

    for(row = 0; row < MATRIX_H_PREV; row++)
    {
        for(col = 0; col < MATRIX_W; col++)     
        {
            if(preview_gamefield[row][col] != 0)
            {
                gamefield[row + counter][col] = preview_gamefield[row][col];
            }
        }
    }

    colorField(gamefield, field);
}

int smallerIntervall(int row, int col, int gamefield[][MATRIX_W])
{

    int counter = 0;
    int current_row;
    int current_col = col;

    for(current_row = MATRIX_H - 1; current_row > row; current_row--)
    {
        if(gamefield[current_row][current_col] != 0)
            counter = 0;
        else if(gamefield[current_row][current_col] == 0)
            counter++;
    }
    return counter;
}

int checkDeleteRows(int gamefield[][MATRIX_W], WINDOW* field)
{
    keypad(field, FALSE);
    int counter_rows  = 0;
    int is_delete_row = 0;
    int counter_numbers;

    int row, col;
    for (row = MATRIX_H - 1; row >= TOP_LINE; row--)
    {   
        
        is_delete_row = 0;
        counter_numbers = 0;
        for (col = 0; col < MATRIX_W; col++)
        {
            if (gamefield[row][col] != 0)
                counter_numbers += 1;
        }

        if (counter_numbers == MATRIX_W)
        {
            is_delete_row = 1;
            counter_rows += 1;
            halfdelay(10);
            goDownTetramini(row, gamefield);
            colorField(gamefield, field);
        }
        
        if (is_delete_row == 1)
            row = row + 1;
    }

    return calculateScoring(counter_rows);
}

int checkGameOver(int gamefield[][MATRIX_W])
{
    int col;
    for (col = 0; col < MATRIX_W; col++)
    {
        if (gamefield[TOP_LINE - 1][col] != 0)
            return 1;
    }
    return 0;
}

void goDownTetramini(int row, int gamefield[][MATRIX_W])
{
    int currentRow;
    int cols;

    int box = 0;
    int next_box = 0;

    for (cols = 0; cols < MATRIX_W; cols++)
        gamefield[row][cols] = 0;

    for (currentRow = row - 1; currentRow >= TOP_LINE; currentRow--)
    {
        for (cols = 0; cols < MATRIX_W; cols++)
        {
            box = gamefield[currentRow][cols];
            next_box = gamefield[currentRow + 1][cols];
            if (box != 0 && next_box == 0)
            {
                gamefield[currentRow + 1][cols] = box;
                gamefield[currentRow][cols] = 0;
            }
        }
    }
}

int calculateScoring(int rows)
{
    if (rows == 0) return 0;
    if (rows == 1) return 1;
    if (rows == 2) return 3;
    if (rows == 3) return 6;
    if (rows == 4) return 12;
    return 0;
}


