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
int calculateScoring(int rows)
{
    if (rows == 0) return 0;
    if (rows == 1) return 1;
    if (rows == 2) return 3;
    if (rows == 3) return 6;
    if (rows == 4) return 12;
    return 0;
}
