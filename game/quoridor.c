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
#include "./walls.h"
#include "../magda/magda.h"
#include "../menu/menu.h"

extern game_data game;

/**
 * @brief This function is responsible of starting the game. It's called
 * by the INT0 key interrupt
 * @param game
 */
void game_start(game_data *game)
{
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
    game->game_status = WAIT_MODE;
    draw_player_token(game->player_1.x_matrix_coordinate, game->player_1.y_matrix_coordinate, PLAYER_1);
    draw_player_token(game->player_2.x_matrix_coordinate, game->player_2.y_matrix_coordinate, PLAYER_2);
    // 0 since the time is not over
    turn_manager(game, 0);
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
 * @brief In here we manage the turns.
 * @param game -> the game instance
 * @param time_over -> a flag, 0 if it's a move, 1 if the time is over
 */
void turn_manager(game_data *game, ui8 time_over)
{
    ui8 success;
    if (game->game_status == WAIT_MODE)
    {
        game->game_status = RUNNING;
        enable_timer(0);
        // The game starts with p1 playing
        p1_turn(game);
    }
    else if (game->game_status == RUNNING)
    {
        // we need to swap turns
        // if (game->text_area_status == FULL)
        //     clear_text_area();

        if (time_over)
        {
            // here we must reset the state, we don't have a confirmation
            if (game->input_mode == PLAYER_MOVEMENT)
            {
                // the player was trying to move itself
                if (game->player_turn == PLAYER_1)
                {
                    // player_1 stuff
                    reset_p1_token();
                    p2_turn(game);
                    return;
                }
                else
                {
                    // player_2 stuff
                    reset_p2_token();
                    p1_turn(game);
                    return;
                }
            }
            else
            {
                // the player was trying to place a wall
                delete_current_wall();
                if (game->player_turn == PLAYER_1)
                {
                    p2_turn(game);
                    return;
                }
                else
                {
                    p1_turn(game);
                    return;
                }
            }
        }
        else
        {
            // we consolidate the move
            if (game->input_mode == PLAYER_MOVEMENT)
            {
                // the player moved itself
                confirm_player_move(game);
                if (game->game_status != OVER)
                {
                    delete_available_moves();
                    if (game->player_turn == PLAYER_1)
                    {
                        draw_player_token(game->player_1.x_matrix_coordinate, game->player_1.y_matrix_coordinate, PLAYER_1);
                        p2_turn(game);
                    }
                    else
                    {
                        draw_player_token(game->player_2.x_matrix_coordinate, game->player_2.y_matrix_coordinate, PLAYER_2);
                        p1_turn(game);
                    }
                }
            }
            else
            {
                success = confirm_wall_placement();
                if (success)
                {
                    // we can swap turn
                    if (game->player_turn == PLAYER_1)
                    {
                        p2_turn(game);
                    }
                    else
                    {
                        p1_turn(game);
                    }
                }
            }
        }
        return;
    }
    else
    {
        // the game is in progress we shouldn't be here
        return;
    }
}

void p1_turn(game_data *game)
{
    game->input_mode = PLAYER_MOVEMENT;
    game->player_turn = PLAYER_1;
    game->game_tick = 20;
    reset_timer(0);
    enable_timer(0);
    find_available_moves(game);
    show_available_moves();
    return;
    return;
}

void p2_turn(game_data *game)
{
    game->input_mode = PLAYER_MOVEMENT;
    game->player_turn = PLAYER_2;
    game->game_tick = 20;
    reset_timer(0);
    enable_timer(0);
    find_available_moves(game);
    show_available_moves();
    return;
}

/**
 * @brief Remember that the bot doesn't show anything, he just moves
 * @param game
 */
void p1_bot_turn(game_data *game)
{
    minimax_res move;
    game->input_mode = PLAYER_MOVEMENT;
    game->player_turn = PLAYER_1;
    game->game_tick = 20;
    reset_timer(0);
    enable_timer(0);
    move = minimax(1, 1);
    if (move.wall_orientation == NONE)
    {
        // player movement
        delete_player_token(game->player_1.x_matrix_coordinate, game->player_1.y_matrix_coordinate);
        game->player_1.tmp_x_matrix_coordinate = move.x;
        game->player_1.tmp_y_matrix_coordinate = move.y;
        confirm_player_move(game);
        draw_player_token(game->player_1.x_matrix_coordinate, game->player_1.y_matrix_coordinate, PLAYER_1);
    }
    else
    {
        // wall placement
    }
    p2_turn(game);
    return;
}

void reset_p1_token(void)
{
    delete_available_moves();
    delete_player_token(game.player_1.tmp_x_matrix_coordinate, game.player_1.tmp_y_matrix_coordinate);
    draw_player_token(game.player_1.x_matrix_coordinate, game.player_1.y_matrix_coordinate, PLAYER_1);
}

void reset_p2_token(void)
{
    delete_available_moves();
    delete_player_token(game.player_2.tmp_x_matrix_coordinate, game.player_2.tmp_y_matrix_coordinate);
    draw_player_token(game.player_2.x_matrix_coordinate, game.player_2.y_matrix_coordinate, PLAYER_2);
}

void confirm_player_move(game_data *game)
{
    if (game->player_turn == PLAYER_1)
    {
        // we need to confirm the player_1 move
        game->board[game->player_1.x_matrix_coordinate][game->player_1.y_matrix_coordinate].availability = FREE;
        game->player_1.x_matrix_coordinate = game->player_1.tmp_x_matrix_coordinate;
        game->player_1.y_matrix_coordinate = game->player_1.tmp_y_matrix_coordinate;
        game->board[game->player_1.x_matrix_coordinate][game->player_1.y_matrix_coordinate].availability = OCCUPIED;
        if (game->player_1.x_matrix_coordinate == 12)
        {
            game_over(game, game->player_1);
            return;
        }
    }
    else
    {
        game->board[game->player_2.x_matrix_coordinate][game->player_2.y_matrix_coordinate].availability = FREE;
        game->player_2.x_matrix_coordinate = game->player_2.tmp_x_matrix_coordinate;
        game->player_2.y_matrix_coordinate = game->player_2.tmp_y_matrix_coordinate;
        game->board[game->player_2.x_matrix_coordinate][game->player_2.y_matrix_coordinate].availability = OCCUPIED;
        if (game->player_2.x_matrix_coordinate == 0)
        {
            game_over(game, game->player_2);
            return;
        }
    }
}

void move_dispatcher(move_type direction, game_data *game)
{
    if (game->game_status == MENU_MODE)
    {
        menu_manager(direction);
        return;
    }

    if (game->game_status == RUNNING)
    {
        if (game->input_mode == PLAYER_MOVEMENT)
        {
            if (direction == UP)
            {
                move_player(UP, game, game->player_turn);
            }
            else if (direction == LEFT)
            {
                move_player(LEFT, game, game->player_turn);
            }
            else if (direction == RIGHT)
            {
                move_player(RIGHT, game, game->player_turn);
            }
            else if (direction == DOWN)
            {
                move_player(DOWN, game, game->player_turn);
            }
        }
        else
        {
            // Wall mode
            if (game->text_area_status == FULL)
                clear_text_area();
            if (direction == UP)
            {
                move_wall(WALL_UP);
            }
            else if (direction == LEFT)
            {
                move_wall(WALL_LEFT);
            }
            else if (direction == RIGHT)
            {
                move_wall(WALL_RIGHT);
            }
            else if (direction == DOWN)
            {
                // down
                move_wall(WALL_DOWN);
            }
            else
            {
                // rotation
                move_wall(WALL_ROTATION);
            }
        }
    }
}

/**
 * @brief We need to distinguish between wall mode and player mode and simply
 * call the function that handles it
 */
void select_button_pressed()
{
    if (game.game_status == MENU_MODE)
    {
        menu_manager(ROTATION);
        return;
    }
    turn_manager(&game, 0);
}

void key1_button_pressed()
{
    if (game.input_mode == PLAYER_MOVEMENT)
    {
        // call an appropriate function, we need to enter in wall mode
        wall_mode_enter();
        return;
    }
    if (game.input_mode == WALL_MODE)
    {
        if (game.text_area_status == FULL)
            clear_text_area();
        wall_mode_exit();
        return;
    }
}

void key2_button_pressed()
{
    if (game.input_mode == WALL_MODE)
    {
        // it's a wall rotation
        move_wall(WALL_ROTATION);
    }
}

/**
 * @brief We need to draw the current_wall
 */
void wall_mode_enter(void)
{
    // we undraw the available moves
    delete_available_moves();
    // reset the player token
    game.player_turn == PLAYER_1 ? (reset_p1_token()) : (reset_p2_token());
    // changing the game mode
    game.input_mode = WALL_MODE;
    // draw the current_wall wall
    current_wall_init();
}

/**
 * @brief We need to delete the current wall from the screen
 */
void wall_mode_exit(void)
{
    delete_current_wall();
    show_available_moves();
    game.input_mode = PLAYER_MOVEMENT;
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
