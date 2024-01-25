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
            // if (((x == game.player_1.tmp_x_matrix_coordinate && y == game.player_1.tmp_y_matrix_coordinate) || (x == game.player_2.tmp_x_matrix_coordinate && y == game.player_2.tmp_y_matrix_coordinate)) && game.game_status == RUNNING)
            //     return;
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

/**
 * @brief This function simply draws the current wall in whatever position.
 * 1. understand the rotation and find the nearest_playable_cell
 * 2. draw the wall
 */
void draw_current_wall(void)
{
    screen_point coordinates = get_current_wall_screen_coordinates();
    // 4 drawline
    if (game.current_wall.wall_orientation == VERTICAL)
    {
        LCD_DrawLine(coordinates.x, coordinates.y, coordinates.x, coordinates.y + WALL_LENGTH, Cyan);
        LCD_DrawLine(coordinates.x + 1, coordinates.y, coordinates.x + 1, coordinates.y + WALL_LENGTH, Cyan);
        LCD_DrawLine(coordinates.x + 2, coordinates.y, coordinates.x + 2, coordinates.y + WALL_LENGTH, Cyan);
        // LCD_DrawLine(coordinates.x + 3, coordinates.y, coordinates.x + 3, coordinates.y + WALL_LENGTH, Cyan);
    }
    else
    {
        LCD_DrawLine(coordinates.x, coordinates.y, coordinates.x + WALL_LENGTH, coordinates.y, Cyan);
        LCD_DrawLine(coordinates.x, coordinates.y + 1, coordinates.x + WALL_LENGTH, coordinates.y + 1, Cyan);
        LCD_DrawLine(coordinates.x, coordinates.y + 2, coordinates.x + WALL_LENGTH, coordinates.y + 2, Cyan);
        // LCD_DrawLine(coordinates.x, coordinates.y + 3, coordinates.x + WALL_LENGTH, coordinates.y + 3, Cyan);
    }
}

/**
 * @brief Get the current wall screen coordinates.
 * We always return the top coordinates
 * @return screen_point
 */
screen_point get_current_wall_screen_coordinates(void)
{
    screen_point starting_coordinates;
    if (game.current_wall.wall_orientation == VERTICAL)
    {
        starting_coordinates.x = VERTICAL_WALL_X_OFFSET(game.current_wall.top.y);
        starting_coordinates.y = VERTICAL_WALL_Y_OFFSET(game.current_wall.top.x);
    }
    else
    {
        // the wall is horizontal, the y has to increase of the offset
        starting_coordinates.x = HORIZONTAL_WALL_X_OFFSET(game.current_wall.top.y);
        starting_coordinates.y = HORIZONTAL_WALL_Y_OFFSET(game.current_wall.top.x);
    }
    return starting_coordinates;
}

/**
 * @brief This function deletes the current wall considering what is already present in the board
 */
void delete_current_wall(void)
{
    screen_point starting_coordinates = get_current_wall_screen_coordinates();
    if (game.current_wall.wall_orientation == VERTICAL)
    {
        // we need to draw in the vertical direction
        // top
        if (game.board[game.current_wall.top.x][game.current_wall.top.y].availability == FREE)
        {
            // cancello quella cella
            LCD_DrawLine(starting_coordinates.x, starting_coordinates.y, starting_coordinates.x, starting_coordinates.y + SQUARE_SIDE, Black);
            LCD_DrawLine(starting_coordinates.x + 1, starting_coordinates.y, starting_coordinates.x + 1, starting_coordinates.y + SQUARE_SIDE, Black);
            LCD_DrawLine(starting_coordinates.x + 2, starting_coordinates.y, starting_coordinates.x + 2, starting_coordinates.y + SQUARE_SIDE, Black);
        }
        // middle
        // we need to update the y
        // TODO: maybe + 1?
        starting_coordinates.y += SQUARE_SIDE + 1;
        if (game.board[game.current_wall.middle.x][game.current_wall.middle.y].availability == FREE)
        {
            // cancello tutta la middle
            LCD_DrawLine(starting_coordinates.x, starting_coordinates.y, starting_coordinates.x, starting_coordinates.y + WALL_SIZE - 1, Black);
            LCD_DrawLine(starting_coordinates.x + 1, starting_coordinates.y, starting_coordinates.x + 1, starting_coordinates.y + WALL_SIZE - 1, Black);
            LCD_DrawLine(starting_coordinates.x + 2, starting_coordinates.y, starting_coordinates.x + 2, starting_coordinates.y + WALL_SIZE - 1, Black);
        }
        if (game.board[game.current_wall.top.x][game.current_wall.top.y].availability == FREE && game.board[game.current_wall.middle.x][game.current_wall.middle.y].availability == OCCUPIED)
        {
            // if the top was free and the middle is occupied, there is a vertical wall
            // la middle è occupata, devo lasciare il quadrato -> 6 drawline, devo cancellare solo due pixel
            LCD_DrawLine(starting_coordinates.x, starting_coordinates.y, starting_coordinates.x, starting_coordinates.y + 1, Black);
            LCD_DrawLine(starting_coordinates.x + 1, starting_coordinates.y, starting_coordinates.x + 1, starting_coordinates.y + 1, Black);
            LCD_DrawLine(starting_coordinates.x + 2, starting_coordinates.y, starting_coordinates.x + 2, starting_coordinates.y + 1, Black);
            // starting_coordinates.y += 5;
            LCD_DrawLine(starting_coordinates.x, starting_coordinates.y + 5, starting_coordinates.x, starting_coordinates.y + 6, Black);
            LCD_DrawLine(starting_coordinates.x + 1, starting_coordinates.y + 5, starting_coordinates.x + 1, starting_coordinates.y + 6, Black);
            LCD_DrawLine(starting_coordinates.x + 2, starting_coordinates.y + 5, starting_coordinates.x + 2, starting_coordinates.y + 6, Black);
            // starting_coordinates.y += 2;
        }
        // bottom
        // TODO: maybe + 1?
        starting_coordinates.y += WALL_SIZE - 1;
        if (game.board[game.current_wall.bottom.x][game.current_wall.bottom.y].availability == FREE)
        {
            // cancello la bottom
            LCD_DrawLine(starting_coordinates.x, starting_coordinates.y, starting_coordinates.x, starting_coordinates.y + SQUARE_SIDE, Black);
            LCD_DrawLine(starting_coordinates.x + 1, starting_coordinates.y, starting_coordinates.x + 1, starting_coordinates.y + SQUARE_SIDE, Black);
            LCD_DrawLine(starting_coordinates.x + 2, starting_coordinates.y, starting_coordinates.x + 2, starting_coordinates.y + SQUARE_SIDE, Black);
        }
    }
    else // il caso orizzontale è perfetto, copialo al verticale
    {
        // the wall is horizontal, we need to draw in the horizontal direction
        if (game.board[game.current_wall.top.x][game.current_wall.top.y].availability == FREE)
        {
            // cancello quella cella
            LCD_DrawLine(starting_coordinates.x, starting_coordinates.y, starting_coordinates.x + SQUARE_SIDE, starting_coordinates.y, Black);
            LCD_DrawLine(starting_coordinates.x, starting_coordinates.y + 1, starting_coordinates.x + SQUARE_SIDE, starting_coordinates.y + 1, Black);
            LCD_DrawLine(starting_coordinates.x, starting_coordinates.y + 2, starting_coordinates.x + SQUARE_SIDE, starting_coordinates.y + 2, Black);
            // we need to update the x
        }
        // middle
        // TODO: maybe + 1?
        starting_coordinates.x += SQUARE_SIDE + 1;
        if (game.board[game.current_wall.middle.x][game.current_wall.middle.y].availability == FREE)
        {
            // cancello tutta la middle
            LCD_DrawLine(starting_coordinates.x, starting_coordinates.y, starting_coordinates.x + WALL_SIZE - 1, starting_coordinates.y, Black);
            LCD_DrawLine(starting_coordinates.x, starting_coordinates.y + 1, starting_coordinates.x + WALL_SIZE - 1, starting_coordinates.y + 1, Black);
            LCD_DrawLine(starting_coordinates.x, starting_coordinates.y + 2, starting_coordinates.x + WALL_SIZE - 1, starting_coordinates.y + 2, Black);
        }
        if (game.board[game.current_wall.top.x][game.current_wall.top.y].availability == FREE && game.board[game.current_wall.middle.x][game.current_wall.middle.y].availability == OCCUPIED)
        {
            // la middle è occupata, devo lasciare il quadrato -> 6 drawline, devo cancellare solo due pixel
            LCD_DrawLine(starting_coordinates.x, starting_coordinates.y, starting_coordinates.x + 1, starting_coordinates.y, Black);
            LCD_DrawLine(starting_coordinates.x, starting_coordinates.y + 1, starting_coordinates.x + 1, starting_coordinates.y + 1, Black);
            LCD_DrawLine(starting_coordinates.x, starting_coordinates.y + 2, starting_coordinates.x + 1, starting_coordinates.y + 2, Black);
            // starting_coordinates.x += 5;
            LCD_DrawLine(starting_coordinates.x + 5, starting_coordinates.y, starting_coordinates.x + 6, starting_coordinates.y, Black);
            LCD_DrawLine(starting_coordinates.x + 5, starting_coordinates.y + 1, starting_coordinates.x + 6, starting_coordinates.y + 1, Black);
            LCD_DrawLine(starting_coordinates.x + 5, starting_coordinates.y + 2, starting_coordinates.x + 6, starting_coordinates.y + 2, Black);
            // starting_coordinates.x += 2;
        }
        // bottom
        // TODO: maybe + 1?
        starting_coordinates.x += WALL_SIZE - 1;
        if (game.board[game.current_wall.bottom.x][game.current_wall.bottom.y].availability == FREE)
        {
            // cancello la bottom
            LCD_DrawLine(starting_coordinates.x, starting_coordinates.y, starting_coordinates.x + SQUARE_SIDE, starting_coordinates.y, Black);
            LCD_DrawLine(starting_coordinates.x, starting_coordinates.y + 1, starting_coordinates.x + SQUARE_SIDE, starting_coordinates.y + 1, Black);
            LCD_DrawLine(starting_coordinates.x, starting_coordinates.y + 2, starting_coordinates.x + SQUARE_SIDE, starting_coordinates.y + 2, Black);
        }
    }
}

void p1_no_more_walls(void)
{
    GUI_Text(X_MESSAGE_AREA, Y_MESSAGE_AREA, "No walls available, move the", White, Black);
    GUI_Text(X_MESSAGE_AREA, Y_MESSAGE_AREA + 16, "token!", White, Black);
    game.text_area_status = FULL;
    return;
}

void p2_no_more_walls(void)
{
    GUI_Text(X_MESSAGE_AREA, Y_MESSAGE_AREA, "No walls available, move the", Red, Black);
    GUI_Text(X_MESSAGE_AREA, Y_MESSAGE_AREA + 16, "token!", Red, Black);
    game.text_area_status = FULL;
    return;
}

void p1_illegal_wall(void)
{
    GUI_Text(X_MESSAGE_AREA, Y_MESSAGE_AREA, (uint8_t *)"Cannot place wall here!", White, Black);
    game.text_area_status = FULL;
    return;
}

void p2_illegal_wall(void)
{
    GUI_Text(X_MESSAGE_AREA, Y_MESSAGE_AREA, (uint8_t *)"Cannot place wall here!", Red, Black);
    game.text_area_status = FULL;
    return;
}
