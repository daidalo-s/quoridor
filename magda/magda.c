/**
 * @file magda.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-01-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "../game/quoridor.h"
#include "../game/player.h"
#include "../game/walls.h"
#include "./magda.h"

extern game_data game;

/**
function minimax(node, depth, maximizingPlayer) is
    if depth = 0 or node is a terminal node then
        return the heuristic value of node
    if maximizingPlayer then
        value := −∞
        for each child of node do
            value := max(value, minimax(child, depth − 1, FALSE))
        return value
    else (* minimizing player *)
        value := +∞
        for each child of node do
            value := min(value, minimax(child, depth − 1, TRUE))
        return value

(* Initial call *)
minimax(origin, depth, TRUE)
 *
 */
// prendere la board
// decidiamo che il bot è player_1 che comincia da sopra
minimax_res minimax(ui8 depth, ui8 maximizingPlayer)
{
    int node_value;
    int i;
    minimax_res move, res;
    wall tmp_wall;
    ui8 x, y;
    if (depth == 0)
    {
        move.value = heuristic_value_of_node();
        return move;
    }

    // mi voglio fermare perché altrimenti continuo a guardare ma è la fine, è il check
    // di game over
    if (game.player_1.x_matrix_coordinate == 12)
    {
        move.value = INT32_MIN;
        return move;
    }

    // se il game non è over
    if (maximizingPlayer)
    {
        move.value = INT32_MIN;
        find_available_moves(&game);
        find_all_possible_walls();
        x = game.player_1.x_matrix_coordinate;
        y = game.player_1.y_matrix_coordinate;
        for (i = 0; i < game.player_1.num_of_possible_moves; i++)
        {
            minimax_move_player(i, x, y, PLAYER_1);
            res = minimax(depth - 1, 0);
            if (res.value > move.value)
            {
                move.value = res.value;
                move.x = game.player_1.possible_moves[i].x;
                move.y = game.player_1.possible_moves[i].y;
                move.wall_orientation = NONE;
            }
            minimax_unmove_player(x, y, PLAYER_1);
        }
        for (i = 0; i < game.wall_moves.num_of_moves; i++)
        {
            minimax_place_wall(i, PLAYER_1);
            res = minimax(depth - 1, 0);
            if (res.value > move.value)
            {
                move.value = res.value;
                move.x = game.wall_moves.all_possible_walls[i].middle.x;
                move.y = game.wall_moves.all_possible_walls[i].middle.y;
                move.wall_orientation = game.wall_moves.all_possible_walls[i].wall_orientation;
            }
            minimax_remove_wall(i, PLAYER_1);
        }
    }
    else
    {
        move.value = INT32_MAX;
        find_available_moves(&game);
        find_all_possible_walls();
        x = game.player_2.x_matrix_coordinate;
        y = game.player_2.y_matrix_coordinate;
        for (i = 0; i < game.player_2.num_of_possible_moves; i++)
        {
            minimax_move_player(i, x, y, PLAYER_2);
            res = minimax(depth - 1, 1);
            if (res.value < move.value)
            {
                move.value = res.value;
                move.x = game.player_2.possible_moves[i].x;
                move.y = game.player_2.possible_moves[i].y;
            }
            minimax_unmove_player(x, y, PLAYER_2);
        }
        for (i = 0; i < game.wall_moves.num_of_moves; i++)
        {
            minimax_place_wall(i, PLAYER_2);
            res = minimax(depth - 1, 0);
            if (res.value > move.value)
            {
                move.value = res.value;
                move.x = game.wall_moves.all_possible_walls[i].middle.x;
                move.y = game.wall_moves.all_possible_walls[i].middle.y;
                move.wall_orientation = game.wall_moves.all_possible_walls[i].wall_orientation;
            }
            minimax_remove_wall(i, PLAYER_2);
        }
    }
    return move;
}

int heuristic_value_of_node()
{
    // if (game.player_turn == PLAYER_1)
    //     return 12 - game.player_1.x_matrix_coordinate;
    // else
    //     return game.player_2.x_matrix_coordinate;
    return (game.player_1.x_matrix_coordinate - 12) + (game.player_2.x_matrix_coordinate);
}

void minimax_move_player(ui8 index, ui8 x, ui8 y, active_player player)
{
    if (player == PLAYER_1)
    {
        game.board[game.player_1.x_matrix_coordinate][game.player_1.y_matrix_coordinate].availability = FREE;
        game.player_1.x_matrix_coordinate = game.player_1.possible_moves[index].x;
        game.player_1.y_matrix_coordinate = game.player_1.possible_moves[index].y;
        game.board[game.player_1.x_matrix_coordinate][game.player_1.y_matrix_coordinate].availability = OCCUPIED;
        game.player_turn = PLAYER_2;
    }
    else
    {
        game.board[game.player_2.x_matrix_coordinate][game.player_2.y_matrix_coordinate].availability = FREE;
        game.player_2.x_matrix_coordinate = game.player_2.possible_moves[index].x;
        game.player_2.y_matrix_coordinate = game.player_2.possible_moves[index].y;
        game.board[game.player_2.x_matrix_coordinate][game.player_2.y_matrix_coordinate].availability = OCCUPIED;
        game.player_turn = PLAYER_1;
    }
}

void minimax_unmove_player(ui8 x, ui8 y, active_player player)
{
    if (player == PLAYER_1)
    {
        game.board[game.player_1.x_matrix_coordinate][game.player_1.y_matrix_coordinate].availability = FREE;
        game.player_1.x_matrix_coordinate = x;
        game.player_1.y_matrix_coordinate = y;
        game.board[game.player_1.x_matrix_coordinate][game.player_1.y_matrix_coordinate].availability = OCCUPIED;
        game.player_turn = PLAYER_1;
    }
    else
    {
        game.board[game.player_2.x_matrix_coordinate][game.player_2.y_matrix_coordinate].availability = FREE;
        game.player_2.x_matrix_coordinate = x;
        game.player_2.y_matrix_coordinate = y;
        game.board[game.player_2.x_matrix_coordinate][game.player_2.y_matrix_coordinate].availability = OCCUPIED;
        game.player_turn = PLAYER_2;
    }
}

void minimax_place_wall(ui8 index, active_player player)
{
    if (player == PLAYER_1)
    {
        game.board[game.wall_moves.all_possible_walls[index].top.x][game.wall_moves.all_possible_walls[index].top.y].availability = OCCUPIED;
        game.board[game.wall_moves.all_possible_walls[index].middle.x][game.wall_moves.all_possible_walls[index].middle.y].availability = OCCUPIED;
        game.board[game.wall_moves.all_possible_walls[index].bottom.x][game.wall_moves.all_possible_walls[index].bottom.y].availability = OCCUPIED;
        //
        game.player_turn = PLAYER_2;
    }
    else
    {
        game.board[game.wall_moves.all_possible_walls[index].top.x][game.wall_moves.all_possible_walls[index].top.y].availability = OCCUPIED;
        game.board[game.wall_moves.all_possible_walls[index].middle.x][game.wall_moves.all_possible_walls[index].middle.y].availability = OCCUPIED;
        game.board[game.wall_moves.all_possible_walls[index].bottom.x][game.wall_moves.all_possible_walls[index].bottom.y].availability = OCCUPIED;
        game.player_turn = PLAYER_1;
    }
}

void minimax_remove_wall(ui8 index, active_player player)
{
    if (player == PLAYER_1)
    {
        game.board[game.wall_moves.all_possible_walls[index].top.x][game.wall_moves.all_possible_walls[index].top.y].availability = FREE;
        game.board[game.wall_moves.all_possible_walls[index].middle.x][game.wall_moves.all_possible_walls[index].middle.y].availability = FREE;
        game.board[game.wall_moves.all_possible_walls[index].bottom.x][game.wall_moves.all_possible_walls[index].bottom.y].availability = FREE;
        game.player_turn = PLAYER_1;
    }
    else
    {
        game.board[game.wall_moves.all_possible_walls[index].top.x][game.wall_moves.all_possible_walls[index].top.y].availability = FREE;
        game.board[game.wall_moves.all_possible_walls[index].middle.x][game.wall_moves.all_possible_walls[index].middle.y].availability = FREE;
        game.board[game.wall_moves.all_possible_walls[index].bottom.x][game.wall_moves.all_possible_walls[index].bottom.y].availability = FREE;
        game.player_turn = PLAYER_2;
    }
}