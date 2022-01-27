#include "singlePlayer.h"
#include "commonConfing.h"
#include <ncurses.h>

int singlePlayer()
{
    /* declaring the game windows */
    WINDOW *w_title = NULL, *w_field = NULL, *w_preview = NULL, *w_score = NULL, *w_save = NULL, *w_cmds = NULL;
    WINDOW *s_preview, *s_score;

    tet current_piece = {0,0};
    tet preview_piece = {0,0};

    /* initialize the variables and the matrix */
    int fieldY,   fieldX;
    int previewY, previewX;
    int scoreY,   scoreX;

    int choice = 0;
    int position_x = 0;
    int countCurrentPiece = 0;

    int pieces = T_NUM * T_PIECES;

    int tetPieces[T_NUM];
    player pg = addPlayer();

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
    initGameMatrix(pg.gameField);
    initTetVector(tetPieces, 0);
    initField(w_title, w_field, w_preview, w_score, w_save, w_cmds);

    /* automate these commands */
    {
        /* creating the refreshable windows */
        pg.window = newwin(FIELD_H - 2, FIELD_W - 2, fieldY, fieldX);
        s_preview = newwin(PREVIEW_H - 2, PREVIEW_W - 2, previewY, previewX);
        s_score = newwin(SCORE_H - 2, SCORE_W - 2, scoreY, scoreX);

        /* assign the proper color scheme */
        wbkgd(pg.window, COLOR_PAIR(0));
        wbkgd(s_preview, COLOR_PAIR(0));
        wbkgd(s_score, COLOR_PAIR(0));

        /* refresh those windows */
        wrefresh(s_preview);
        wrefresh(pg.window);
        wrefresh(s_score);
    }

    /* Single player routine */
    preview_piece.tet = current_piece.tet + 1;
    refreshPreview(s_preview ,&preview_piece);
    refreshGameField(&position_x, &current_piece, &pg);
    refreshScore(s_score ,tetPieces[current_piece.tet], pg.score);

    do
    {
        keypad(pg.window, TRUE);
        choice = wgetch(pg.window);
        colorField(&pg);

        countCurrentPiece = tetPieces[current_piece.tet];
        if (countCurrentPiece == 0) 
            changePiece(s_score); 

        switch (choice)
        {
            case 'n':
                nextPiece(&current_piece, &preview_piece);
                refreshScore(s_score, tetPieces[current_piece.tet], pg.score);
                break;

            case 'b':
                backPiece(&current_piece, &preview_piece);
                refreshScore(s_score, tetPieces[current_piece.tet], pg.score);
                break;
            
            case 'r':
                rotatingPiece(&current_piece);
                break;
            
            case 'h':
                break;
            
            case KEY_RIGHT:
                position_x += 2;
                break;
            
            case KEY_LEFT:
                position_x -= 2;
                break;
            
            case KEY_DOWN:
                if (countCurrentPiece)
                {
                    fallingPiece(&pg);
                    tetPieces[current_piece.tet] -= 1;
                    pieces -= 1; 
                    pg.score += checkDeleteRows(&pg);
                    refreshScore(s_score, tetPieces[current_piece.tet], pg.score);
                }
                break;

            case 'q':
                initQuit(pg.score);
                return 1;
                break;
        
            default:
                break;
        }

        if (checkGameOver(pg.gameField))
        {
            initGameOver(pg.score);
            return 0;
        }

        refreshPreview(s_preview, &preview_piece);
        refreshGameField(&position_x, &current_piece, &pg);
        
    } while(pieces);

    return initReturnToMenu(pg.score);
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

void initGameOver(int score)
{
    clear();
    int starty, startx;

    WINDOW* w_gameover;
    starty = (LINES - GAMEOVER_H) / 2;
    startx = (COLS  - GAMEOVER_W) / 2;

    refresh();

    w_gameover = newwin(GAMEOVER_H, GAMEOVER_W, starty, startx);
    box(w_gameover, V_LINES, H_LINES);
    wbkgd(w_gameover, COLOR_PAIR(3));
    mvwprintw(w_gameover, 0, 21, "| GAME OVER |");
    mvwprintw(w_gameover, 3,  2, "You lose bro!! try again");
    mvwprintw(w_gameover, 5,  2, "Your score is: ");
    mvwprintw(w_gameover, 5,  17, "%d", score);
    mvwprintw(w_gameover, 7, 21, "PRESS ANY KEY");
    wrefresh(w_gameover);
    getch();
}

void initQuit(int score)
{
    clear();
    WINDOW* w_quit;
    int starty, startx;
    starty =  (LINES - MQUIT_H) / 2;
    startx =  (COLS  - MQUIT_W) / 2;

    refresh();
    w_quit = newwin(MQUIT_H, MQUIT_W, starty, startx);
    box(w_quit, V_LINES, H_LINES);
    wbkgd(w_quit, COLOR_PAIR(3));
    mvwprintw(w_quit, 0, 20, "| QUIT |");
    mvwprintw(w_quit, 3,  2, "Your score is: ");
    mvwprintw(w_quit, 3,  17, "%d", score);
    mvwprintw(w_quit, 7,  21, "PRESS ANY KEY");
    wrefresh(w_quit);
    getch();
    refresh();
}

int initReturnToMenu(int score)
{
    clear();
    WINDOW* returnToMenu;
    int starty, startx;
    starty =  (LINES - MQUIT_H) / 2;
    startx =  (COLS  - MQUIT_W) / 2;

    refresh();
    returnToMenu = newwin(MQUIT_H, MQUIT_W, starty, startx);
    box(returnToMenu, V_LINES, H_LINES);
    wbkgd(returnToMenu, COLOR_PAIR(3));
    mvwprintw(returnToMenu, 0, 20, "| RETURN TO MENU |");
    mvwprintw(returnToMenu, 4,  2, "Sorry Bro! You have finisched all pieces! ");
    mvwprintw(returnToMenu, 4,  2, "Your score is: ");
    mvwprintw(returnToMenu, 4,  17, "%d", score);
    mvwprintw(returnToMenu, 7,  21, "PRESS ANY KEY");
    wrefresh(returnToMenu);
    getch();
    return 1;
}

void changePiece(WINDOW* score)
{
    mvwprintw(score, 5, (SCORE_W - 13) / 2, "CAMBIA PEZZO!");
    wrefresh(score);
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

void refreshScore(WINDOW* s_score ,int pieces, int score)
{
    werase(s_score);
    mvwprintw(s_score, 1, 1, "Disponibili: ");
    mvwprintw(s_score, 1, 14, "%d", pieces);
    mvwprintw(s_score, 3, 1, "Punteggio: ");
    mvwprintw(s_score, 3, 14, "%d", score);
    wrefresh(s_score);
}

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
        preview_piece->tet = 1;
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

void fallingPiece(player *pg)
{

    int row, col;
    int counter = 0;
    for(row = 0; row < TOP_LINE; row++)
    {
        for(col = 0; col < MATRIX_W; col++)
        {
            if(previewGamefield[row][col] != 0)
            {   
                if(counter == 0 || counter > smallerIntervall(row, col, pg->gameField))
                    counter = smallerIntervall(row, col, pg->gameField);
            }
        }   
    }

    for(row = 0; row < MATRIX_H_PREV; row++)
    {
        for(col = 0; col < MATRIX_W; col++)     
        {
            if(previewGamefield[row][col] != 0)
            {
                pg->gameField[row + counter][col] = previewGamefield[row][col];
            }
        }
    }

    colorField(pg);
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

int checkDeleteRows(player *pg)
{
    keypad(pg->window, FALSE);
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
            if (pg->gameField[row][col] != 0)
                counter_numbers += 1;
        }

        if (counter_numbers == MATRIX_W)
        {
            is_delete_row = 1;
            counter_rows += 1;
            halfdelay(10);
            goDownTetramini(row, pg->gameField);
            colorField(pg);
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