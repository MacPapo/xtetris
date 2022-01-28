#include "multiPlayer.h"
#include "cpu.h"
#include "commonConfing.h"
#include "singlePlayer.h"
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>


void colorFieldCPU(int gameField[][MATRIX_W], player *cpu)
{
    int i, j;
    werase(cpu->window);
    for(i = 0; i < MATRIX_H; i++)
    {
        for(j = 0; j < MATRIX_W; j++)
        {
            if(gameField[i][j] != 0)
            {
                wattron(cpu->window, COLOR_PAIR(gameField[i][j] + 9));
                mvwprintw(cpu->window, i, j*2, "  ");
                wattroff(cpu->window, COLOR_PAIR(gameField[i][j] + 9));
            } 
        }
    }
    initTopLine(cpu->window);
    wrefresh(cpu->window);
}

int checkLastRow(int gamefield[][MATRIX_W])
{
    int row = MATRIX_H - 1, cols = 0;
    for (cols = 0; cols < MATRIX_W; cols++)
    {
        if (gamefield[row][cols] != 0)
            return 1;
    }
    return 0;
}

void fallingAI(player *cpu)
{
    int row, col;
    int counter = 0;

    for(row = 0; row < MATRIX_H_PREV; row++)
    {
        for(col = 0; col < MATRIX_W; col++)
        {
            if(previewGamefield[row][col] != 0)
            {
                if(counter == 0 || (counter > smallerIntervall(row, col, cpu->gameField)))
                    counter = smallerIntervall(row, col, cpu->gameField);
            }
        }
    }

    for(row = 0; row < MATRIX_H_PREV; row++)
    {
        for(col = 0; col < MATRIX_W; col++)
        {
            if(previewGamefield[row][col] != 0)
            {
                cpu->gameField[row + counter][col] = previewGamefield[row][col];
            }
        }
    }
    /* sleep(1); */
    colorField(cpu);
}

void opening(player *cpu, int tetraminos[])
{
    int i;
    int tet_type;
    int tet_ori;
    int offset;

    resetPreview();
    wrefresh(cpu->window);
    srand(time(NULL));

    tet_type = rand() % T_NUM;
    tet_ori = rand() % T_ORI;
    offset =  rand() % 7;

    for(i = 0; i < TETS_CELL; i++)
    {
        cell = TETROMINOS[tet_type][tet_ori][i];
        previewGamefield[cell.row][cell.col + offset] = tet_type + 1;
    }
    colorFieldCPU(previewGamefield, cpu);
    fallingAI(cpu);
    tetraminos[tet_type] -= 1;
    wrefresh(cpu->window);
}

void fillPossibleChoices( int zeros, int* tet_type, int* tet_ori, int start, int* offset, int tetLeft[])
{
    tet possible_choiches[7];
    if(zeros == 1)
    {
        int isSelected = 0;

        while (isSelected != 1)
        {
            int randChoice = rand() % 7;

            if (randChoice == 0 && tetLeft[0] > 0)
            {
                *tet_type = possible_choiches[0].tet = 0;
                *tet_ori  = possible_choiches[0].ori = 1;
                isSelected = 1;
            }

            if (randChoice == 1 && start != 9 && tetLeft[1] > 0)
            {
                *tet_type = possible_choiches[1].tet = 1;
                *tet_ori  = possible_choiches[1].ori = 1;
                isSelected = 1;
            }

            if (randChoice == 2 && start <= 7 && tetLeft[2] > 0)
            {
                *tet_type = possible_choiches[2].tet = 2;
                *tet_ori  = possible_choiches[2].ori = 0;
                isSelected = 1;
            }

            if (randChoice == 3 && start != 0 && tetLeft[4] > 0)
            {
                *tet_type = possible_choiches[3].tet = 4;
                *tet_ori  = possible_choiches[3].ori = 1;
                *offset -= 1;
                isSelected = 1;
            }

            if (randChoice == 4 && start <= 8 && tetLeft[6] > 0)
            {
                *tet_type = possible_choiches[4].tet = 6;
                *tet_ori  = possible_choiches[4].ori = 1;
                isSelected = 1;
            }

            if (randChoice == 5 && start != 0 && tetLeft[5] > 0)
            {
                *tet_type = possible_choiches[5].tet = 5;
                *tet_ori  = possible_choiches[5].ori = 1;
                *offset -= 1;
                isSelected = 1;
            }

            if (randChoice == 6 && start != 9 && tetLeft[5] > 0)
            {
                *tet_type = possible_choiches[6].tet = 5;
                *tet_ori  = possible_choiches[6].ori = 3;
                isSelected = 1;
            }
        }

    }
    if(zeros == 2)
    {
        int isSelected = 0;

        while (isSelected != 1)
        {
            int randChoice = rand() % 5;

            if (randChoice == 0 && tetLeft[0] > 0)
            {
                *tet_type = possible_choiches[0].tet = 1;
                *tet_ori  = possible_choiches[0].ori = 3;
                isSelected = 1;
            }

            if (randChoice == 1 && tetLeft[1] > 0)
            {
                *tet_type = possible_choiches[1].tet = 2;
                *tet_ori  = possible_choiches[1].ori = 1;
                isSelected = 1;
            }

            if (randChoice == 2 && tetLeft[2] > 0)
            {
                *tet_type = possible_choiches[2].tet = 3;
                *tet_ori  = possible_choiches[2].ori = 0;
                isSelected = 1;
            }

            if (randChoice == 3 && start <= 7 && tetLeft[4] > 0)
            {
                *tet_type = possible_choiches[3].tet = 4;
                *tet_ori  = possible_choiches[3].ori = 0;
                isSelected = 1;
            }

            if (randChoice == 4 && start != 0 && tetLeft[6] > 0)
            {
                *tet_type = possible_choiches[4].tet = 6;
                *tet_ori  = possible_choiches[4].ori = 0;
                *offset -= 1;
                isSelected = 1;
            }
        }
    }
    if(zeros == 3)
    {
        int isSelected = 0;

        while (isSelected != 1)
        {
            int randChoice = rand() % 3;

            if (randChoice == 0 && tetLeft[1] > 0)
            {
                *tet_type = possible_choiches[0].tet = 1;
                *tet_ori  = possible_choiches[0].ori = 2;
                isSelected = 1;
            }

            if (randChoice == 1 && tetLeft[2] > 0)
            {
                *tet_type = possible_choiches[1].tet = 2;
                *tet_ori  = possible_choiches[1].ori = 2;
                isSelected = 1;
            }
            if (randChoice == 2 && tetLeft[5] > 0)
            {
                *tet_type = possible_choiches[2].tet = 5;
                *tet_ori  = possible_choiches[2].ori = 2;
                isSelected = 1;
            }
        }
    }

    if (zeros >= 4)
    {
        int isSelected = 0;

        while (isSelected != 1)
        {
            int randChoice = rand() % 4;

            if (randChoice == 0 && tetLeft[1] > 0)
            {
                *tet_type = possible_choiches[0].tet = 1;
                *tet_ori  = possible_choiches[0].ori = 2;
                isSelected = 1;
            }

            if (randChoice == 1 && tetLeft[2] > 0)
            {
                *tet_type = possible_choiches[1].tet = 2;
                *tet_ori  = possible_choiches[1].ori = 2;
                isSelected = 1;
            }
            if (randChoice == 2 && tetLeft[5] > 0)
            {
                *tet_type = possible_choiches[2].tet = 5;
                *tet_ori  = possible_choiches[2].ori = 2;
                isSelected = 1;
            }
            if (randChoice == 3 && tetLeft[0] > 0)
            {
                *tet_type = possible_choiches[0].tet = 0;
                *tet_ori  = possible_choiches[0].ori = 0;
                isSelected = 1;
            }
        }
    }
}

int fullConsecutive(int row, int gamefield[][MATRIX_W])
{
    int cols;
    int counter = 0;
    int max = 0;
    for (cols = 0; cols < MATRIX_W - 1; cols++)
    {
        if (gamefield[row][cols] != 0 && gamefield[row][cols + 1] != 0)
            counter++;
    }

    if (counter > max)
        return counter;
    return max;
}

int isCompletable(int row, int gamefield[][MATRIX_W])
{
    int cols;

    if (row == TOP_LINE)
        return 1;

    for (cols = 0; cols < MATRIX_W; ++cols) {
        if (gamefield[row][cols] == 0 && gamefield[row - 1][cols] != 0)
            return 0;
    }
    isCompletable(row - 1, gamefield);
    return 0;
}

int completionSpot(int bestRow, int gamefield[][MATRIX_W], int* start)
{
    int cols;
    int counterMinZeros = 0;
    int minZeros = 0;
    for (cols = 0; cols < MATRIX_W; cols++)
    {
        if (gamefield[bestRow][cols] == 0 )
            counterMinZeros++;
        if(gamefield[bestRow][cols] != 0 || cols == MATRIX_W - 1)
        {
            if (!minZeros || (counterMinZeros < minZeros && counterMinZeros > 0))
            {
                minZeros = counterMinZeros;
                if(cols == MATRIX_W - 1 && gamefield[bestRow][cols] == 0)
                {
                    *start = MATRIX_W - minZeros;
                }
                else {
                    *start = cols - minZeros;
                }
            }
            counterMinZeros = 0;
        }
    }
    return minZeros;
}

int belowControl(int bestRow, int start, int zero, int gamefield[][MATRIX_W])
{
    int cols, row;
    int below = 0;

    for (row = bestRow; row < MATRIX_H; ++row) {
        for (cols = start; cols <= (MATRIX_W - zero); cols++) {
            if (gamefield[row + 1][cols] == 0) {
                below++;
            }
        }
    }

    if( below > (zero/2) )
        return below;
    else
        return 0;
}

int analizeRows(player *cpu)
{
    int row;
    int counter = 0;
    int best_row = 0;
    int firstCompletable = 6;

    for (row = 5; row < MATRIX_H; ++row)
    {
        if( isCompletable(row, cpu->gameField) && counter <= fullConsecutive(row, cpu->gameField))
        {
            if (firstCompletable == 6)
                firstCompletable = row;
            counter = fullConsecutive(row, cpu->gameField);
            best_row = row;
        }
    }
    return best_row;
}

void bigBrain(player *cpu, int* bestRow, int* zeroInterval, int tetraminos[])
{
    /* init some handy variable */
    int i;
    int firstBestRow    = 0;
    int firstClearSpots = 0;
    int start = 0;
    int stampa = 0;

    /*rand*/
    int tet_type = 0;
    int tet_ori  = 0;

    int offset;

    if (!checkLastRow(cpu->gameField))
        opening(cpu, tetraminos);
    else {
        resetPreview();
        *bestRow = analizeRows(cpu);
        firstBestRow = *bestRow;
        *zeroInterval = completionSpot(firstBestRow, cpu->gameField, &start);
        firstClearSpots = *zeroInterval;

        offset = start;
        fillPossibleChoices(firstClearSpots, &tet_type, &tet_ori, start, &offset, tetraminos);
        if(firstBestRow != 24)
            stampa = belowControl(firstBestRow, start, firstClearSpots, cpu->gameField);

        wrefresh(cpu->window);
        for(i = 0; i < TETS_CELL; i++)
        {
            cell = TETROMINOS[tet_type][tet_ori][i];
            previewGamefield[cell.row][cell.col + offset] = tet_type + 1;
        }

        colorFieldCPU(previewGamefield, cpu);
        fallingAI(cpu);
        tetraminos[tet_type] -= 1;

    }
}

void refreshCpuScore(int pieces, int *pg1Score, int *pg2Score, WINDOW* score)
{
    werase(score);
    mvwprintw(score, 1, 1, "Disponibili: ");
    mvwprintw(score, 1, 14, "%d", pieces);
    mvwprintw(score, 3, 1, "Punteggio P1: ");
    mvwprintw(score, 4, 1, "Punteggio P2: ");
    mvwprintw(score, 3, 14, "%d", *pg1Score);
    mvwprintw(score, 4, 14, "%d", *pg2Score);
    wrefresh(score);
}

void startCpuGame(player *pg1, player* cpu, tet *piece, int* tetPieces, WINDOW* s_preview, WINDOW* s_score)
{
    int pieces = T_NUM * (T_PIECES * 2);
    int quit = 0;
    int choice = 0;
    int countCurrentPiece;

    int bestRow = 0;
    int zeroInterval = 0;

    int position_x = 0;

    int *currentScore = &pg1->score;
    int *cpuScore = &cpu->score;

    tet preview_piece = {piece->tet + 1, 0};

    refreshPreview(s_preview, &preview_piece);
    refreshGameField(&position_x, piece, pg1);
    refreshCpuScore(tetPieces[piece->tet], currentScore, cpuScore, s_score);

    do
    {
        keypad(s_preview, TRUE);
        choice = wgetch(s_preview);
        colorField(pg1);

        countCurrentPiece = tetPieces[piece->tet];
        if (countCurrentPiece == 0)
            changePiece(s_score);

        switch (choice)
        {

            case 'n':
                nextPiece(piece, &preview_piece);
                refreshCpuScore(tetPieces[piece->tet], currentScore, cpuScore, s_score);
                break;

            /* case: back piece */
            case 'b':
                backPiece(piece, &preview_piece);
                refreshCpuScore(tetPieces[piece->tet], currentScore, cpuScore, s_score);
                break;

            /* case: rotate piece */
            case 'r':
                rotatingPiece(piece);
                break;

            case KEY_LEFT:
                position_x -= 2;
                break;
            
            case KEY_RIGHT:
                position_x += 2;
                break;

            case KEY_DOWN:
                if (countCurrentPiece)
                {
                    fallingPiece(pg1);
                    tetPieces[piece->tet] -= 1;
                    pieces -= 1;
                    bigBrain(cpu, &bestRow, &zeroInterval, tetPieces);
                    *currentScore += checkAndReverseRows(pg1, cpu);
                    *cpuScore += checkAndReverseRows(cpu, pg1);
                    wrefresh(cpu->window);
                    refreshCpuScore(tetPieces[piece->tet], currentScore, cpuScore, s_score);
                }                
                break;

            case 'q':
                quit = 1;
                break;

            default:
                break;
        }

        if (checkGameOver(pg1->gameField))
        {
            initGameOver(pg1->score);
            return; 
        } else if (checkGameOver(cpu->gameField)) {
            initGameOver(cpu->score);
            return ;
        }

        if (pieces == 0 || quit)
        {
            initGameOver(pg1->score);
            return ;
        }
            
        refreshPreview(s_preview, &preview_piece);
        refreshGameField(&position_x, piece, pg1);
        
    } while (pieces&& !quit);

    return ;
}

int CPU()
{
    WINDOW *w_title = NULL, *w_field = NULL, *w_sfield = NULL, *w_preview = NULL, *w_score = NULL, *w_save = NULL, *w_cmds = NULL;
    WINDOW *s_preview = NULL, *s_score = NULL;

    /* clear the screen to visualize a new windows */
    clear();
    cbreak();

    tet piece = {0,0};
    int tetPieces[T_NUM];

    int lastAction = 0;

    player pg1 = addPlayer();
    player cpu = addPlayer();
    initGameMatrix(pg1.gameField);
    initGameMatrix(cpu.gameField);

    initTetVector(tetPieces, 1);
    initMultiField(w_title, w_field, w_preview, w_sfield, w_score, w_save, w_cmds);

    pg1.window = initFirstPlayerWindow(pg1.window);
    cpu.window = initSecondPlayerWindow(cpu.window);
    s_preview = initMultiPreviewWindow(s_preview);
    s_score = initMultiScoreWindow(s_score);
    
    startCpuGame(&pg1, &cpu, &piece, tetPieces, s_preview, s_score);

    if (lastAction == 1)
    {
        initWinner(&pg1, &cpu);
    }
        
    refresh();
    getch();

    return 3;
}