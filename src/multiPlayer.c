#include "multiPlayer.h"
#include "commonConfing.h"
#include <ncurses.h>


int multiPlayer()
{

    WINDOW *w_title = NULL, *w_field = NULL, *w_sfield = NULL, *w_preview = NULL, *w_score = NULL, *w_save = NULL, *w_cmds = NULL;
    WINDOW *s_preview = NULL, *s_score = NULL;

    /* clear the screen to visualize a new windows */
    clear();
    cbreak();

    int pieces =  T_NUM * (T_PIECES * 2);

    tet current_piece = {0,0};
    tet preview_piece = {0,0};

    int tetPieces[T_NUM];

    int choice = 0;
    int position_x = 0;
    int countCurrentPiece = 0;
    int turn = 0;

    player pg1 = addPlayer();
    player pg2 = addPlayer();
    initGameMatrix(pg1.gameField);
    initGameMatrix(pg1.gameField);

    initTetVector(tetPieces, 1);
    initMultiField(w_title, w_field, w_preview, w_sfield, w_score, w_save, w_cmds);

    pg1.window = initFirstPlayerWindow(pg1.window);
    pg2.window = initSecondPlayerWindow(pg2.window);
    s_preview = initMultiPreviewWindow(s_preview);
    s_score = initMultiScoreWindow(s_score);
    
    do
    {
        

    } while (pieces);
    

    refresh();
    getch();

    return 1;

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
