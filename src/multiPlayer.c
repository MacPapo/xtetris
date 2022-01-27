#include "multiPlayer.h"
#include "commonConfing.h"
#include <ncurses.h>

int multiPlayer()
{

    WINDOW *w_title = NULL, *w_field = NULL, *w_sfield = NULL, *w_preview = NULL, *w_score = NULL, *w_save = NULL, *w_cmds = NULL;
    WINDOW *s_preview, *s_score;

    /* clear the screen to visualize a new windows */
    clear();
    cbreak();

    int pieces;
    pieces = T_NUM * (T_PIECES * 2);

    int tetPieces[T_NUM];

    player pg1 = addPlayer();
    player pg2 = addPlayer();
    initGameMatrix(pg1.gameField);
    initGameMatrix(pg1.gameField);

    initTetVector(tetPieces, 1);
    initMultiField(w_title, w_field, w_preview, w_sfield, w_score, w_save, w_cmds);

    while (1)
    {
        /* code */
    }
    

    refresh();
    getch();

    return 1;

}

void initMultiPlayerTitle(WINDOW* title)
{
    int titleY, titleX;

    titleY = HCENTER - MTITLE_H - 1;
    titleX = WCENTER - 3 - SCORE_W / 2;

    title = newwin(MTITLE_H, MTITLE_W, titleY, titleX);
    box(title, V_LINES, H_LINES);
    wbkgd(title, COLOR_PAIR(2));
    mvwprintw(title, 1, 20, "MULTI PLAYER");
    wrefresh(title);
}

void initMultiPlayerField(WINDOW* firstField, WINDOW* secondField)
{
    int fieldY, fieldX;
    int sfieldY, sfieldX;

    fieldY = HCENTER;
    fieldX = WCENTER - SCORE_W/2;

    firstField = newwin(FIELD_H, FIELD_W, fieldY, fieldX);
    box(firstField, V_LINES, H_LINES);
    wbkgd(firstField, COLOR_PAIR(2));
    wrefresh(firstField);
}

void initMultiPlayerCmds(WINDOW *cmds)
{
    int cmdsY, cmdsX;

    cmdsY = HCENTER + FIELD_H + 1;
    cmdsX = WCENTER - SCORE_W/2 - 3;

    cmds = newwin(MCMDS_H, MCMDS_W, cmdsY, cmdsX);
    box(cmds, V_LINES, H_LINES);
    wbkgd(cmds, COLOR_PAIR(2));
    mvwprintw(cmds, 0, 19, "| COMMANDS |");
    mvwprintw(cmds, 1, 9, "[ 'R' ] [ 'N' ] [ < ] [ v ] [ > ]");
    wrefresh(cmds);
}

void initMultiField(WINDOW* title, WINDOW* firstField, WINDOW *secondField, WINDOW* preview, WINDOW* score, WINDOW* save, WINDOW* cmds)
{
    refresh();
    initMultiPlayerTitle(title);
    initMultiPlayerField(firstField, secondField);
    initSinglePlayerPreview(preview);
    initSinglePlayerScore(score);
    initSinglePlayerSave(save);
    initMultiPlayerCmds(cmds);
    refresh();
}
