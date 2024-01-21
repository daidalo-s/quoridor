#ifndef MENU_H
#define MENU_H

#define ui8 uint8_t
#include "../game/quoridor.h"

// show_int0_message
#define X_INT0_MESSAGE_LINE_1 44
#define Y_INT0_MESSAGE_LINE_1 141
#define X_INT0_MESSAGE_LINE_2 96
#define Y_INT0_MESSAGE_LINE_2 (Y_INT0_MESSAGE_LINE_1 + 16 + 6)

// show_game_mode_menu
#define SGM_X_TEXT_AREA_LINE_1 80
#define SGM_Y_TEXT_AREA_LINE_1 112
#define SGM_X_TEXT_AREA_LINE_2 84
#define SGM_Y_TEXT_AREA_LINE_2 (SGM_Y_TEXT_AREA_LINE_1 + 6)
#define SGM_X_FIRST_SELECTION 72
#define SGM_Y_FIRST_SELECTION (SGM_Y_TEXT_AREA_LINE_2 + 20)
#define SGM_X_SECOND_SELECTION 80
#define SGM_Y_SECOND_SELECTION (SGM_Y_FIRST_SELECTION + 6)

// show_single_board_menu
#define SSB_X_TEXT_AREA_LINE_1 40
#define SSB_Y_TEXT_AREA_LINE_1 101
#define SSB_X_TEXT_AREA_LINE_2 44
#define SSB_Y_TEXT_AREA_LINE_2 (SSB_Y_TEXT_AREA_LINE_1 + 6)
#define SSB_X_FIRST_SELECTION 100
#define SSB_Y_FIRST_SELECTION (SSB_Y_TEXT_AREA_LINE_2 + 20)
#define SSB_X_SECOND_SELECTION 108
#define SSB_Y_SECOND_SELECTION (SSB_Y_FIRST_SELECTION + 6)
#define SSB_X_THIRD_SELECTION 104
#define SSB_Y_THIRD_SELECTION (SSB_Y_SECOND_SELECTION + 6)

// show_two_board_menu
#define STB_X_TEXT_AREA_LINE_1 48
#define STB_Y_TEXT_AREA_LINE_1 101
#define STB_X_TEXT_AREA_LINE_2 76
#define STB_Y_TEXT_AREA_LINE_2 (STB_Y_TEXT_AREA_LINE_1 + 6)
#define STB_X_FIRST_SELECTION 100
#define STB_Y_FIRST_SELECTION (STB_Y_TEXT_AREA_LINE_2 + 20)
#define STB_X_SECOND_SELECTION 108
#define STB_Y_SECOND_SELECTION (STB_Y_FIRST_SELECTION + 6)
#define STB_X_THIRD_SELECTION 104
#define STB_Y_THIRD_SELECTION (STB_Y_SECOND_SELECTION + 6)

typedef enum menu_window
{
    MENU_INT0,
    GAME_MODE_SELECT,
    SINGLE_BOARD,
    MULTI_BOARD
} menu_window;

typedef struct menu
{
    menu_window menu_window;
    ui8 option_num;
} menu;

void menu_init(void);
void menu_manager(void);
void show_int0_message(void);
void show_game_mode_menu(void);
void show_single_board_menu(void);
void show_two_board_menu(void);
#endif
