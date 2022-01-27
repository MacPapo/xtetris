#include "commonConfing.h"

int previewGamefield[MATRIX_H][MATRIX_W];

/* tridimensional array for tetramini */
tet_location cell = {0,0};

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

player addPlayer()
{
    player player;
    player.score = 0;
    return player;
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

void refreshGameField(int* x, tet* current_piece, player *pg)
{
    int i;
    resetPreview();
    wrefresh(pg->window);

    int current_tet = current_piece->tet;
    int current_ori = current_piece->ori;

    for(i = 0; i < TETS_CELL; ++i)
    {
        wattron(pg->window, COLOR_PAIR(current_tet + 10));
        cell = TETROMINOS[current_tet][current_ori][i];
        mvwprintw(pg->window, cell.row, (cell.col) * 2 + *x, "  ");
        wattroff(pg->window, COLOR_PAIR(current_tet + 10));

        if(cell.col + *x < 0)
        {
            colorField(pg);
            *x += 2;
            refreshGameField(x, current_piece, pg);
        }

        if( (cell.col) * 2 + *x > FIELD_W - 3)
        {
            colorField(pg);
            *x -= 2;
            refreshGameField(x, current_piece, pg);
        }
        previewGamefield[cell.row][cell.col + (*x / 2)] = current_tet + 1;
    }
    initTopLine(pg->window);
    wrefresh(pg->window);
}
void resetPreview()
{
    int row;
    int col;

    for( row = 0; row < MATRIX_H_PREV; row++)
    {
        for( col = 0; col < MATRIX_W; col++)
        {
            if(previewGamefield[row][col] != 0)
                previewGamefield[row][col] = 0;
        }
    }
}
void initTopLine(WINDOW* field)
{
    int col;
    for (col = 0; col < FIELD_W - 2; col++)
    {
        wattron(field, COLOR_PAIR(19));
        mvwprintw(field, TOP_LINE - 1, col, " ");
        wattroff(field, COLOR_PAIR(19));
    }
    wrefresh(field);
}
void colorField(player *pg)
{
    int i, j;
    werase(pg->window);
    for(i = 0; i < MATRIX_H; i++)
    {
        for(j = 0; j < MATRIX_W; j++)
        {
            if(pg->gameField[i][j] != 0)
            {
                wattron(pg->window, COLOR_PAIR(pg->gameField[i][j] + 9));
                mvwprintw(pg->window, i, j*2, "  ");
                wattroff(pg->window, COLOR_PAIR(pg->gameField[i][j] + 9));
            } 
        }
    }
    initTopLine(pg->window);
    wrefresh(pg->window);
}
void initGameMatrix(int gameField[][MATRIX_W])
{
    int row, cols;
    for(row = 0; row < MATRIX_H; ++row)
        for(cols = 0; cols < MATRIX_W; ++cols)
        {
            gameField[row][cols] = 0;
            if(row < MATRIX_H_PREV)
                previewGamefield[row][cols] = 0;
        }
}
void initTetVector(int *tetPieces, int mod)
{  
    int i;
    if (mod == 0)
    {
        for (i = 0; i < T_NUM; i++)  
        tetPieces[i] = T_PIECES;
    }

    if (mod == 1 || mod == 2)
    {
        for (i = 0; i < T_NUM; i++)  
        tetPieces[i] = T_PIECES * 2;
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
