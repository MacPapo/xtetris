#include "menu.h"
#include "colors.h"
#include <ncurses.h>
#include <string.h>


/* define the center of the screen */
#define HCENTER ((LINES - MENU_TITLE_H) / 2)
#define WCENTER (( COLS  - MENU_TITLE_W) / 2)

//* Prototype all the functions **************************************************/

/* Prototype the init functions for painting the objects */
void initTitle(WINDOW* title);
void initSwitcher(WINDOW* menu);
void initDescription(WINDOW* desc);
void initCmds(WINDOW* cmds);
void initMenu(WINDOW* menu, WINDOW* title, WINDOW* cmds, WINDOW* desc);

/* Prototype the text based functions for the highlight and the description */
void describeMod(WINDOW* desc, int mod);
void highlightMenu(WINDOW* menu, WINDOW* desc, int mod );

//* End of prototyping *******************************************************/

//* Main function ************************************************************/
int menu()
{
    /* declaring windows */
    WINDOW *w_menu = NULL, *w_title = NULL, *w_cmds = NULL, *w_desc = NULL;
    WINDOW *s_menu, *s_desc;

    int menuY, descY;
    int menuX, descX;
    int modCounter;
    int user_choice;

    /* initialize modCounter & userChoice */
    modCounter = user_choice = 0;

    /* defining parameter for creating menu box */
    menuY = HCENTER - (MENU_TITLE_Y) + 1;
    menuX = WCENTER + 1;
    /* defining parameters for creating description box*/
    descY =  HCENTER - (MENU_TITLE_Y) + 1;
    descX =  WCENTER + MENU_W + 3;

    /* background color & initialize the menu */
    bkgd(COLOR_PAIR(6));
    initMenu(w_menu, w_title, w_cmds, w_desc);

    /* creating the 2 little box that contain the text */
    s_desc = newwin(DESC_H - 2, DESC_W - 2, descY, descX);
    wbkgd(s_desc, COLOR_PAIR(3));
    s_menu = newwin(MENU_H - 2, DESC_W - 2, menuY, menuX);
    wbkgd(s_menu, COLOR_PAIR(3));

    /* defining key input & echo disable */
    keypad(s_menu, TRUE);
    noecho();
    curs_set(0);

    do
    {
        highlightMenu(s_menu, s_desc, modCounter);
        user_choice = wgetch(s_menu);
        switch (user_choice)
        {
            case KEY_UP:
                modCounter--;
                highlightMenu(s_menu, s_desc, modCounter);
                if(modCounter == -1)
                {
                    modCounter = 4;
                    halfdelay(1);
                }
                break;

            case KEY_DOWN:
                modCounter++;
                highlightMenu(s_menu, s_desc, modCounter);
                if(modCounter == 5)
                {
                    modCounter = 0;
                    halfdelay(1);
                }
                break;

            default:
                highlightMenu(s_menu, s_desc, modCounter);
                break;
        }
        halfdelay(1);
        if(user_choice == 10 && modCounter != 3)
        {
            return modCounter;
        }
    }
    while(user_choice != 'q');

    return 4;
}

//* End of Main function *****************************************************/


//* Init Funcs ***************************************************************/

void initTitle(WINDOW* title)
{
    int t_starty, t_startx;

    /* defining parameters for centering title box*/
    t_starty = HCENTER  - (MENU_TITLE_Y + MENU_Y);
    t_startx = WCENTER;
    /* creating title window */
    title = newwin(MENU_TITLE_H, MENU_TITLE_W, t_starty, t_startx); /* parameters ( higth, width, start_y, start_x  ) */
    box(title, BOX_TITLE_VE, BOX_TITLE_HO); /* creating title box window, parameters ( *window, vertical_line, horizontal_lines ) */
    wbkgd(title, COLOR_PAIR(2));
    mvwprintw(title, XTPP_Y, XTPP_X, "XTETRIS PROJECT");
    wrefresh(title);

}

void initSwitcher(WINDOW* menu)
{
    int m_starty, m_startx;

    /* defining parameters for centering menu box*/
    m_starty =  HCENTER - (MENU_TITLE_Y);
    m_startx =  WCENTER;
    /* creating menu window and window borders */
    menu = newwin(MENU_H, MENU_W, m_starty, m_startx);
    box(menu, BOX_MENU_VE, BOX_MENU_HO);
    wbkgd(menu, COLOR_PAIR(3));
    mvwprintw(menu, 0, PCENTER_M, "| GAME MENU |");
    wrefresh(menu);
}

void initDescription(WINDOW* desc)
{
    int d_starty, d_startx;

    /* defining parameters for creating description box*/
    d_starty =  HCENTER - (MENU_TITLE_Y);
    d_startx =  WCENTER + MENU_W + 2;
    /* creating description window and window borders */
    desc = newwin(DESC_H, DESC_W, d_starty, d_startx);
    box(desc, BOX_DESC_VE, BOX_DESC_HO);
    mvwprintw(desc, 0, PCENTER_D, "| DESCRIPTION |");
    wbkgd(desc, COLOR_PAIR(4));
    wrefresh(desc);
}

void initCmds(WINDOW* cmds)
{
    int c_starty, c_startx;

    /* defining parameters for centering cmds box*/
    c_starty = HCENTER - (MENU_TITLE_Y + MENU_Y) + MENU_CMDS_Y + 1;
    c_startx = WCENTER;
    /* creating cmds window and window borders */
    cmds = newwin(MENU_CMDS_H, MENU_CMDS_W, c_starty, c_startx);
    box(cmds, BOX_CMDS_VE, BOX_CMDS_HO);
    wbkgd(cmds, COLOR_PAIR(5));
    mvwprintw(cmds, 0, PCENTER_C, "| COMMANDS |");
    mvwprintw(cmds, CMDSP_Y, CMDSP_X, "[ UP ]  [ DOWN ]  [ ENTER to select ]");
    wrefresh(cmds);
}

void initMenu(WINDOW* menu, WINDOW* title, WINDOW* cmds, WINDOW* desc)
{
    refresh();
    initTitle(title);
    initSwitcher(menu);
    initDescription(desc);
    initCmds(cmds);
}

//* End Init Funcs **********************************************************/

//* Text based funcs *********************************************************/
void describeMod(WINDOW* desc, int mod)
{
    werase(desc);
    /* SINGLE PLAYER */
    if( mod == 0 )
    {
        mvwprintw(desc, 0, 2, "The player has 20 pieces");
        mvwprintw(desc, 1, 2, "for each tetramino.");
        mvwprintw(desc, 2, 2, "Rules for making points:");
        mvwprintw(desc, 3, 2, "1) One   row   1 pt");
        mvwprintw(desc, 4, 2, "2) Two   rows  3 pt");
        mvwprintw(desc, 5, 2, "3) Three rows  6 pt");
        mvwprintw(desc, 6, 2, "4) Four  rows 12 pt");
    }

    /* MULTI PLAYER*/
    if( mod == 1 )
    {
        mvwprintw(desc, 0, 2, "Player share 20 pieces");
        mvwprintw(desc, 1, 2, "for each tetramino.");
        mvwprintw(desc, 2, 2, "Rules for making points:");
        mvwprintw(desc, 3, 2, "1) One   row   1 pt");
        mvwprintw(desc, 4, 2, "2) Two   rows  3 pt");
        mvwprintw(desc, 5, 2, "3) Three rows  6 pt");
        mvwprintw(desc, 6, 2, "4) Four  rows 12 pt");
    }

    /* PLAYER VS CPU */
    if( mod == 2 )
    {
        mvwprintw(desc, 0, 2, "Player and CPU share 20");
        mvwprintw(desc, 1, 2, "pieces for each tetramino.");
        mvwprintw(desc, 2, 2, "Rules for making points:");
        mvwprintw(desc, 3, 2, "1) One   row   1 pt");
        mvwprintw(desc, 4, 2, "2) Two   rows  3 pt");
        mvwprintw(desc, 5, 2, "3) Three rows  6 pt");
        mvwprintw(desc, 6, 2, "4) Four  rows 12 pt");
    }

    /* ABOUT DEVELOPERS */
    if( mod == 3 )
    {
        mvwprintw(desc, 0, 2, "GITHUB:");
        mvwprintw(desc, 1, 2, "Jacopo Costantini");
        mvwprintw(desc, 2, 2, "-> @MacPapo");
        mvwprintw(desc, 3, 2, "Matteo Zambon");
        mvwprintw(desc, 4, 2, "-> @Blast291");
        mvwprintw(desc, 5, 2, "Alvise Silvestri");
        mvwprintw(desc, 6, 2, "-> @mastrodeimastri");
    }

    /* EXIT */
    if( mod == 4 )
    {
        mvwprintw(desc, 1, 2, "EXIT");
        mvwprintw(desc, 2, 2, "");
        mvwprintw(desc, 3, 2, "BY BY !! XD");
    }
    wrefresh(desc);
}

void highlightMenu(WINDOW* menu, WINDOW* desc, int mod )
{
    int i;
    char mod_list[NMOD][MOD_LENGHT] = {
        "Single Player",
        "Multi Player",
        "Player VS CPU",
        "About Developers",
        "Exit"
    };

    for( i = 0; i < NMOD; i++ )
    {
        if( i == mod )
        {
            wattrset(menu, A_STANDOUT);
            describeMod(desc, mod);
            wrefresh(desc);
        }

        mvwprintw(menu, i + 1, 2, "%s", mod_list[i]);
        wattroff(menu, A_STANDOUT );
        wrefresh(menu);
    }
}

//* End of Text based funcs **************************************************/
