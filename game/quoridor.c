/**
 * @file quoridor.c
 * @author your name (you@domain.com)
 * @brief inside this file there is all the game logic
 * @version 0.1
 * @date 2024-01-10
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "quoridor.h"
#include "../timer/timer.h"
#include "lpc17xx.h"
#include "../graphics/graphics.h"
#include "../GLCD/GLCD.h"
#include "./player.h"
/**
 * @brief This function initializes the board with the cell type and the
 * players position and color
 * @param game
 */
void game_init(game_data *game)
{
    // Setting all the cells as free
    board_cell_type_init(game);
    // Drawing the game board
    draw_board();
    // Initializing player data
    init_players(game);
    /**
     * This timer is the timer that will count the game time.
     * It's setted to 1s (25MHz*1sec).
     * The timer needs to reset and generate ad interrupt upon count, so SRI -> 011 = 3
     */
    init_timer(0, 0, 0, 3, 0x17D7840);
    // At this point we are in wait mode
}

/**
 * @brief This function is responsible of starting the game. It's called
 * by the INT0 key interrupt
 * @param game
 */
void game_start(game_data *game)
{
    // If the game is already running we don't have to do anything
    if (game->game_status != WAIT_MODE)
        return;
    draw_player_token(game->player_1.x_matrix_coordinate, game->player_1.y_matrix_coordinate, PLAYER_1);
    draw_player_token(game->player_2.x_matrix_coordinate, game->player_2.y_matrix_coordinate, PLAYER_2);
    turn_manager(game);
}

void game_over(game_data *game, player winner)
{
    disable_timer(0);
    game->game_status = OVER;
    show_winner_message(winner);
}

/**
 * @brief This function initializes each cell of the board setting the right
 * cell type depending from the position
 *
 * @param game
 */
void board_cell_type_init(game_data *game)
{
    int i = 0;
    int j = 0;
    // Initializing the board with the correct cell type
    for (i = 0; i < 13; i++)
    {
        for (j = 0; j < 13; j++)
        {
            // first of all we need to know if we are looking at a row with also
            // board cells or only walls
            if (i % 2 == 0)
            {
                // if we are here it means that this will be an "hybrid row"
                // a board cell is always at a even index
                if (j % 2 == 0)
                {
                    // We are looking at a board cell
                    game->board[i][j].cell = BOARD_CELL;
                    game->board[i][j].availability = FREE;
                }
                else
                {
                    // We are looking at a wall cell
                    game->board[i][j].cell = WALL_CELL;
                    game->board[i][j].availability = FREE;
                }
            }
            else
            {
                // If we are here its a only walls row
                game->board[i][j].cell = WALL_CELL;
                game->board[i][j].availability = FREE;
            }
        }
    }
}

/**
 * @brief This function is called by the RIT handler when a joystick input
 * is received.
 * The only check previously made about the incoming data is that the game must be running.
 * We need to check if the move is legal or not and in case modify the tmp x/y coordinates.
 * The final modification will be done inside the select button handler.
 * Remember that:
 * - when going UP the x is decreasing
 * - when going DOWN the x is increasing
 * - when going LEFT the y is decreasing
 * - when going RIGHT the y is increasing
 * - when moving, we need to skip the wall cell (so +/- 2)
 * The conditions that make a move illegal are:
 * - going out of the board
 * - we need to cross a wall cell
 * - in the landing cell there is the opponent
 * @param move
 */
void move_player(move_type move, game_data *game)
{
    int x;
    int y;
    uint8_t old_tmp_x;
    uint8_t old_tmp_y;
    if (game->player_turn == PLAYER_1)
    {
        x = game->player_1.x_matrix_coordinate;
        y = game->player_1.y_matrix_coordinate;
        old_tmp_x = game->player_1.tmp_x_matrix_coordinate;
        old_tmp_y = game->player_1.tmp_y_matrix_coordinate;
        game->player_1.tmp_x_matrix_coordinate = x;
        game->player_1.tmp_y_matrix_coordinate = y;
    }
    else
    {
        x = game->player_2.x_matrix_coordinate;
        y = game->player_2.y_matrix_coordinate;
        old_tmp_x = game->player_2.tmp_x_matrix_coordinate;
        old_tmp_y = game->player_2.tmp_y_matrix_coordinate;
        game->player_2.tmp_x_matrix_coordinate = x;
        game->player_2.tmp_y_matrix_coordinate = y;
    }

    // checking if the move is legal or not
    if (move == UP)
    {
        if (x - 2 < 0 || game->board[x - 1][y].availability == OCCUPIED)
        {
            show_available_moves(game);
            game->player_turn == PLAYER_1 ? (update_player_token_pos(old_tmp_x, old_tmp_y, game->player_1.tmp_x_matrix_coordinate, game->player_1.tmp_y_matrix_coordinate, PLAYER_1))
                                          : (update_player_token_pos(old_tmp_x, old_tmp_y, game->player_2.tmp_x_matrix_coordinate, game->player_2.tmp_y_matrix_coordinate, PLAYER_2));
            goto illegal_move;
        }
        // We are not over, we still need to check for the presence of the opponent
        if (game->board[x - 2][y].availability == OCCUPIED)
        {
            // The opponent is facing us, we need to check if we can jump him
            // Two cases: there is a wall in between or the landing cell is out of the board
            /**
             * If we enter here the opponent is facing us, we need to check if we can jump him
             * Two things can go wrong:
             * - there is a wall behind him
             * - the landing position is out of the board
             */
            if (x - 4 < 0 || game->board[x - 3][y].availability == OCCUPIED)
                goto illegal_move;
            // If we are still here the opponent is facing us but the landing cell is legal
            game->player_turn == PLAYER_1 ? (game->player_1.tmp_x_matrix_coordinate = x - 4) : (game->player_2.tmp_x_matrix_coordinate = x - 4);
            // updating the Interface
            game->player_turn == PLAYER_1 ? (update_player_token_pos(old_tmp_x, old_tmp_y, game->player_1.tmp_x_matrix_coordinate, game->player_1.tmp_y_matrix_coordinate, PLAYER_1))
                                          : (update_player_token_pos(old_tmp_x, old_tmp_y, game->player_2.tmp_x_matrix_coordinate, game->player_2.tmp_y_matrix_coordinate, PLAYER_2));
            // Before moving we need to clear the starting cell
            // game->player_turn == PLAYER_1 ? (clear_moves(game->player_1.x_matrix_coordinate, game->player_1.y_matrix_coordinate)) : (clear_moves(game->player_2.x_matrix_coordinate, game->player_2.y_matrix_coordinate));
            return;
        }
        // If we are here the move is legal and the opponent is not facing us
        game->player_turn == PLAYER_1 ? (game->player_1.tmp_x_matrix_coordinate = x - 2) : (game->player_2.tmp_x_matrix_coordinate = x - 2);
        // updating the Interface
        game->player_turn == PLAYER_1 ? (update_player_token_pos(old_tmp_x, old_tmp_y, game->player_1.tmp_x_matrix_coordinate, game->player_1.tmp_y_matrix_coordinate, PLAYER_1))
                                      : (update_player_token_pos(old_tmp_x, old_tmp_y, game->player_2.tmp_x_matrix_coordinate, game->player_2.tmp_y_matrix_coordinate, PLAYER_2));
        // Before moving we need to clear the starting cell
        // game->player_turn == PLAYER_1 ? (clear_moves(game->player_1.x_matrix_coordinate, game->player_1.y_matrix_coordinate)) : (clear_moves(game->player_2.x_matrix_coordinate, game->player_2.y_matrix_coordinate));
    }
    else if (move == DOWN)
    {
        // checking that the move is legal, now we are moving down so increasing X
        if (x + 2 > 12 || game->board[x + 1][y].availability == OCCUPIED)
        {
            show_available_moves(game);
            game->player_turn == PLAYER_1 ? (update_player_token_pos(old_tmp_x, old_tmp_y, game->player_1.tmp_x_matrix_coordinate, game->player_1.tmp_y_matrix_coordinate, PLAYER_1))
                                          : (update_player_token_pos(old_tmp_x, old_tmp_y, game->player_2.tmp_x_matrix_coordinate, game->player_2.tmp_y_matrix_coordinate, PLAYER_2));
            goto illegal_move;
        }
        if (game->board[x + 2][y].availability == OCCUPIED)
        {
            if (x + 4 > 12 || game->board[x + 3][y].availability == OCCUPIED)
                goto illegal_move;
            game->player_turn == PLAYER_1 ? (game->player_1.tmp_x_matrix_coordinate = x + 4) : (game->player_2.tmp_x_matrix_coordinate = x + 4);
            // updating the Interface
            game->player_turn == PLAYER_1 ? (update_player_token_pos(old_tmp_x, old_tmp_y, game->player_1.tmp_x_matrix_coordinate, game->player_1.tmp_y_matrix_coordinate, PLAYER_1))
                                          : (update_player_token_pos(old_tmp_x, old_tmp_y, game->player_2.tmp_x_matrix_coordinate, game->player_2.tmp_y_matrix_coordinate, PLAYER_2));
            // Before moving we need to clear the starting cell
            // game->player_turn == PLAYER_1 ? (clear_moves(game->player_1.x_matrix_coordinate, game->player_1.y_matrix_coordinate)) : (clear_moves(game->player_2.x_matrix_coordinate, game->player_2.y_matrix_coordinate));
            return;
        }
        game->player_turn == PLAYER_1 ? (game->player_1.tmp_x_matrix_coordinate = x + 2) : (game->player_2.tmp_x_matrix_coordinate = x + 2);
        // updating the Interface
        game->player_turn == PLAYER_1 ? (update_player_token_pos(old_tmp_x, old_tmp_y, game->player_1.tmp_x_matrix_coordinate, game->player_1.tmp_y_matrix_coordinate, PLAYER_1))
                                      : (update_player_token_pos(old_tmp_x, old_tmp_y, game->player_2.tmp_x_matrix_coordinate, game->player_2.tmp_y_matrix_coordinate, PLAYER_2));
        // Before moving we need to clear the starting cell
        // game->player_turn == PLAYER_1 ? (clear_moves(game->player_1.x_matrix_coordinate, game->player_1.y_matrix_coordinate)) : (clear_moves(game->player_2.x_matrix_coordinate, game->player_2.y_matrix_coordinate));
    }
    else if (move == LEFT)
    {
        // checking that the move is legal, we are decrementing y
        if (y - 2 < 0 || game->board[x][y - 1].availability == OCCUPIED)
        {
            show_available_moves(game);
            game->player_turn == PLAYER_1 ? (update_player_token_pos(old_tmp_x, old_tmp_y, game->player_1.tmp_x_matrix_coordinate, game->player_1.tmp_y_matrix_coordinate, PLAYER_1))
                                          : (update_player_token_pos(old_tmp_x, old_tmp_y, game->player_2.tmp_x_matrix_coordinate, game->player_2.tmp_y_matrix_coordinate, PLAYER_2));
            goto illegal_move;
        }
        if (game->board[x][y - 2].availability == OCCUPIED)
        {
            if (y - 4 < 0 || game->board[x][y - 3].availability == OCCUPIED)
                goto illegal_move;
            game->player_turn == PLAYER_1 ? (game->player_1.tmp_y_matrix_coordinate = y - 4) : (game->player_2.tmp_y_matrix_coordinate = y - 4);
            // updating the Interface
            game->player_turn == PLAYER_1 ? (update_player_token_pos(old_tmp_x, old_tmp_y, game->player_1.tmp_x_matrix_coordinate, game->player_1.tmp_y_matrix_coordinate, PLAYER_1))
                                          : (update_player_token_pos(old_tmp_x, old_tmp_y, game->player_2.tmp_x_matrix_coordinate, game->player_2.tmp_y_matrix_coordinate, PLAYER_2));
            // Before moving we need to clear the starting cell
            // game->player_turn == PLAYER_1 ? (clear_moves(game->player_1.x_matrix_coordinate, game->player_1.y_matrix_coordinate)) : (clear_moves(game->player_2.x_matrix_coordinate, game->player_2.y_matrix_coordinate));
            return;
        }
        game->player_turn == PLAYER_1 ? (game->player_1.tmp_y_matrix_coordinate = y - 2) : (game->player_2.tmp_y_matrix_coordinate = y - 2);
        // updating the Interface
        game->player_turn == PLAYER_1 ? (update_player_token_pos(old_tmp_x, old_tmp_y, game->player_1.tmp_x_matrix_coordinate, game->player_1.tmp_y_matrix_coordinate, PLAYER_1))
                                      : (update_player_token_pos(old_tmp_x, old_tmp_y, game->player_2.tmp_x_matrix_coordinate, game->player_2.tmp_y_matrix_coordinate, PLAYER_2));
        // Before moving we need to clear the starting cell
        // game->player_turn == PLAYER_1 ? (clear_moves(game->player_1.x_matrix_coordinate, game->player_1.y_matrix_coordinate)) : (clear_moves(game->player_2.x_matrix_coordinate, game->player_2.y_matrix_coordinate));
    }
    else if (move == RIGHT)
    {
        // checking that the move is legal, we are incrementing y
        if (y + 2 > 12 || game->board[x][y + 1].availability == OCCUPIED)
        {
            show_available_moves(game);
            game->player_turn == PLAYER_1 ? (update_player_token_pos(old_tmp_x, old_tmp_y, game->player_1.tmp_x_matrix_coordinate, game->player_1.tmp_y_matrix_coordinate, PLAYER_1))
                                          : (update_player_token_pos(old_tmp_x, old_tmp_y, game->player_2.tmp_x_matrix_coordinate, game->player_2.tmp_y_matrix_coordinate, PLAYER_2));
            goto illegal_move;
        }
        if (game->board[x][y + 2].availability == OCCUPIED)
        {
            if (y + 4 > 12 || game->board[x][y + 3].availability == OCCUPIED)
                goto illegal_move;
            game->player_turn == PLAYER_1 ? (game->player_1.tmp_y_matrix_coordinate = y + 4) : (game->player_2.tmp_y_matrix_coordinate = y + 4);
            // updating the Interface
            game->player_turn == PLAYER_1 ? (update_player_token_pos(old_tmp_x, old_tmp_y, game->player_1.tmp_x_matrix_coordinate, game->player_1.tmp_y_matrix_coordinate, PLAYER_1))
                                          : (update_player_token_pos(old_tmp_x, old_tmp_y, game->player_2.tmp_x_matrix_coordinate, game->player_2.tmp_y_matrix_coordinate, PLAYER_2));
            // Before moving we need to clear the starting cell
            // game->player_turn == PLAYER_1 ? (clear_moves(game->player_1.x_matrix_coordinate, game->player_1.y_matrix_coordinate)) : (clear_moves(game->player_2.x_matrix_coordinate, game->player_2.y_matrix_coordinate));
            return;
        }
        game->player_turn == PLAYER_1 ? (game->player_1.tmp_y_matrix_coordinate = y + 2) : (game->player_2.tmp_y_matrix_coordinate = y + 2);
        // updating the Interface
        game->player_turn == PLAYER_1 ? (update_player_token_pos(old_tmp_x, old_tmp_y, game->player_1.tmp_x_matrix_coordinate, game->player_1.tmp_y_matrix_coordinate, PLAYER_1))
                                      : (update_player_token_pos(old_tmp_x, old_tmp_y, game->player_2.tmp_x_matrix_coordinate, game->player_2.tmp_y_matrix_coordinate, PLAYER_2));
        // Before moving we need to clear the starting cell
        // game->player_turn == PLAYER_1 ? (clear_moves(game->player_1.x_matrix_coordinate, game->player_1.y_matrix_coordinate)) : (clear_moves(game->player_2.x_matrix_coordinate, game->player_2.y_matrix_coordinate));
    }

illegal_move:
    return;
}

/**
 * TODO: be sure that when we change turn we don't have to reset everything
 * We also need to check if it's the last move of the game
 * @param game
 */
void swap_turn(game_data *game, uint8_t select)
{
    matrix_point point;
    if (select)
    {
        //     if (game->player_turn == PLAYER_1 && game->input_mode == PLAYER_MOVEMENT)
        //     {
        //         show_available_moves(game, HIDE);
        //         clear_text_area();
        //         save_last_move(game);
        //         game->board[game->player_1.x_matrix_coordinate][game->player_1.y_matrix_coordinate].availability = FREE;
        //         game->player_1.x_matrix_coordinate = game->player_1.tmp_x_matrix_coordinate;
        //         game->player_1.y_matrix_coordinate = game->player_1.tmp_y_matrix_coordinate;
        //         game->board[game->player_1.x_matrix_coordinate][game->player_1.y_matrix_coordinate].availability = OCCUPIED;
        //         if (game->player_1.x_matrix_coordinate == 12)
        //         {
        //             game_over(game, game->player_1);
        //             return;
        //         }
        //     }
        //     else if (game->player_turn == PLAYER_2 && game->input_mode == PLAYER_MOVEMENT)
        //     {
        //         show_available_moves(game, HIDE);
        //         clear_text_area();
        //         save_last_move(game);
        //         game->board[game->player_2.x_matrix_coordinate][game->player_2.y_matrix_coordinate].availability = FREE;
        //         game->player_2.x_matrix_coordinate = game->player_2.tmp_x_matrix_coordinate;
        //         game->player_2.y_matrix_coordinate = game->player_2.tmp_y_matrix_coordinate;
        //         game->board[game->player_2.x_matrix_coordinate][game->player_2.y_matrix_coordinate].availability = OCCUPIED;
        //         if (game->player_2.x_matrix_coordinate == 0)
        //         {
        //             game_over(game, game->player_2);
        //             return;
        //         }
        //     }
        //     else
        //     {
        //         // we are in the wall mode
        //         // devo cancellare current_wall se non è definitivo
        //         if (game->board[game->current_wall.top.x][game->current_wall.top.y].availability == FREE)
        //         {
        //             point.x = game->current_wall.top.x;
        //             point.y = game->current_wall.top.y;
        //             game->current_wall.top.x == game->current_wall.bottom.x ? (clear_wall(point, HORIZONTAL)) : (clear_wall(point, VERTICAL));
        //         }
        //         if (game->board[game->current_wall.middle.x][game->current_wall.middle.y].availability == FREE)
        //         {
        //             point.x = game->current_wall.middle.x;
        //             point.y = game->current_wall.middle.y;
        //             game->current_wall.top.x == game->current_wall.top.x ? (clear_wall(point, HORIZONTAL)) : (clear_wall(point, VERTICAL));
        //         }
        //         if (game->board[game->current_wall.bottom.x][game->current_wall.bottom.y].availability == FREE)
        //         {
        //             point.x = game->current_wall.bottom.x;
        //             point.y = game->current_wall.bottom.y;
        //             game->current_wall.top.x == game->current_wall.bottom.x ? (clear_wall(point, HORIZONTAL)) : (clear_wall(point, VERTICAL));
        //         }
        //         clear_text_area();
        //         save_last_move(game);
        //     }
        // }
        // else
        // {
        //     if (game->input_mode == PLAYER_MOVEMENT)
        //     {
        //         game->player_turn == PLAYER_1 ? (game->player_1.tmp_x_matrix_coordinate = game->player_1.x_matrix_coordinate) : (game->player_2.tmp_x_matrix_coordinate = game->player_2.x_matrix_coordinate);
        //         game->player_turn == PLAYER_1 ? (game->player_1.tmp_y_matrix_coordinate = game->player_1.y_matrix_coordinate) : (game->player_2.tmp_y_matrix_coordinate = game->player_2.y_matrix_coordinate);
        //         show_available_moves(game, HIDE);
        //         clear_text_area();
        //         save_last_move(game);
        //         game->player_turn == PLAYER_1 ? (update_player_token_pos(game->player_1.x_matrix_coordinate, game->player_1.y_matrix_coordinate, game->player_1.x_matrix_coordinate, game->player_1.y_matrix_coordinate, PLAYER_1))
        //                                       : (update_player_token_pos(game->player_2.x_matrix_coordinate, game->player_2.y_matrix_coordinate, game->player_2.x_matrix_coordinate, game->player_2.y_matrix_coordinate, PLAYER_2));
        //     }
        //     else
        //     {
        //         game->current_wall.top.x == game->current_wall.bottom.x ? (clear_wall(game->current_wall.top, HORIZONTAL)) : (clear_wall(game->current_wall.top, VERTICAL));
        //         game->current_wall.top.x == game->current_wall.bottom.x ? (clear_wall(game->current_wall.middle, HORIZONTAL)) : (clear_wall(game->current_wall.middle, VERTICAL));
        //         game->current_wall.top.x == game->current_wall.bottom.x ? (clear_wall(game->current_wall.bottom, HORIZONTAL)) : (clear_wall(game->current_wall.bottom, VERTICAL));
        //         clear_text_area();
        //         save_last_move(game);
        //     }
    }
    // We also need to clear any residual wall from the screen
    // We need to swap player turn
    game->player_turn == PLAYER_1 ? (game->player_turn = PLAYER_2) : (game->player_turn = PLAYER_1);
    // And to reset the game mode
    game->input_mode = PLAYER_MOVEMENT;
    // And reset the game ticks
    game->game_tick = 20;
    // And reset && enable the timer
    reset_timer(0);
    enable_timer(0);
    // And show the new moves
    show_available_moves(game);
}

/**
 * The structure is the following:
 * - player_id : 8 bits
 * - player_move / wall : 4 bits (0-player, 1-wall)
 * - vertical/horizontal : 4 bits (0 vertical, 1 horizontal)
 * - y player : 8 bits
 * - x player : 8 bits
 * @param game
 */
void save_last_move(game_data *game)
{
    uint8_t tmp;
    tmp = game->player_turn;
    game->player_turn == PLAYER_1 ? (tmp = 0) : (tmp = 1);
    game->last_move |= tmp << 24;
    game->input_mode == PLAYER_MOVEMENT ? (tmp = 0) : (tmp = 1);
    game->last_move |= tmp << 20;
    if (game->input_mode == PLAYER_MOVEMENT)
    {
        tmp = 0;
        game->last_move |= tmp << 16;
    }
    else
    {
        game->current_wall.top.x == game->current_wall.bottom.x ? (tmp = 1) : (tmp = 0);
        game->last_move |= tmp << 16;
    }
    game->player_turn == PLAYER_1 ? (tmp = game->player_1.x_matrix_coordinate) : (tmp = game->player_2.x_matrix_coordinate);
    game->last_move |= tmp << 8;
    game->player_turn == PLAYER_1 ? (tmp = game->player_1.y_matrix_coordinate) : (tmp = game->player_2.y_matrix_coordinate);
    game->last_move |= tmp << 8;
}

/**
 * @brief In here we manage the turns
 *
 * @param game
 */
void turn_manager(game_data *game)
{
    if (game->game_status == WAIT_MODE)
    {
        game->game_status = RUNNING;
        // we need to start the timer
        enable_timer(0);
        // The game starts with player 1 playing, we need to show the possible moves
        find_available_moves(game);
        // TODO: remove the show
        show_available_moves(game);
    }
    else if (game->game_status == RUNNING)
    {
        // we need to swap turns
        // TODO: why do we need the select?
        swap_turn(game, 0);
        return;
    }
    else
    {
        // the game is in progress we shouldn't be here
        return;
    }
}
