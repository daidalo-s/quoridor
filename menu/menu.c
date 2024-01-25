#include "./menu.h"
#include "../GLCD/GLCD.h"
#include "../CAN/CAN.h"
#include "../CAN/can_manager.h"

extern menu game_menu;
extern game_data game;

void menu_init(void)
{
    show_int0_message();
}

// ROTATION will be our select
void menu_manager(move_type move)
{
    if (game.game_status == MENU_MODE && game_menu.menu_window == MENU_INT0)
    {
        // the user pressed int0
        show_game_mode_menu();
        return;
    }
    // at this point we are or in the GAME_MODE_SELECT or in another submenu
    if (game_menu.menu_window == GAME_MODE_SELECT)
    {
        update_game_mode_menu(move);
        return;
    }
    if (game_menu.menu_window == SINGLE_BOARD)
    {
        update_single_board_menu(move);
        return;
    }
    if (game_menu.menu_window == MULTI_BOARD)
    {
        update_two_board_menu(move);
        return;
    }
}

void show_int0_message(void)
{
    LCD_Clear(Black);
    game.game_status = MENU_MODE;
    game_menu.menu_window = MENU_INT0;
    game.game_mode = DEFAULT;
    game.multi_board_master = 0;
    GUI_Text(X_INT0_MESSAGE_LINE_1, Y_INT0_MESSAGE_LINE_1, (ui8 *)"Press INT0 to start", White, Black);
    GUI_Text(X_INT0_MESSAGE_LINE_2, Y_INT0_MESSAGE_LINE_2, (ui8 *)"a game", White, Black);
}

void show_game_mode_menu(void)
{
    LCD_Clear(Black);
    game_menu.menu_window = GAME_MODE_SELECT;
    game_menu.option_num = 1;
    GUI_Text(SGM_X_TEXT_AREA_LINE_1, SGM_Y_TEXT_AREA_LINE_1, (ui8 *)"Select the", White, Black);
    GUI_Text(SGM_X_TEXT_AREA_LINE_2, SGM_Y_TEXT_AREA_LINE_2, (ui8 *)"GAME MODE", White, Black);
    GUI_Text(SGM_X_FIRST_SELECTION, SGM_Y_FIRST_SELECTION, (ui8 *)"Single board", Black, White);
    GUI_Text(SGM_X_SECOND_SELECTION, SGM_Y_SECOND_SELECTION, (ui8 *)"Two boards", White, Black);
}

void update_game_mode_menu(move_type move)
{
    if (move == ROTATION)
    {
        // we have selected the current option
        if (game_menu.option_num == 1)
        {
            show_single_board_menu();
            return;
        }
        if (game_menu.option_num == 2)
        {
            game.multi_board_master = 1;
            show_two_board_menu();
            return;
        }
    }

    if (move == DOWN)
    {
        if (game_menu.option_num == 1)
        {
            GUI_Text(SGM_X_FIRST_SELECTION, SGM_Y_FIRST_SELECTION, (ui8 *)"Single board", White, Black);
            GUI_Text(SGM_X_SECOND_SELECTION, SGM_Y_SECOND_SELECTION, (ui8 *)"Two boards", Black, White);
            game_menu.option_num = 2;
        }
    }
    else if (move == UP)
    {
        if (game_menu.option_num == 2)
        {
            GUI_Text(SGM_X_SECOND_SELECTION, SGM_Y_SECOND_SELECTION, (ui8 *)"Two boards", White, Black);
            GUI_Text(SGM_X_FIRST_SELECTION, SGM_Y_FIRST_SELECTION, (ui8 *)"Single board", Black, White);
            game_menu.option_num = 1;
        }
    }
}

void show_single_board_menu(void)
{
    LCD_Clear(Black);
    game_menu.menu_window = SINGLE_BOARD;
    game_menu.option_num = 1;
    GUI_Text(SSB_X_TEXT_AREA_LINE_1, SSB_Y_TEXT_AREA_LINE_1, (ui8 *)"Single-board: select", White, Black);
    GUI_Text(SSB_X_TEXT_AREA_LINE_2, SSB_Y_TEXT_AREA_LINE_2, (ui8 *)"the opposite player", White, Black);
    GUI_Text(SSB_X_FIRST_SELECTION, SSB_Y_FIRST_SELECTION, (ui8 *)"Human", Black, White);
    GUI_Text(SSB_X_SECOND_SELECTION, SSB_Y_SECOND_SELECTION, (ui8 *)"NPC", White, Black);
    GUI_Text(SSB_X_THIRD_SELECTION, SSB_Y_THIRD_SELECTION, (ui8 *)"Back", White, Black);
}

void update_single_board_menu(move_type move)
{
    if (move == ROTATION)
    {
        // human-npc
        if (game_menu.option_num == 1)
        {
            // qui dobbiamo inizializzare per giocare contro un umano come sempre fatto
            LCD_Clear(Black);
            game.game_mode = SINGLE_HUMAN_VS_HUMAN;
            game_start(&game);
            return;
        }
        if (game_menu.option_num == 2)
        {
            // qui dobbiamo inizializzare per giocare contro il bot
            LCD_Clear(Black);
            game.game_mode = SINGLE_HUMAN_VS_NPC;
            game_start(&game);
            return;
        }
        if (game_menu.option_num == 3)
        {
            show_game_mode_menu();
            return;
        }
    }
    if (move == DOWN)
    {
        if (game_menu.option_num == 1)
        {
            GUI_Text(SSB_X_FIRST_SELECTION, SSB_Y_FIRST_SELECTION, (ui8 *)"Human", White, Black);
            GUI_Text(SSB_X_SECOND_SELECTION, SSB_Y_SECOND_SELECTION, (ui8 *)"NPC", Black, White);
            game_menu.option_num = 2;
        }
        else if (game_menu.option_num == 2)
        {
            GUI_Text(SSB_X_SECOND_SELECTION, SSB_Y_SECOND_SELECTION, (ui8 *)"NPC", White, Black);
            GUI_Text(SSB_X_THIRD_SELECTION, SSB_Y_THIRD_SELECTION, (ui8 *)"Back", Black, White);
            game_menu.option_num = 3;
        }
    }
    else if (move == UP)
    {
        if (game_menu.option_num == 2)
        {
            GUI_Text(SSB_X_SECOND_SELECTION, SSB_Y_SECOND_SELECTION, (ui8 *)"NPC", White, Black);
            GUI_Text(SSB_X_FIRST_SELECTION, SSB_Y_FIRST_SELECTION, (ui8 *)"Human", Black, White);
            game_menu.option_num = 1;
        }
        else if (game_menu.option_num == 3)
        {
            GUI_Text(SSB_X_THIRD_SELECTION, SSB_Y_THIRD_SELECTION, (ui8 *)"Back", White, Black);
            GUI_Text(SSB_X_SECOND_SELECTION, SSB_Y_SECOND_SELECTION, (ui8 *)"NPC", Black, White);
            game_menu.option_num = 2;
        }
    }
}

void show_two_board_menu(void)
{
    LCD_Clear(Black);
    game_menu.menu_window = MULTI_BOARD;
    game_menu.option_num = 1;
    // if we are the first to reach this point we need to send the ack
    if (game.multi_board_master == 0)
    {
        // sono lo slave, mi ci hanno portato qua
        GUI_Text(0, 0, (uint8_t *)"sono lo slave", White, Black);
    }
    GUI_Text(STB_X_TEXT_AREA_LINE_1, STB_Y_TEXT_AREA_LINE_1, (ui8 *)"Two-boards: select", White, Black);
    GUI_Text(STB_X_TEXT_AREA_LINE_2, STB_Y_TEXT_AREA_LINE_2, (ui8 *)"your player", White, Black);
    GUI_Text(STB_X_FIRST_SELECTION, STB_Y_FIRST_SELECTION, (ui8 *)"Human", Black, White);
    GUI_Text(STB_X_SECOND_SELECTION, STB_Y_SECOND_SELECTION, (ui8 *)"NPC", White, Black);
    GUI_Text(STB_X_THIRD_SELECTION, STB_Y_THIRD_SELECTION, (ui8 *)"Back", White, Black);
}

void update_two_board_menu(move_type move)
{
    if (move == ROTATION)
    {
        // human-npc
        if (game_menu.option_num == 1)
        {
            // Quando entro qua devo capire se sono il master o lo slave
            if (game.multi_board_master)
            {
                // sono il master
                game.game_mode = MULTI_HUMAN;
                send_ack();
                GUI_Text(0, 0, (uint8_t *)"Waiting for a connection..", White, Black);
            }
            else
            {
                // sono lo slave
                game.game_mode = MULTI_HUMAN;
                send_ack();
                LCD_Clear(Black);
                game_start(&game);
            }
            // qui dobbiamo far giocare il bot
            // GUI_Text(0, 0, (uint8_t *)"Ready soon...", White, Black);
            return;
        }
        if (game_menu.option_num == 2)
        {
            // Quando entro qua devo capire se sono il master o lo slave
            if (game.multi_board_master)
            {
                // sono il master
                game.game_mode = MULTI_NPC;
                send_ack();
                GUI_Text(0, 0, (uint8_t *)"Waiting for a connection..", White, Black);
            }
            else
            {
                // sono lo slave
                game.game_mode = MULTI_NPC;
                send_ack();
                // devo startare il game, sono player_2
                LCD_Clear(Black);
                game_start(&game);
            }
            // qui dobbiamo giocare noi
            // GUI_Text(0, 0, (uint8_t *)"Ready soon...", White, Black);
            return;
        }
        if (game_menu.option_num == 3)
        {
            show_game_mode_menu();
            return;
        }
    }
    if (move == DOWN)
    {
        if (game_menu.option_num == 1)
        {
            GUI_Text(SSB_X_FIRST_SELECTION, SSB_Y_FIRST_SELECTION, (ui8 *)"Human", White, Black);
            GUI_Text(SSB_X_SECOND_SELECTION, SSB_Y_SECOND_SELECTION, (ui8 *)"NPC", Black, White);
            game_menu.option_num = 2;
        }
        else if (game_menu.option_num == 2)
        {
            GUI_Text(SSB_X_SECOND_SELECTION, SSB_Y_SECOND_SELECTION, (ui8 *)"NPC", White, Black);
            GUI_Text(SSB_X_THIRD_SELECTION, SSB_Y_THIRD_SELECTION, (ui8 *)"Back", Black, White);
            game_menu.option_num = 3;
        }
    }
    else if (move == UP)
    {
        if (game_menu.option_num == 2)
        {
            GUI_Text(SSB_X_SECOND_SELECTION, SSB_Y_SECOND_SELECTION, (ui8 *)"NPC", White, Black);
            GUI_Text(SSB_X_FIRST_SELECTION, SSB_Y_FIRST_SELECTION, (ui8 *)"Human", Black, White);
            game_menu.option_num = 1;
        }
        else if (game_menu.option_num == 3)
        {
            GUI_Text(SSB_X_THIRD_SELECTION, SSB_Y_THIRD_SELECTION, (ui8 *)"Back", White, Black);
            GUI_Text(SSB_X_SECOND_SELECTION, SSB_Y_SECOND_SELECTION, (ui8 *)"NPC", Black, White);
            game_menu.option_num = 2;
        }
    }
}
