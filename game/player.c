#include "./player.h"
#include "./quoridor.h"
#include "../graphics/graphics.h"

// inside here all the functions related to the players management

/**
 * @brief In this function we basically initialize the game_data structure
 * with the players starting position.
 * - player_1 will be the WHITE one
 * - player_2 will be the RED one
 * @param game
 */
void init_players(game_data *game)
{
    // Setting player_1 data
    game->player_1.player_color = WHITE;
    game->player_1.x_matrix_coordinate = 0;
    game->player_1.y_matrix_coordinate = 6;
    game->player_1.tmp_x_matrix_coordinate = 0;
    game->player_1.tmp_y_matrix_coordinate = 6;
    game->player_1.available_walls = 8;
    // Also the index of the vector of moves
    game->player_1.num_of_possible_moves = 0;
    // Setting player_2 data
    game->player_2.player_color = RED;
    game->player_2.x_matrix_coordinate = 12;
    game->player_2.y_matrix_coordinate = 6;
    game->player_2.tmp_x_matrix_coordinate = 12;
    game->player_2.tmp_y_matrix_coordinate = 6;
    game->player_2.available_walls = 8;
    game->player_2.num_of_possible_moves = 0;
    // Dobbiamo aggiornare la board
    game->board[0][6].availability = OCCUPIED;
    game->board[12][6].availability = OCCUPIED;
    // We also need to initialize the tick number
    game->game_tick = 20;
    // And we need to assign the first player to the white one
    game->player_turn = PLAYER_1;
    // We are in wait mode
    game->game_status = WAIT_MODE;
    // We begin in player_mode
    game->input_mode = PLAYER_MOVEMENT;
    // Setting the text area status
    game->text_area_status = CLEAR;
}

void find_available_moves(game_data *game)
{
    // we need to find all available moves given the cell
    ui8 x, y;
    int i;
    matrix_point destination;
    if (game->player_turn == PLAYER_1)
    {
        x = game->player_1.x_matrix_coordinate;
        y = game->player_1.y_matrix_coordinate;
        // We need to reset it every time
        game->player_1.num_of_possible_moves = 0;
        for (i = 0; i < NUM_MOVES; i++)
        {
            game->player_1.possible_moves[i] = (matrix_point){0, 0};
        }
    }
    else
    {
        x = game->player_2.x_matrix_coordinate;
        y = game->player_2.y_matrix_coordinate;
        game->player_2.num_of_possible_moves = 0;
        for (i = 0; i < NUM_MOVES; i++)
        {
            game->player_2.possible_moves[i] = (matrix_point){0, 0};
        }
    }

    // UP
    if (!(x - 2 < 0 || game->board[x - 1][y].availability == OCCUPIED))
    {
        // going up is legal
        if (game->board[x - 2][y].availability == OCCUPIED)
        {
            // The opponent is facing us, we need to check if we can jump him
            if (!(x - 4 < 0 || game->board[x - 3][y].availability == OCCUPIED))
                add_move(game, (matrix_point){x - 4, y});
        }
        else
        {
            add_move(game, (matrix_point){x - 2, y});
        }
    }

    // DOWN
    // checking that the move is legal, now we are moving down so increasing X
    if (!(x + 2 > 12 || game->board[x + 1][y].availability == OCCUPIED))
    {
        // going down is legal
        if (game->board[x + 2][y].availability == OCCUPIED)
        {
            if (!(x + 4 > 12 || game->board[x + 3][y].availability == OCCUPIED))
                add_move(game, (matrix_point){x + 4, y});
        }
        else
        {
            add_move(game, (matrix_point){x + 2, y});
        }
    }
    // LEFT
    // checking that the move is legal, we are decrementing y
    if (!(y - 2 < 0 || game->board[x][y - 1].availability == OCCUPIED))
    {
        // going left is legal
        if (game->board[x][y - 2].availability == OCCUPIED)
        {
            if (!(y - 4 < 0 || game->board[x][y - 3].availability == OCCUPIED))
                add_move(game, (matrix_point){x, y - 4});
        }
        else
        {
            add_move(game, (matrix_point){x, y - 2});
        }
    }
    // RIGHT
    if (!(y + 2 > 12 || game->board[x][y + 1].availability == OCCUPIED))
    {
        // going right is legal
        if (game->board[x][y + 2].availability == OCCUPIED)
        {
            if (!(y + 4 > 12 || game->board[x][y + 3].availability == OCCUPIED))
                add_move(game, (matrix_point){x, y + 4});
        }
        else
        {
            add_move(game, (matrix_point){x, y + 2});
        }
    }
}

void add_move(game_data *game, matrix_point point)
{
    if (game->player_turn == PLAYER_1)
    {
        game->player_1.possible_moves[game->player_1.num_of_possible_moves] = point;
        game->player_1.num_of_possible_moves++;
    }
    else
    {
        // player's 2 turn
        game->player_2.possible_moves[game->player_2.num_of_possible_moves] = point;
        game->player_2.num_of_possible_moves++;
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
void move_player(move_type move, game_data *game, active_player player)
{
    int x, y;
    uint8_t old_tmp_x, old_tmp_y;

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
            // show_available_moves();
            // update_player_token_pos(game->player_1.tmp_x_matrix_coordinate, game->player_1.tmp_y_matrix_coordinate);
            return;
        }
        // We are not over, we still need to check for the presence of the opponent
        if (game->board[x - 2][y].availability == OCCUPIED)
        {
            if (x - 4 < 0 || game->board[x - 3][y].availability == OCCUPIED)
            {
                return;
            }
            // If we are still here the opponent is facing us but the landing cell is legal
            game->player_turn == PLAYER_1 ? (game->player_1.tmp_x_matrix_coordinate = x - 4) : (game->player_2.tmp_x_matrix_coordinate = x - 4);
            // updating the Interface
            update_player_token_pos(x - 4, y);
            return;
        }
        // If we are here the move is legal and the opponent is not facing us
        game->player_turn == PLAYER_1 ? (game->player_1.tmp_x_matrix_coordinate = x - 2) : (game->player_2.tmp_x_matrix_coordinate = x - 2);
        // updating the Interface
        update_player_token_pos(x - 2, y);
    }
    else if (move == DOWN)
    {
        // checking that the move is legal, now we are moving down so increasing X
        if (x + 2 > 12 || game->board[x + 1][y].availability == OCCUPIED)
        {
            return;
        }
        if (game->board[x + 2][y].availability == OCCUPIED)
        {
            if (x + 4 > 12 || game->board[x + 3][y].availability == OCCUPIED)
            {
                return;
            }
            game->player_turn == PLAYER_1 ? (game->player_1.tmp_x_matrix_coordinate = x + 4) : (game->player_2.tmp_x_matrix_coordinate = x + 4);
            // updating the Interface
            update_player_token_pos(x + 4, y);
            return;
        }
        game->player_turn == PLAYER_1 ? (game->player_1.tmp_x_matrix_coordinate = x + 2) : (game->player_2.tmp_x_matrix_coordinate = x + 2);
        // updating the Interface
        update_player_token_pos(x + 2, y);
    }
    else if (move == LEFT)
    {
        // checking that the move is legal, we are decrementing y
        if (y - 2 < 0 || game->board[x][y - 1].availability == OCCUPIED)
        {
            return;
        }
        if (game->board[x][y - 2].availability == OCCUPIED)
        {
            if (y - 4 < 0 || game->board[x][y - 3].availability == OCCUPIED)
            {
                return;
            }
            game->player_turn == PLAYER_1 ? (game->player_1.tmp_y_matrix_coordinate = y - 4) : (game->player_2.tmp_y_matrix_coordinate = y - 4);
            // updating the Interface
            update_player_token_pos(x, y - 4);
            return;
        }
        game->player_turn == PLAYER_1 ? (game->player_1.tmp_y_matrix_coordinate = y - 2) : (game->player_2.tmp_y_matrix_coordinate = y - 2);
        // updating the Interface
        update_player_token_pos(x, y - 2);
    }
    else if (move == RIGHT)
    {
        // checking that the move is legal, we are incrementing y
        if (y + 2 > 12 || game->board[x][y + 1].availability == OCCUPIED)
        {
            return;
        }
        if (game->board[x][y + 2].availability == OCCUPIED)
        {
            if (y + 4 > 12 || game->board[x][y + 3].availability == OCCUPIED)
            {
                return;
            }
            game->player_turn == PLAYER_1 ? (game->player_1.tmp_y_matrix_coordinate = y + 4) : (game->player_2.tmp_y_matrix_coordinate = y + 4);
            // updating the Interface
            update_player_token_pos(x, y + 4);
            return;
        }
        game->player_turn == PLAYER_1 ? (game->player_1.tmp_y_matrix_coordinate = y + 2) : (game->player_2.tmp_y_matrix_coordinate = y + 2);
        // updating the Interface
        update_player_token_pos(x, y + 2);
    }
}
