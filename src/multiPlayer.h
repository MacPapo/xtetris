#ifndef MULTIPLAYER_H_
#define MULTIPLAYER_H_

#include <stdbool.h>
#include <ncurses.h>

/* ---------------------------------------------------------- defining title parameters */
/* Multi fields */
#define MTITLE_H         3
#define MTITLE_W        75

#define CMDS_H           3
#define CMDS_W          51

#define MPLAYER_X       31
#define PVSCPU_X        31

#define MCMDS_H          3
#define MCMDS_W         75
/* ---------------------------------------------------------- END defining title parameters */

#define FIRST_PLAYER     1

#define SECOND_PLAYER    0

void initMultiPlayerCmds(WINDOW* w_cmds);
void initMultiPlayerTitle(WINDOW* title);
void initMultiPlayerField(WINDOW* firstField, WINDOW* secondField);
void initMultiField(WINDOW* title, WINDOW* firstField, WINDOW* secondField, WINDOW* preview, WINDOW* score, WINDOW* save, WINDOW* cmds);

int multiPlayer();

#endif 
