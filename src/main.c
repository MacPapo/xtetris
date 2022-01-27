#include "main.h"
#include "menu.h"
#include "singlePlayer.h"
#include "colors.h"
#include <ncurses.h>
#include <stdio.h>

int main()
{
    int mod = 0;

    /* Initialize the memory with ncurses session & start colors */
    initscr();
    initializeColors();

    do
    {
        mod = menu();
        if(mod == 0)
            singlePlayer();
        if(mod == 1)
            printw("MIAO");
        if(mod == 2)
            printw("ZAO");
    }
    while (mod != 4);

    /* Deallocates memory and ends ncurses */
    refresh();
    endwin();

    return 0;
}
