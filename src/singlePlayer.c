#include "singlePlayer.h"
#include "commonConfing.h"
#include <ncurses.h>


int singlePlayer()
{
    /* declaring the game windows */
    WINDOW *w_title = NULL, *w_field = NULL, *w_preview = NULL, *w_score = NULL, *w_save = NULL, *w_cmds = NULL;
    WINDOW *s_preview = NULL, *s_score = NULL;

    tet current_piece = {0,0};
    tet preview_piece = {0,0};

    int choice = 0;
    int position_x = 0;
    int countCurrentPiece = 0;

    int pieces = T_NUM * T_PIECES;

    int tetPieces[T_NUM];
    player pg = addPlayer();

    /* Clear the screen to visualize a new window */
    clear();
    cbreak();

    /* initialize the game matrix & init the base screen & creating the refreshable windows */
    initGameMatrix(pg.gameField);
    initTetVector(tetPieces, 0);
    initField(w_title, w_field, w_preview, w_score, w_save, w_cmds);


    pg.window = initPlayerWindow(pg.window);
    s_preview = initPreviewWindow(s_preview);
    s_score = initScoreWindow(s_score);

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
    refresh();
    getch();
}

WINDOW* initPlayerWindow(WINDOW* pgWindow)
{
    int fieldY, fieldX;
    fieldY   = HCENTER + 1;
    fieldX   = WCENTER - SCORE_W/2 + 1;
    pgWindow = newwin(FIELD_H - 2, FIELD_W - 2, fieldY, fieldX);
    wbkgd(pgWindow, COLOR_PAIR(0));
    wrefresh(pgWindow);
    return pgWindow;
}

WINDOW* initPreviewWindow(WINDOW* preview)
{
    int previewY, previewX;
    previewY = HCENTER + 1;
    previewX = WCENTER - SCORE_W/2 + FIELD_W + 3;
    preview = newwin(PREVIEW_H - 2, PREVIEW_W - 2, previewY, previewX);
    wbkgd(preview, COLOR_PAIR(0));
    wrefresh(preview);
    return preview;
}

WINDOW* initScoreWindow(WINDOW* score)
{
    int scoreY, scoreX;
    scoreY   = HCENTER + PREVIEW_H + 2;
    scoreX   = WCENTER - SCORE_W/2 + FIELD_W + 3;
    score = newwin(SCORE_H - 2, SCORE_W - 2, scoreY, scoreX);        
    wbkgd(score, COLOR_PAIR(0));
    wrefresh(score);
    return score;
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
    mvwprintw(returnToMenu, 3,  2, "Sorry Bro! You have finisched all pieces! ");
    mvwprintw(returnToMenu, 4,  2, "Your score is: ");
    mvwprintw(returnToMenu, 4,  17, "%d", score);
    mvwprintw(returnToMenu, 7,  21, "PRESS ANY KEY");
    wrefresh(returnToMenu);
    getch();
    return 1;
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
