#include "graphics.h"
#include "../game/quoridor.h"
#include "../GLCD/GLCD.h"
#include <stdio.h>
#include "../images/disco_dance.c"
#include "../images/the_pirate.c"

extern game_data game;

void draw_board(void)
{
    int i;
    int j;
    screen_point point;
    point.x = 5;
    point.y = 5;

    for (i = 0; i < 7; i++)
    {
        // Qua dentro mi occupo delle righe
        point.y = 5 + (i * ITERATION_OFFSET);
        for (j = 0; j < 7; j++)
        {
            // Qua dentro mi occupo delle colonne
            point.x = 5 + (j * ITERATION_OFFSET);
            LCD_DrawLine(point.x, point.y, point.x, point.y + SQUARE_SIDE, White);
            LCD_DrawLine(point.x, point.y + SQUARE_SIDE, point.x + SQUARE_SIDE, point.y + SQUARE_SIDE, White);
            LCD_DrawLine(point.x + SQUARE_SIDE, point.y + SQUARE_SIDE, point.x + SQUARE_SIDE, point.y, White);
            LCD_DrawLine(point.x + SQUARE_SIDE, point.y, point.x, point.y, White);
        }
    }

    // Drawing the data_area_rectangle in the order | _ | -
    point.x = 5;
    point.y = Y_DATA_AREA;
    LCD_DrawLine(point.x, point.y, point.x, point.y + HEIGHT_DATA_AREA, White);
    LCD_DrawLine(point.x, point.y + HEIGHT_DATA_AREA, point.x + WIDTH_DATA_AREA, point.y + HEIGHT_DATA_AREA, White);
    LCD_DrawLine(point.x + WIDTH_DATA_AREA, point.y + HEIGHT_DATA_AREA, point.x + WIDTH_DATA_AREA, point.y, White);
    LCD_DrawLine(point.x, point.y, point.x + WIDTH_DATA_AREA, point.y, White);
    // Adding the text in the two wall's rectangles, the code is custom
    // P1
    GUI_Text(X_P1_TEXT_AREA_BEGIN, Y_P1_TEXT_AREA_BEGIN, "P1 Walls 8", White, Black);
    LCD_DrawLine(X_TIME_AREA_BEGIN - 11, Y_DATA_AREA, X_TIME_AREA_BEGIN - 11, Y_DATA_AREA + HEIGHT_DATA_AREA, White);
    GUI_Text(X_TIME_AREA_BEGIN, Y_P1_TEXT_AREA_BEGIN, "20s", White, Black);
    LCD_DrawLine(X_P2_TEXT_AREA_BEGIN - 9, Y_DATA_AREA, X_P2_TEXT_AREA_BEGIN - 9, Y_DATA_AREA + HEIGHT_DATA_AREA, White);
    GUI_Text(X_P2_TEXT_AREA_BEGIN, Y_P1_TEXT_AREA_BEGIN, "P2 Walls 8", Red, Black);
}

/**
 * @brief This function draws the player token based on the x-y matrix coordinates passed in input
 * @param x -> a board row index
 * @param y -> a board column index
 * @param player
 */
void draw_player_token(ui8 x, ui8 y, active_player player)
{
    screen_point point;
    int i, j;
    ui16 offset = 0;
    point.y = 5 + ((x / 2) * ITERATION_OFFSET) + 1;
    for (i = 0; i < SQUARE_SIDE - 1; i++)
    {
        // Its on the horizontal axes so we need to give the y
        point.x = point.x = 5 + ((y / 2) * ITERATION_OFFSET) + 1;
        for (j = 0; j < SQUARE_SIDE - 1; j++)
        {
            if (player == PLAYER_1)
            {
                LCD_SetPoint(point.x, point.y, RGB565CONVERT(the_pirate.pixel_data[offset], the_pirate.pixel_data[offset + 1], the_pirate.pixel_data[offset + 2]));
            }
            else
            {
                LCD_SetPoint(point.x, point.y, RGB565CONVERT(disco_dance.pixel_data[offset], disco_dance.pixel_data[offset + 1], disco_dance.pixel_data[offset + 2]));
            }
            point.x++;
            offset += 3;
        }
        point.y++;
    }
}

/**
 * @brief With this function we simply un-draw the player token given the two coordinates as input
 * @param x
 * @param y
 * @param player
 */
void delete_player_token(ui8 x, ui8 y)
{
    screen_point point;
    int i, j;
    point.y = 5 + ((x / 2) * ITERATION_OFFSET) + 1;
    for (i = 0; i < SQUARE_SIDE - 1; i++)
    {
        // Its on the horizontal axes so we need to give the y
        point.x = point.x = 5 + ((y / 2) * ITERATION_OFFSET) + 1;
        for (j = 0; j < SQUARE_SIDE - 1; j++)
        {
            LCD_SetPoint(point.x, point.y, RGB565CONVERT(0, 0, 0));
            point.x++;
        }
        point.y++;
    }
}

/**
 * @brief Given the new coordinates of the active_player, this function deletes
 * the player token from the board by showing again the possible moves and then
 * draws the token again in the new position
 * @param x -> the new x matrix coordinate
 * @param y -> the new y matrix coordinate
 */
void update_player_token_pos(ui8 x, ui8 y)
{
    player current_player;
    game.player_turn == PLAYER_1 ? (current_player = game.player_1) : (current_player = game.player_2);
    delete_player_token(current_player.x_matrix_coordinate, current_player.y_matrix_coordinate);
    show_available_moves();
    draw_player_token(x, y, game.player_turn);
}

/**
 * @brief Given board matrix coordinates, this function draws it as an available
 * move cell
 * @param x
 * @param y
 */
void draw_available_move(ui8 x, ui8 y)
{
    int i;
    int j;
    screen_point point;
    point.y = 5 + ((x / 2) * ITERATION_OFFSET) + 1;
    for (i = 0; i < SQUARE_SIDE - 1; i++)
    {
        // Its on the horizontal axes so we need to give the y
        point.x = point.x = 5 + ((y / 2) * ITERATION_OFFSET) + 1;
        for (j = 0; j < SQUARE_SIDE - 1; j++)
        {
            LCD_SetPoint(point.x, point.y, RGB565CONVERT(251, 241, 167));
            point.x++;
        }
        point.y++;
    }
}

/**
 * @brief This function simply clears the available move cell
 * @param x
 * @param y
 */
void clear_available_move(ui8 x, ui8 y)
{
    int i;
    int j;
    screen_point point;
    point.y = 5 + ((x / 2) * ITERATION_OFFSET) + 1;
    for (i = 0; i < SQUARE_SIDE - 1; i++)
    {
        // Its on the horizontal axes so we need to give the y
        point.x = point.x = 5 + ((y / 2) * ITERATION_OFFSET) + 1;
        for (j = 0; j < SQUARE_SIDE - 1; j++)
        {
            if (((x == game.player_1.tmp_x_matrix_coordinate && y == game.player_1.tmp_y_matrix_coordinate) || (x == game.player_2.tmp_x_matrix_coordinate && y == game.player_2.tmp_y_matrix_coordinate)) && game.game_status == RUNNING)
                return;
            LCD_SetPoint(point.x, point.y, RGB565CONVERT(0, 0, 0));
            point.x++;
        }
        point.y++;
    }
}

/**
 * @brief Based on the current player turn, we simply show the available moves
 */
void show_available_moves(void)
{
    ui8 i;
    if (game.player_turn == PLAYER_1)
    {
        for (i = 0; i < game.player_1.num_of_possible_moves; i++)
        {
            draw_available_move(game.player_1.possible_moves[i].x, game.player_1.possible_moves[i].y);
        }
    }
    else
    {
        for (i = 0; i < game.player_2.num_of_possible_moves; i++)
        {
            draw_available_move(game.player_2.possible_moves[i].x, game.player_2.possible_moves[i].y);
        }
    }
    return;
}

/**
 * @brief This function deletes all the available moves given an active_player
 */
void delete_available_moves(void)
{
    ui8 i;
    if (game.player_turn == PLAYER_1)
    {
        for (i = 0; i < game.player_1.num_of_possible_moves; i++)
        {
            clear_available_move(game.player_1.possible_moves[i].x, game.player_1.possible_moves[i].y);
        }
    }
    else
    {
        for (i = 0; i < game.player_2.num_of_possible_moves; i++)
        {
            clear_available_move(game.player_2.possible_moves[i].x, game.player_2.possible_moves[i].y);
        }
    }
    return;
}

/**
 * @brief Given a value counter, it writes it inside the remaining seconds area
 * @param counter
 */
void ticks_counter_update(ui8 counter)
{
    char buffer[3] = {0};
    if (counter >= 10)
    {
        sprintf(buffer, "%ds", counter);
        GUI_Text(TICKS_NUMBER_X, TICKS_NUMBER_Y, (uint8_t *)buffer, Green, Black);
    }
    else if (counter >= 5)
    {
        sprintf(buffer, "0%ds", counter);
        GUI_Text(TICKS_NUMBER_X, TICKS_NUMBER_Y, (uint8_t *)buffer, Yellow, Black);
    }
    else
    {
        sprintf(buffer, "0%ds", counter);
        GUI_Text(TICKS_NUMBER_X, TICKS_NUMBER_Y, (uint8_t *)buffer, Red, Black);
    }
}

/**
 * @brief Given the number of residual walls for the player_1, it updates the number
 * in the data area in the lower part of the screen
 * @param walls_residual_number
 */
void p1_walls_update(ui8 walls_residual_number)
{
    char buffer[1] = {0};
    sprintf(buffer, "%d", walls_residual_number);
    GUI_Text(P1_WALLS_NUMBER_X, P1_WALLS_NUMBER_Y, (uint8_t *)buffer, White, Black);
}

/**
 * @brief Given the number of residual walls for the player_2, it updates the number
 * in the data area in the lower part of the screen
 * @param walls_residual_number
 */
void p2_walls_update(ui8 walls_residual_number)
{
    char buffer[1] = {0};
    sprintf(buffer, "%d", walls_residual_number);
    GUI_Text(P2_WALLS_NUMBER_X, P2_WALLS_NUMBER_Y, (uint8_t *)buffer, Red, Black);
}

/**
 * @brief This function clears the text_area below the board and before the data area
 */
void clear_text_area(void)
{
    int i;
    // 32 because we have at most two lines of text
    for (i = 0; i < 32; i++)
    {
        LCD_DrawLine(0, Y_MESSAGE_AREA + i, 240, Y_MESSAGE_AREA + i, Black);
    }
}

/**
 * @brief Given a player, this function shows on the screen the winner message
 * @param winner
 */
void show_winner_message(player winner)
{
    char winner_string[13] = {0};
    if (winner.player_color == WHITE)
    {
        sprintf(winner_string, "Player 1 won!");
        GUI_Text(X_MESSAGE_AREA, Y_MESSAGE_AREA, (uint8_t *)winner_string, White, Black);
    }
    else
    {
        sprintf(winner_string, "Player 2 won!");
        GUI_Text(X_MESSAGE_AREA, Y_MESSAGE_AREA, (uint8_t *)winner_string, Red, Black);
    }
}

/********************************* BELOW HERE IT'S ALL RADIOACTIVE **************************/

// Se cancellando finisco per cancellare un muro già tracciato lo devo ridisegnare -> uso middle per questo
void draw_walls(void)
{
    // matrix point?
    screen_point nearest_playable_cell;
    screen_point screen_coordinates;

    /**
     * Il codice che segue disegna current_wall qualsiasi sia il suo orientamento e anche dopo ogni spostamento
     */

    // Arrivo alla cella, io voglio disegnare solo top
    // Se orizzontale sta sotto la cella a cui arrivo, se verticale sta a destra
    if (game.current_wall.top.x == game.current_wall.bottom.x)
    {
        // è orizzontale, dobbiamo disegnare sotto
        nearest_playable_cell.x = game.current_wall.top.x - 1;
        // la y non deve cambiare
        nearest_playable_cell.y = game.current_wall.top.y;
        // Ora ho le coordinate della cella che uso come reference, ottieniamo i pixel
        screen_coordinates.x = 5 + ((nearest_playable_cell.y / 2) * ITERATION_OFFSET);
        screen_coordinates.y = 5 + ((nearest_playable_cell.x / 2) * ITERATION_OFFSET) + 1;
        // Ora mi devo spostare verso il basso, aumenta la y -> 3 in teoria ci mette in mezzo
        screen_coordinates.y += SQUARE_SIDE + 2;
        // Possiamo disegnare 3 linee
        // game.player_turn == PLAYER_1 ? (LCD_DrawLine(screen_coordinates.x, screen_coordinates.y,
        //                                              screen_coordinates.x + (SQUARE_SIDE * 2 + WALL_SIZE), screen_coordinates.y, White))
        //                              : (LCD_DrawLine(screen_coordinates.x, screen_coordinates.y,
        //                                              screen_coordinates.x + (SQUARE_SIDE * 2 + WALL_SIZE), screen_coordinates.y, Red));
        // game.player_turn == PLAYER_1 ? (LCD_DrawLine(screen_coordinates.x, screen_coordinates.y + 1,
        //                                              screen_coordinates.x + (SQUARE_SIDE * 2 + WALL_SIZE), screen_coordinates.y + 1, White))
        //                              : (LCD_DrawLine(screen_coordinates.x, screen_coordinates.y + 1,
        //                                              screen_coordinates.x + (SQUARE_SIDE * 2 + WALL_SIZE), screen_coordinates.y + 1, Red));
        // game.player_turn == PLAYER_1 ? (LCD_DrawLine(screen_coordinates.x, screen_coordinates.y + 2,
        //                                              screen_coordinates.x + (SQUARE_SIDE * 2 + WALL_SIZE), screen_coordinates.y + 2, White))
        //                              : (LCD_DrawLine(screen_coordinates.x, screen_coordinates.y + 2,
        //                                              screen_coordinates.x + (SQUARE_SIDE * 2 + WALL_SIZE), screen_coordinates.y + 2, Red));
        LCD_DrawLine(screen_coordinates.x, screen_coordinates.y, screen_coordinates.x + (SQUARE_SIDE * 2 + WALL_SIZE), screen_coordinates.y, Cyan);
        LCD_DrawLine(screen_coordinates.x, screen_coordinates.y + 1, screen_coordinates.x + (SQUARE_SIDE * 2 + WALL_SIZE), screen_coordinates.y + 1, Cyan);
        LCD_DrawLine(screen_coordinates.x, screen_coordinates.y + 2, screen_coordinates.x + (SQUARE_SIDE * 2 + WALL_SIZE), screen_coordinates.y + 2, Cyan);
    }
    else
    {
        // è verticale, dobbiamo disegnare a destra
        nearest_playable_cell.x = game.current_wall.top.x;
        nearest_playable_cell.y = game.current_wall.top.y - 1;
        // Ora ho le coordinate della cella che uso come reference, ottieniamo i pixel
        screen_coordinates.x = 5 + ((nearest_playable_cell.y / 2) * ITERATION_OFFSET) + 1;
        screen_coordinates.y = 5 + ((nearest_playable_cell.x / 2) * ITERATION_OFFSET);
        // Ora mi devo spostare a destra
        screen_coordinates.x += SQUARE_SIDE + 2;
        // game.player_turn == PLAYER_1 ? (LCD_DrawLine(screen_coordinates.x, screen_coordinates.y,
        //                                              screen_coordinates.x, screen_coordinates.y + (SQUARE_SIDE * 2 + WALL_SIZE), White))
        //                              : (LCD_DrawLine(screen_coordinates.x, screen_coordinates.y,
        //                                              screen_coordinates.x, screen_coordinates.y + (SQUARE_SIDE * 2 + WALL_SIZE), Red));
        // game.player_turn == PLAYER_1 ? (LCD_DrawLine(screen_coordinates.x + 1, screen_coordinates.y,
        //                                              screen_coordinates.x + 1, screen_coordinates.y + (SQUARE_SIDE * 2 + WALL_SIZE), White))
        //                              : (LCD_DrawLine(screen_coordinates.x + 1, screen_coordinates.y,
        //                                              screen_coordinates.x + 1, screen_coordinates.y + (SQUARE_SIDE * 2 + WALL_SIZE), Red));
        // game.player_turn == PLAYER_1 ? (LCD_DrawLine(screen_coordinates.x + 2, screen_coordinates.y,
        //                                              screen_coordinates.x + 2, screen_coordinates.y + (SQUARE_SIDE * 2 + WALL_SIZE), White))
        //                              : (LCD_DrawLine(screen_coordinates.x + 2, screen_coordinates.y,
        //                                              screen_coordinates.x + 2, screen_coordinates.y + (SQUARE_SIDE * 2 + WALL_SIZE), Red));
        LCD_DrawLine(screen_coordinates.x, screen_coordinates.y, screen_coordinates.x, screen_coordinates.y + (SQUARE_SIDE * 2 + WALL_SIZE), Cyan);
        LCD_DrawLine(screen_coordinates.x + 1, screen_coordinates.y, screen_coordinates.x + 1, screen_coordinates.y + (SQUARE_SIDE * 2 + WALL_SIZE), Cyan);
        LCD_DrawLine(screen_coordinates.x + 2, screen_coordinates.y, screen_coordinates.x + 2, screen_coordinates.y + (SQUARE_SIDE * 2 + WALL_SIZE), Cyan);
    }
}

void update_wall_drawing(wall_move_type move)
{
    int horizontal;
    screen_point point;
    game.current_wall.top.x == game.current_wall.bottom.x ? (horizontal = 1) : (horizontal = 0);
    if (move == WALL_ROTATION)
    {
        // If we are here the wall has been rotated -> before drawing current_wall we need to be sure we are not overriding things
        if (horizontal)
        {
            // we need to search in a specific direction, both over middle and under middle
            // Over middle
            if (game.board[game.current_wall.middle.x - 1][game.current_wall.middle.y].availability == FREE)
            {
                point.x = game.current_wall.middle.x - 1;
                point.y = game.current_wall.middle.y;
                clear_wall(point, VERTICAL);
            }
            // under middle
            if (game.board[game.current_wall.middle.x + 1][game.current_wall.middle.y].availability == FREE)
            {
                point.x = game.current_wall.middle.x + 1;
                point.y = game.current_wall.middle.y;
                clear_wall(point, VERTICAL);
            }
        }
        else
        {
            // search in the vertical direction
            // checking left
            if (game.board[game.current_wall.middle.x][game.current_wall.middle.y - 1].availability == FREE)
            {
                point.x = game.current_wall.middle.x;
                point.y = game.current_wall.middle.y - 1;
                clear_wall(point, HORIZONTAL);
            }
            // checking right
            if (game.board[game.current_wall.middle.x][game.current_wall.middle.y + 1].availability == FREE)
            {
                point.x = game.current_wall.middle.x;
                point.y = game.current_wall.middle.y + 1;
                clear_wall(point, HORIZONTAL);
            }
        }
        draw_walls();
        return;
    }
    else if (move == WALL_UP)
    {
        if (horizontal)
        {
            // we need to search in a specific direction, we need to check all 3 points
            if (game.board[game.current_wall.top.x + 2][game.current_wall.top.y].availability == FREE)
            {
                point.x = game.current_wall.top.x + 2;
                point.y = game.current_wall.top.y;
                clear_wall(point, HORIZONTAL);
            }
            if (game.board[game.current_wall.middle.x + 2][game.current_wall.middle.y].availability == FREE)
            {
                point.x = game.current_wall.middle.x + 2;
                point.y = game.current_wall.middle.y;
                clear_wall(point, HORIZONTAL);
            }
            if (game.board[game.current_wall.bottom.x + 2][game.current_wall.bottom.y].availability == FREE)
            {
                point.x = game.current_wall.bottom.x + 2;
                point.y = game.current_wall.bottom.y;
                clear_wall(point, HORIZONTAL);
            }
        }
        else
        {
            // search in the vertical direction
            if (game.board[game.current_wall.bottom.x + 2][game.current_wall.top.y].availability == FREE)
            {
                point.x = game.current_wall.bottom.x + 2;
                point.y = game.current_wall.bottom.y;
                clear_wall(point, VERTICAL);
            }
            if (game.board[game.current_wall.bottom.x + 1][game.current_wall.top.y].availability == FREE)
            {
                point.x = game.current_wall.bottom.x + 1;
                point.y = game.current_wall.bottom.y;
                clear_wall(point, VERTICAL);
            }
        }
        draw_walls();
        return;
    }
    else if (move == WALL_DOWN)
    {
        if (horizontal)
        {
            // we need to search in a specific direction
            if (game.board[game.current_wall.top.x - 2][game.current_wall.top.y].availability == FREE)
            {
                point.x = game.current_wall.top.x - 2;
                point.y = game.current_wall.top.y;
                clear_wall(point, HORIZONTAL);
            }
            if (game.board[game.current_wall.middle.x - 2][game.current_wall.middle.y].availability == FREE)
            {
                point.x = game.current_wall.middle.x - 2;
                point.y = game.current_wall.middle.y;
                clear_wall(point, HORIZONTAL);
            }
            if (game.board[game.current_wall.bottom.x - 2][game.current_wall.bottom.y].availability == FREE)
            {
                point.x = game.current_wall.bottom.x - 2;
                point.y = game.current_wall.bottom.y;
                clear_wall(point, HORIZONTAL);
            }
        }
        else
        {
            // search in the vertical direction
            if (game.board[game.current_wall.top.x - 2][game.current_wall.top.y].availability == FREE)
            {
                point.x = game.current_wall.top.x - 2;
                point.y = game.current_wall.top.y;
                clear_wall(point, VERTICAL);
            }
            if (game.board[game.current_wall.top.x - 1][game.current_wall.top.y].availability == FREE)
            {
                point.x = game.current_wall.top.x - 1;
                point.y = game.current_wall.top.y;
                clear_wall(point, VERTICAL);
            }
        }
        draw_walls();
        return;
    }
    else if (move == WALL_LEFT)
    {
        if (horizontal)
        {
            // we need to search in a specific direction
            if (game.board[game.current_wall.bottom.x][game.current_wall.bottom.y + 1].availability == FREE)
            {
                point.x = game.current_wall.bottom.x;
                point.y = game.current_wall.bottom.y + 1;
                clear_wall(point, HORIZONTAL);
            }
            if (game.board[game.current_wall.bottom.x][game.current_wall.bottom.y + 2].availability == FREE)
            {
                point.x = game.current_wall.bottom.x;
                point.y = game.current_wall.bottom.y + 2;
                clear_wall(point, HORIZONTAL);
            }
        }
        else
        {
            // search in the vertical direction
            if (game.board[game.current_wall.top.x][game.current_wall.top.y + 2].availability == FREE)
            {
                point.x = game.current_wall.top.x;
                point.y = game.current_wall.top.y + 2;
                clear_wall(point, VERTICAL);
            }
            if (game.board[game.current_wall.middle.x][game.current_wall.middle.y + 2].availability == FREE)
            {
                point.x = game.current_wall.middle.x;
                point.y = game.current_wall.middle.y + 2;
                clear_wall(point, VERTICAL);
            }
            if (game.board[game.current_wall.bottom.x][game.current_wall.bottom.y + 2].availability == FREE)
            {
                point.x = game.current_wall.bottom.x;
                point.y = game.current_wall.bottom.y + 2;
                clear_wall(point, VERTICAL);
            }
        }
        draw_walls();
        return;
    }
    else
    {
        if (horizontal)
        {
            // we need to search in a specific direction
            if (game.board[game.current_wall.top.x][game.current_wall.top.y - 2].availability == FREE)
            {
                point.x = game.current_wall.top.x;
                point.y = game.current_wall.top.y - 2;
                clear_wall(point, HORIZONTAL);
            }
            if (game.board[game.current_wall.top.x][game.current_wall.top.y - 1].availability == FREE)
            {
                point.x = game.current_wall.top.x;
                point.y = game.current_wall.top.y - 1;
                clear_wall(point, HORIZONTAL);
            }
        }
        else
        {
            // search in the vertical direction
            if (game.board[game.current_wall.top.x][game.current_wall.top.y - 2].availability == FREE)
            {
                point.x = game.current_wall.top.x;
                point.y = game.current_wall.top.y - 2;
                clear_wall(point, VERTICAL);
            }
            if (game.board[game.current_wall.middle.x][game.current_wall.middle.y - 2].availability == FREE)
            {
                point.x = game.current_wall.middle.x;
                point.y = game.current_wall.middle.y - 2;
                clear_wall(point, VERTICAL);
            }
            if (game.board[game.current_wall.bottom.x][game.current_wall.bottom.y - 2].availability == FREE)
            {
                point.x = game.current_wall.bottom.x;
                point.y = game.current_wall.bottom.y - 2;
                clear_wall(point, VERTICAL);
            }
        }
        draw_walls();
        return;
    }
}

void clear_wall(screen_point point_to_clear, wall_orientation wall_orientation)
{
    screen_point nearest_playable_cell;
    screen_point screen_coordinates;

    // We need to understand if we need to clear also the mid cell
    if ((point_to_clear.x % 2 != 0) && (point_to_clear.y % 2 != 0))
    {
        // we have to clear a middle cell
        // whatever is the orientation, we choose the top left cell as a reference
        nearest_playable_cell.x = point_to_clear.x - 1;
        nearest_playable_cell.y = point_to_clear.y - 1;
        screen_coordinates.x = 5 + ((nearest_playable_cell.y / 2) * ITERATION_OFFSET) + 1;
        screen_coordinates.y = 5 + ((nearest_playable_cell.x / 2) * ITERATION_OFFSET);
        screen_coordinates.x += SQUARE_SIDE;
        // we are in the middle right from the starting point, going down one SQUARE_SIDE
        screen_coordinates.y += SQUARE_SIDE + 2;
        LCD_DrawLine(screen_coordinates.x, screen_coordinates.y, screen_coordinates.x, screen_coordinates.y + 6, Black);
        LCD_DrawLine(screen_coordinates.x + 1, screen_coordinates.y, screen_coordinates.x + 1, screen_coordinates.y + 6, Black);
        LCD_DrawLine(screen_coordinates.x + 2, screen_coordinates.y, screen_coordinates.x + 2, screen_coordinates.y + 6, Black);
        LCD_DrawLine(screen_coordinates.x + 3, screen_coordinates.y, screen_coordinates.x + 3, screen_coordinates.y + 6, Black);
        LCD_DrawLine(screen_coordinates.x + 4, screen_coordinates.y, screen_coordinates.x + 4, screen_coordinates.y + 6, Black);
        LCD_DrawLine(screen_coordinates.x + 5, screen_coordinates.y, screen_coordinates.x + 5, screen_coordinates.y + 6, Black);
        LCD_DrawLine(screen_coordinates.x + 6, screen_coordinates.y, screen_coordinates.x + 6, screen_coordinates.y + 6, Black);
    }
    else
    {
        if (wall_orientation == VERTICAL)
        {
            // we need to clear right
            // è verticale, dobbiamo disegnare a destra
            nearest_playable_cell.x = point_to_clear.x;
            nearest_playable_cell.y = point_to_clear.y - 1;
            // Ora ho le coordinate della cella che uso come reference, ottieniamo i pixel
            screen_coordinates.x = 5 + ((nearest_playable_cell.y / 2) * ITERATION_OFFSET) + 1;
            screen_coordinates.y = 5 + ((nearest_playable_cell.x / 2) * ITERATION_OFFSET);
            // Ora mi devo spostare a destra
            screen_coordinates.x += SQUARE_SIDE + 2;
            // what i'm doing below is good if the middle cell is occupied, but what if is not?
            LCD_DrawLine(screen_coordinates.x, screen_coordinates.y - 2, screen_coordinates.x, screen_coordinates.y + (SQUARE_SIDE + 2), Black);
            LCD_DrawLine(screen_coordinates.x + 1, screen_coordinates.y - 2, screen_coordinates.x + 1, screen_coordinates.y + (SQUARE_SIDE + 2), Black);
            LCD_DrawLine(screen_coordinates.x + 2, screen_coordinates.y - 2, screen_coordinates.x + 2, screen_coordinates.y + (SQUARE_SIDE + 2), Black);
        }
        else
        {
            // we need to clear under
            nearest_playable_cell.x = point_to_clear.x - 1;
            // la y non deve cambiare
            nearest_playable_cell.y = point_to_clear.y;
            // Ora ho le coordinate della cella che uso come reference, ottieniamo i pixel
            screen_coordinates.x = 5 + ((nearest_playable_cell.y / 2) * ITERATION_OFFSET);
            screen_coordinates.y = 5 + ((nearest_playable_cell.x / 2) * ITERATION_OFFSET) + 1;
            // Ora mi devo spostare verso il basso, aumenta la y -> 3 in teoria ci mette in mezzo
            screen_coordinates.y += SQUARE_SIDE + 2;
            // Possiamo disegnare 3 linee
            LCD_DrawLine(screen_coordinates.x - 2, screen_coordinates.y, screen_coordinates.x + (SQUARE_SIDE + 2), screen_coordinates.y, Black);
            LCD_DrawLine(screen_coordinates.x - 2, screen_coordinates.y + 1, screen_coordinates.x + (SQUARE_SIDE + 2), screen_coordinates.y + 1, Black);
            LCD_DrawLine(screen_coordinates.x - 2, screen_coordinates.y + 2, screen_coordinates.x + (SQUARE_SIDE + 2), screen_coordinates.y + 2, Black);
        }
    }
}
