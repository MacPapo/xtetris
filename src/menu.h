#ifndef menu_h
#define menu_h

/* defining number of modalities */
#define NMOD             5
#define MOD_LENGHT      20

/* defining XTETRIS PROJECT (title) print Y, X */
#define XTPP_Y           1
#define XTPP_X          24

/* defining title window parameters */
#define MENU_TITLE_Y          1
#define MENU_TITLE_X          3

#define MENU_TITLE_H          3
#define MENU_TITLE_W         64

/* defining box title parameters */
#define BOX_TITLE_VE    ACS_VLINE
#define BOX_TITLE_HO    ACS_HLINE

/* defining menu window parameters */
#define MENU_Y           4
#define MENU_X           3

#define MENU_H           9
#define MENU_W          31

/* defining the center position of GAME MENU print */
#define PCENTER_M        9

/* defining box menu parameters */
#define BOX_MENU_VE     ACS_VLINE
#define BOX_MENU_HO     ACS_HLINE

/* defining description window parameters */
#define DESC_Y           4
#define DESC_X           3

#define DESC_H           9
#define DESC_W          31

#define PCENTER_D        8

/* defining box description parameters */
#define BOX_DESC_VE     ACS_VLINE
#define BOX_DESC_HO     ACS_HLINE

/* defining cmds window parameters */
#define MENU_CMDS_Y          13
#define MENU_CMDS_X           3

#define MENU_CMDS_H           3
#define MENU_CMDS_W          64

#define PCENTER_C       26

/* defining box cmds parameters*/
#define BOX_CMDS_VE     ACS_VLINE
#define BOX_CMDS_HO     ACS_HLINE

/* defining commands print Y, X */
#define CMDSP_Y          1
#define CMDSP_X         13

/* defining byby window parameters */
#define BYBY_Y          13
#define BYBY_X           3

#define BYBY_H           9
#define BYBY_W          54

/* defining box byby parameters */
#define BOX_BYBY_VE     ACS_VLINE
#define BOX_BYBY_HO     ACS_HLINE

/* defining quit window parameters */
#define QUIT_H          10
#define QUIT_W          10

/* difining box quit parameters */
#define BOX_QUIT_VE     ACS_VLINE
#define BOX_QUIT_HO     ACS_HLINE

/*defining functions*/
int menu();

#endif
