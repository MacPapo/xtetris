#include "multiPlayer.h"
#include "commonConfing.h"
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

int multiPlayer()
{

    WINDOW *w_title = NULL, *w_field = NULL, *w_sfield = NULL, *w_preview = NULL, *w_score = NULL, *w_save = NULL, *w_cmds = NULL;
    WINDOW *s_preview = NULL, *s_score = NULL;

    /* clear the screen to visualize a new windows */
    clear();
    cbreak();

    int pieces =  T_NUM * (T_PIECES * 2);
    tet piece = {0,0};
    int tetPieces[T_NUM];
    int turn = 0;

    int lastAction = 0;


    player pg1 = addPlayer();
    player pg2 = addPlayer();
    initGameMatrix(pg1.gameField);
    initGameMatrix(pg2.gameField);

    initTetVector(tetPieces, 1);
    initMultiField(w_title, w_field, w_preview, w_sfield, w_score, w_save, w_cmds);

    pg1.window = initFirstPlayerWindow(pg1.window);
    pg2.window = initSecondPlayerWindow(pg2.window);
    s_preview = initMultiPreviewWindow(s_preview);
    s_score = initMultiScoreWindow(s_score);
    
    lastAction = startTheGame(&pg1, &pg2, &piece, tetPieces, &pieces, &turn, s_preview, s_score);

    if (lastAction == 1)
    {
        initWinner(&pg1, &pg2);
    }
        
    refresh();
    getch();

    return 1;

}

int startTheGame(player* pg1, player* pg2, tet* piece, int* tetPieces, int* pieces, int* turn, WINDOW* preview, WINDOW* score)
{
    int choice = 0;
    int action = 0;

    int positionX = 0;
    tet preview_piece = {piece->tet + 1, 0};

    int countCurrentPiece = 0;
    int *currentScore = &pg1->score;

    if (*pieces == 0)
        return 1;

    refreshPreview(preview, &preview_piece);
    refreshGameField(&positionX, piece, pg1);
    refreshMultiScore(tetPieces[piece->tet], currentScore, pg2->score, turn, score);

    do
    {
        keypad(pg1->window, TRUE);
        colorField(pg2);
        choice = wgetch(pg1->window);
        colorField(pg1);

        countCurrentPiece = tetPieces[piece->tet];
        if (countCurrentPiece == 0)
            changePiece(score);

        switch (choice)
        {
            case 'n':
                nextPiece(piece, &preview_piece);
                refreshMultiScore(tetPieces[piece->tet], currentScore, pg2->score, turn, score);
                break;

            /* case: back piece */
            case 'b':
                backPiece(piece, &preview_piece);
                refreshMultiScore(tetPieces[piece->tet], currentScore, pg2->score, turn, score);
                break;

            /* case: rotate piece */
            case 'r':
                rotatingPiece(piece);
                break;

            case KEY_LEFT:
                positionX -= 2;
                break;

            case KEY_RIGHT:
                positionX += 2;
                break;

            case KEY_DOWN:
                if (countCurrentPiece)
                {
                    fallingPiece(pg1);
                    tetPieces[piece->tet] -= 1;
                    *pieces -= 1;
                    *currentScore += checkAndReverseRows(pg1, pg2);
                    refreshMultiScore(tetPieces[piece->tet], currentScore, pg2->score, turn, score);
                    action = 1;
                }
                break;

            case 'q':
                initMultiQuit();
                return 0;
                break;

            case 'h':
                break;

            default:
                break;
        }

        refreshPreview(preview, &preview_piece);
        refreshGameField(&positionX, piece, pg1);

    } while (!action);
    
    if (checkGameOver(pg1->gameField))
    {
        multiGameOver(pg2->score, turn);
        return 0;
    }

    colorField(pg1);
    return startTheGame(pg2, pg1, piece, tetPieces, pieces, changeTurn(turn), preview, score);
    
}

void initMultiPlayerTitle(WINDOW* title)
{
    int titleY, titleX;

    titleY = HCENTER - MTITLE_H - 1;
    titleX = WCENTER - 3 - SCORE_W;

    title = newwin(MTITLE_H, MTITLE_W, titleY, titleX);
    box(title, V_LINES, H_LINES);
    wbkgd(title, COLOR_PAIR(2));
    mvwprintw(title, 1, 31, "MULTI PLAYER");
    wrefresh(title);
}

void initMultiPlayerField(WINDOW* firstField, WINDOW* secondField)
{
    int fieldY, fieldX;
    int sfieldY, sfieldX;

    fieldY = HCENTER;
    fieldX = WCENTER - SCORE_W;

    sfieldY = ((LINES  - FIELD_H) / 2);
    sfieldX = fieldX + FIELD_W + 2 + SCORE_W + 2;

    firstField = newwin(FIELD_H, FIELD_W, fieldY, fieldX);
    box(firstField, V_LINES, H_LINES);
    wbkgd(firstField, COLOR_PAIR(2));
    wrefresh(firstField);

    secondField = newwin(FIELD_H, FIELD_W, sfieldY, sfieldX);
    box(secondField, V_LINES, H_LINES);
    wbkgd(secondField, COLOR_PAIR(2));
    wrefresh(secondField);
}

void initMultiPlayerCmds(WINDOW *cmds)
{
    int cmdsY, cmdsX;

    cmdsY = HCENTER + FIELD_H + 1;
    cmdsX = WCENTER - SCORE_W - 3;

    cmds = newwin(MCMDS_H, MCMDS_W, cmdsY, cmdsX);
    box(cmds, V_LINES, H_LINES);
    wbkgd(cmds, COLOR_PAIR(2));
    mvwprintw(cmds, 0, 31, "| COMMANDS |");
    mvwprintw(cmds, 1, 10, "[ 'R' rotation ] [ 'N' next piece ] [ < ] [ v ] [ > ]");
    wrefresh(cmds);
}

void initMultiPlayerPreview(WINDOW* preview)
{
    int previewY, previewX;

    previewY = HCENTER;
    previewX = WCENTER - SCORE_W + FIELD_W + 2;

    preview = newwin(PREVIEW_H, PREVIEW_W, previewY, previewX);
    box( preview, V_LINES, H_LINES);
    wbkgd(preview, COLOR_PAIR(2));
    mvwprintw(preview , 0 , 5 , "| PREVIEW |");
    wrefresh(preview);
}

void initMultiPlayerScore(WINDOW* score)
{
    int scoreY, scoreX;

    scoreY = HCENTER + PREVIEW_H + 1;
    scoreX = WCENTER - SCORE_W + FIELD_W + 2;

    score = newwin(SCORE_H, SCORE_W, scoreY, scoreX);
    box(score , V_LINES, H_LINES);
    wbkgd(score, COLOR_PAIR(2));
    mvwprintw(score , 0 , 6 , "| SCORE |");
    wrefresh(score);
}

void initMultiPlayerSave(WINDOW* save)
{
    int saveY, saveX;

    saveY = HCENTER + PREVIEW_H + SCORE_H + 3;
    saveX = WCENTER - SCORE_W + FIELD_W + 2;

    save = newwin(SAVE_H, SAVE_W, saveY, saveX);
    box(save, V_LINES, H_LINES);
    wbkgd(save, COLOR_PAIR(3));
    mvwprintw(save, 0, 6, "| SAVE |");
    mvwprintw(save, 1, 1, "'H' to help page");
    mvwprintw(save, 2, 1, "'S' to save game");
    mvwprintw(save, 3, 1, "'Q' return to menu");
    wrefresh(save);
}

WINDOW* initFirstPlayerWindow(WINDOW* pgWindow)
{
    int fieldY, fieldX;
    fieldY   = HCENTER + 1;
    fieldX   = WCENTER - SCORE_W + 1;
    pgWindow = newwin(FIELD_H - 2, FIELD_W - 2, fieldY, fieldX);
    wbkgd(pgWindow, COLOR_PAIR(0));
    wrefresh(pgWindow);
    return pgWindow;
}

WINDOW* initMultiPreviewWindow(WINDOW* preview)
{
    int previewY, previewX;
    previewY = HCENTER + 1;
    previewX = WCENTER - SCORE_W + FIELD_W + 3;
    preview = newwin(PREVIEW_H - 2, PREVIEW_W - 2, previewY, previewX);
    wbkgd(preview, COLOR_PAIR(0));
    wrefresh(preview);
    return preview;
}

WINDOW* initMultiScoreWindow(WINDOW* score)
{
    int scoreY, scoreX;
    scoreY   = HCENTER + PREVIEW_H + 2;
    scoreX   = WCENTER - SCORE_W + FIELD_W + 3;
    score = newwin(SCORE_H - 2, SCORE_W - 2, scoreY, scoreX);        
    wbkgd(score, COLOR_PAIR(0));
    wrefresh(score);
    return score;
}

void initMultiField(WINDOW* title, WINDOW* firstField, WINDOW *secondField, WINDOW* preview, WINDOW* score, WINDOW* save, WINDOW* cmds)
{
    refresh();
    initMultiPlayerTitle(title);
    initMultiPlayerField(firstField, secondField);
    initMultiPlayerPreview(preview);
    initMultiPlayerScore(score);
    initMultiPlayerSave(save);
    initMultiPlayerCmds(cmds);
    refresh();
}

void refreshMultiScore(int pieces, int *pg1Score, int pg2Score,int *turn, WINDOW* score)
{
    werase(score);
    mvwprintw(score, 1, 1, "Disponibili: ");
    mvwprintw(score, 1, 14, "%d", pieces);
    mvwprintw(score, 3, 1, "Punteggio PG1: ");
    mvwprintw(score, 4, 1, "Punteggio PG2: ");
    if (*turn == FIRST_PLAYER)
    {
        mvwprintw(score, 3, 16, "%d", *pg1Score);
        mvwprintw(score, 4, 16, "%d", pg2Score);
    }
    if (*turn == SECOND_PLAYER)
    {
        mvwprintw(score, 4, 16, "%d", *pg1Score);
        mvwprintw(score, 3, 16, "%d", pg2Score);
    }  
    wrefresh(score);
}

void reverseRowsGameField(int secondGamefield[][MATRIX_W], int counterRows)
{
    int row, col;
    int cell;

    for (row = MATRIX_H - 1; row > (MATRIX_H - 1) - counterRows; row--)
    {
        for (col = 0; col < MATRIX_W; col++)
        {
            cell = secondGamefield[row][col];
            if ( cell == 0)
            {
                secondGamefield[row][col] = (rand() % (T_NUM - 1)) + 1;
            } else {
                secondGamefield[row][col] = 0;
            }
        }
    }
}

int checkAndReverseRows(player* pg1, player* pg2)
{
    keypad(pg1->window, FALSE);
    int counterRows  = 0;
    int isDeleteRow = 0;
    int counterNumbers;

    int row, col;
    for (row = MATRIX_H - 1; row >= TOP_LINE; row--)
    {   
        isDeleteRow = 0;
        counterNumbers = 0;
        for (col = 0; col < MATRIX_W; col++)
        {
            if (pg1->gameField[row][col] != 0)
                counterNumbers += 1;
        }

        if (counterNumbers == MATRIX_W)
        {
            isDeleteRow = 1;
            counterRows += 1;
            halfdelay(10);
            goDownTetramini(row, pg1->gameField);
            colorField(pg1);
        }
        
        if (isDeleteRow == 1)
            row = row + 1;
    }

    if (counterRows >= 3)
    {
        reverseRowsGameField(pg2->gameField, counterRows);
        colorField(pg2);
    }

    return calculateScoring(counterRows);
}

void multiGameOver(int score, int* turn)
{
    clear();

    int starty, startx;
    starty =  (LINES - GAMEOVER_H) / 2;
    startx =  (COLS  - GAMEOVER_W) / 2;
    
    refresh();
    WINDOW* w_gameover;
    w_gameover = newwin(GAMEOVER_H, GAMEOVER_W, starty, startx);
    box(w_gameover, V_LINES, H_LINES);
    wbkgd(w_gameover, COLOR_PAIR(3));
    mvwprintw(w_gameover, 0, 21, "| GAME OVER |");
    if (*turn == FIRST_PLAYER)
    {
        mvwprintw(w_gameover, 3,  2, "Second player win the game !!");
        mvwprintw(w_gameover, 5,  2, "Player's score: ");
        mvwprintw(w_gameover, 5,  17, "%d", score);
    } else if (*turn == SECOND_PLAYER) {
        mvwprintw(w_gameover, 3,  2, "First player win the game !!");
        mvwprintw(w_gameover, 5,  2, "Player's score: ");
        mvwprintw(w_gameover, 5,  17, "%d", score);
    }
    mvwprintw(w_gameover, 7, 21, "PRESS ANY KEY");
    wrefresh(w_gameover);
    refresh();
    getch();
}

int* changeTurn(int *turn)
{
    if (*turn == FIRST_PLAYER)
    {
        *turn = SECOND_PLAYER;
        return turn;
    }
    if (*turn == SECOND_PLAYER)
    {
        *turn = FIRST_PLAYER;
        return turn;
    }
    return turn;
}

void initMultiQuit()
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
    mvwprintw(w_quit, 3,  2, "IT WAS GREAT PLAY WITH YOU!");
    wrefresh(w_quit);
    getch();
    refresh();
}

void initWinner(player *pg1, player *pg2)
{
    clear();
    WINDOW* w_winner;
    int starty, startx;
    starty =  (LINES - MQUIT_H) / 2;
    startx =  (COLS  - MQUIT_W) / 2;

    refresh();
    w_winner = newwin(MQUIT_H, MQUIT_W, starty, startx);
    box(w_winner, V_LINES, H_LINES);
    wbkgd(w_winner, COLOR_PAIR(3));
    mvwprintw(w_winner, 0, 20, "| WINNER |");

    if (pg1->score > pg2->score)
    {
        mvwprintw(w_winner, 3,  2, "First player win the game !!");
        mvwprintw(w_winner, 5,  2, "Player's score: ");
        mvwprintw(w_winner, 5,  17, "%d", pg1->score);
    } else if (pg1->score < pg2->score) {
        mvwprintw(w_winner, 3,  2, "Second player win the game !!");
        mvwprintw(w_winner, 5,  2, "Player's score: ");
        mvwprintw(w_winner, 5,  17, "%d", pg2->score);
    } else {
        mvwprintw(w_winner, 3,  2, "Congratulations to both of you, no one wins");
    }
    wrefresh(w_winner);
    refresh();
    getch();
}




