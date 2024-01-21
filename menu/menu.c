#include "./menu.h"
#include "../GLCD/GLCD.h"
#include "../game/quoridor.h"

void show_int0_message(void)
{
    LCD_Clear(Black);
    GUI_Text(X_INT0_MESSAGE_LINE_1, Y_INT0_MESSAGE_LINE_1, (ui8 *)"Press INT0 to start", White, Black);
    GUI_Text(X_INT0_MESSAGE_LINE_2, Y_INT0_MESSAGE_LINE_2, (ui8 *)"a game", White, Black);
}

void show_game_mode_menu(void)
{
    LCD_Clear(Black);
    GUI_Text(SGM_X_TEXT_AREA_LINE_1, SGM_Y_TEXT_AREA_LINE_1, (ui8 *)"Select the", White, Black);
    GUI_Text(SGM_X_TEXT_AREA_LINE_2, SGM_Y_TEXT_AREA_LINE_2, (ui8 *)"GAME MODE", White, Black);
    GUI_Text(SGM_X_FIRST_SELECTION, SGM_Y_FIRST_SELECTION, (ui8 *)"Single board", Black, White);
    GUI_Text(SGM_X_SECOND_SELECTION, SGM_Y_SECOND_SELECTION, (ui8 *)"Two boards", White, Black);
}

void show_single_board_menu(void)
{
    LCD_Clear(Black);
    GUI_Text(SSB_X_TEXT_AREA_LINE_1, SSB_Y_TEXT_AREA_LINE_1, (ui8 *)"Single-board: select", White, Black);
    GUI_Text(SSB_X_TEXT_AREA_LINE_2, SSB_Y_TEXT_AREA_LINE_2, (ui8 *)"the opposite player", White, Black);
    GUI_Text(SSB_X_FIRST_SELECTION, SSB_Y_FIRST_SELECTION, (ui8 *)"Human", Black, White);
    GUI_Text(SSB_X_SECOND_SELECTION, SSB_Y_SECOND_SELECTION, (ui8 *)"NPC", White, Black);
    GUI_Text(SSB_X_THIRD_SELECTION, SSB_Y_THIRD_SELECTION, (ui8 *)"Back", White, Black);
}

void show_two_board_menu(void)
{
    LCD_Clear(Black);
    GUI_Text(STB_X_TEXT_AREA_LINE_1, STB_Y_TEXT_AREA_LINE_1, (ui8 *)"Two-boards: select", White, Black);
    GUI_Text(STB_X_TEXT_AREA_LINE_2, STB_Y_TEXT_AREA_LINE_2, (ui8 *)"your player", White, Black);
    GUI_Text(STB_X_FIRST_SELECTION, STB_Y_FIRST_SELECTION, (ui8 *)"Human", Black, White);
    GUI_Text(STB_X_SECOND_SELECTION, STB_Y_SECOND_SELECTION, (ui8 *)"NPC", White, Black);
    GUI_Text(STB_X_THIRD_SELECTION, STB_Y_THIRD_SELECTION, (ui8 *)"Back", White, Black);
}