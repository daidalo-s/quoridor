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
 * minimax(depth, maximizing)
 * max_value(state):
 *  if terminal(s) return Utility(s)
 *  value = -inf
 *  for every n_s in next_states(state)
 *      v' = min_value(n_s)
 *      if v' > v => v = v'
 *  return v;
 *
 * min_value(state):
 *  if terminal(s) return Utility(s)
 *  value = +inf
 *  for every n_s in next_states(state)
 *      v' = max_value(n_s)
 *      if v' < v => v = v'
 *  return v;
 */
move minimax(ui8 depth)
{
    move best_move, move;
    int i;
    matrix_point previous_position;
    wall tmp_wall;
    player *player;
    // non so quale sia questa mossa ma so che ha uno score bassissimo
    best_move.score = INT32_MIN;
    // ora mi guardo tutte le mosse possibili di movimento
    // -> prende le coordinate del current_player e calcola a partire da quelle
    game.player_turn == PLAYER_1 ? (player = &game.player_1) : (player = &game.player_2);
    find_available_moves(&game);
    for (i = 0; i < player->num_of_possible_moves; i++)
    {
        // do the move m -> ho un matrix point
        previous_position = (matrix_point){player->x_matrix_coordinate, player->y_matrix_coordinate};
        minimax_move_player(player, player->possible_moves[i]);
        move.type_of_move.type = PLAYER;
        move.x = player->x_matrix_coordinate;
        move.y = player->y_matrix_coordinate;
        move.score = MIN(depth - 1);
        if (move.score > best_move.score)
        {
            best_move = move;
        }
        minimax_unmove_player(player, previous_position);
    }
    // ora mi guardo tutte le mosse possibili di posizionamento muri
    // TODO
    // ho abbastanza muri
    if (player->available_walls > 0)
    {
        find_all_possible_walls();
        for (i = 0; i < game.wall_moves.num_of_moves; i++)
        {
            // minimax_move_player(player, player->possible_moves[i]);
            tmp_wall = game.wall_moves.all_possible_walls[i];
            minimax_place_wall(game.wall_moves.all_possible_walls[i]);
            move.type_of_move.type = WALL;
            move.x = game.wall_moves.all_possible_walls[i].middle.x;
            move.y = game.wall_moves.all_possible_walls[i].middle.y;
            move.type_of_move.orientation = game.wall_moves.all_possible_walls[i].wall_orientation;
            move.score = MIN(depth - 1);
            if (move.score > best_move.score)
            {
                best_move = move;
            }
            minimax_unplace_wall(tmp_wall);
        }
    }
    return best_move;
}

int MIN(ui8 depth)
{
    move best_move, move;
    matrix_point previous_position;
    wall tmp_wall;
    player *player;
    int i;
    game.player_turn == PLAYER_1 ? (player = &game.player_1) : (player = &game.player_2);
    // se il game è finito devo ritornare eval_ending
    if (minimax_game_over())
    {
        // return EVAL_ENDING -> + inf se ha vinto il bot, - inf se perdo
        if (game.player_1.bot)
        {
            // il player_1 è un bot, ha vinto?
            if (game.player_1.x_matrix_coordinate == 12)
            {
                return INT32_MAX;
            }
            else
            {
                // ha vinto l'opponent
                return INT32_MIN;
            }
        }
        else
        {
            // il player_2 è il bot
            if (game.player_2.x_matrix_coordinate == 0)
            {
                return INT32_MAX;
            }
            else
            {
                return INT32_MIN;
            }
        }
    }
    else if (depth == 0)
    {
        return state_eval();
    }
    else
    {
        best_move.score = INT32_MAX;
        // ora mi guardo tutte le mosse possibili di movimento
        // -> prende le coordinate del current_player e calcola a partire da quelle
        find_available_moves(&game);
        for (i = 0; i < player->num_of_possible_moves; i++)
        {
            // do the move m -> ho un matrix point
            previous_position = (matrix_point){player->x_matrix_coordinate, player->y_matrix_coordinate};
            minimax_move_player(player, player->possible_moves[i]);
            move.type_of_move.type = PLAYER;
            move.x = player->x_matrix_coordinate;
            move.y = player->y_matrix_coordinate;
            move.score = MAX(depth - 1);
            if (move.score < best_move.score)
            {
                best_move = move;
            }
            minimax_unmove_player(player, previous_position);
        }
        // TODO
        // ho abbastanza muri
        if (player->available_walls > 0)
        {
            find_all_possible_walls();
            for (i = 0; i < game.wall_moves.num_of_moves; i++)
            {
                // do the move m -> ho un matrix point
                // minimax_move_player(player, player->possible_moves[i]);
                tmp_wall = game.wall_moves.all_possible_walls[i];
                minimax_place_wall(game.wall_moves.all_possible_walls[i]);
                move.type_of_move.type = WALL;
                move.x = game.wall_moves.all_possible_walls[i].middle.x;
                move.y = game.wall_moves.all_possible_walls[i].middle.y;
                move.type_of_move.orientation = game.wall_moves.all_possible_walls[i].wall_orientation;
                move.score = MAX(depth - 1);
                if (move.score < best_move.score)
                {
                    best_move = move;
                }
                minimax_unplace_wall(tmp_wall);
            }
        }
    }
    return best_move.score;
}

int MAX(ui8 depth)
{
    move best_move, move;
    matrix_point previous_position;
    wall tmp_wall;
    player *player;
    int i;
    game.player_turn == PLAYER_1 ? (player = &game.player_1) : (player = &game.player_2);
    // se il game è finito devo ritornare eval_ending
    if (minimax_game_over())
    {
        // return EVAL_ENDING -> + inf se ha vinto il bot, - inf se perdo
        if (game.player_1.bot)
        {
            // il player_1 è un bot, ha vinto?
            if (game.player_1.x_matrix_coordinate == 12)
            {
                return INT32_MAX;
            }
            else
            {
                // ha vinto l'opponent
                return INT32_MIN;
            }
        }
        else
        {
            // il player_2 è il bot
            if (game.player_2.x_matrix_coordinate == 0)
            {
                return INT32_MAX;
            }
            else
            {
                return INT32_MIN;
            }
        }
    }
    else if (depth == 0)
    {
        return state_eval();
    }
    else
    {
        best_move.score = INT32_MIN;
        // ora mi guardo tutte le mosse possibili di movimento
        // -> prende le coordinate del current_player e calcola a partire da quelle
        find_available_moves(&game);
        for (i = 0; i < player->num_of_possible_moves; i++)
        {
            // do the move m -> ho un matrix point
            previous_position = (matrix_point){player->x_matrix_coordinate, player->y_matrix_coordinate};
            minimax_move_player(player, player->possible_moves[i]);
            move.type_of_move.type = PLAYER;
            move.x = player->x_matrix_coordinate;
            move.y = player->y_matrix_coordinate;
            move.score = MIN(depth - 1);
            if (move.score < best_move.score)
            {
                best_move = move;
            }
            minimax_unmove_player(player, previous_position);
        }
        // TODO
        //       ho abbastanza muri if (player->available_walls > 0)
        {
            find_all_possible_walls();
            for (i = 0; i < game.wall_moves.num_of_moves; i++)
            {
                // do the move m -> ho un matrix point
                // minimax_move_player(player, player->possible_moves[i]);
                tmp_wall = game.wall_moves.all_possible_walls[i];
                minimax_place_wall(game.wall_moves.all_possible_walls[i]);
                move.type_of_move.type = WALL;
                move.x = game.wall_moves.all_possible_walls[i].middle.x;
                move.y = game.wall_moves.all_possible_walls[i].middle.y;
                move.type_of_move.orientation = game.wall_moves.all_possible_walls[i].wall_orientation;
                move.score = MIN(depth - 1);
                if (move.score < best_move.score)
                {
                    best_move = move;
                }
                minimax_unplace_wall(tmp_wall);
            }
        }
    }
    return best_move.score;
}

/**
 * Moving a player means setting as free his position, changing the x-y coordinates and
 * marking that cell as occupied
 * @param player
 * @param destination
 */
void minimax_move_player(player *player, matrix_point destination)
{
    game.board[player->x_matrix_coordinate][player->y_matrix_coordinate].availability = FREE;
    player->x_matrix_coordinate = destination.x;
    player->y_matrix_coordinate = destination.y;
    game.board[player->x_matrix_coordinate][player->y_matrix_coordinate].availability = OCCUPIED;
    minimax_swap_turn();
}

void minimax_unmove_player(player *player, matrix_point destination)
{
    game.board[player->x_matrix_coordinate][player->y_matrix_coordinate].availability = FREE;
    player->x_matrix_coordinate = destination.x;
    player->y_matrix_coordinate = destination.y;
    game.board[player->x_matrix_coordinate][player->y_matrix_coordinate].availability = OCCUPIED;
    minimax_swap_turn();
}

void minimax_place_wall(wall wall)
{
    game.current_wall.top.x = wall.top.x;
    game.current_wall.top.y = wall.top.y;
    game.current_wall.middle.x = wall.middle.x;
    game.current_wall.middle.y = wall.middle.y;
    game.current_wall.bottom.x = wall.bottom.x;
    game.current_wall.bottom.y = wall.bottom.y;
    place_current_wall();
    minimax_swap_turn();
}
void minimax_unplace_wall(wall wall)
{
    game.current_wall.top.x = wall.top.x;
    game.current_wall.top.y = wall.top.y;
    game.current_wall.middle.x = wall.middle.x;
    game.current_wall.middle.y = wall.middle.y;
    game.current_wall.bottom.x = wall.bottom.x;
    game.current_wall.bottom.y = wall.bottom.y;
    remove_current_wall();
    minimax_swap_turn();
}

void minimax_swap_turn(void)
{
    game.player_turn == PLAYER_1 ? (game.player_turn = PLAYER_2) : (game.player_turn = PLAYER_1);
}

ui8 minimax_game_over(void)
{
    if (game.player_turn == PLAYER_1)
    {
        if (game.player_1.x_matrix_coordinate == 12)
            return 1;
    }
    else
    {
        if (game.player_2.x_matrix_coordinate == 0)
            return 1;
    }
    return 0;
}

int state_eval(void)
{
    // quanto manca a me - quanto manca all'avversario
    // bfs_node node1, node2;
    // active_player other_player;
    // game.player_turn == PLAYER_1 ? (other_player = PLAYER_2) : (other_player = PLAYER_1);
    // node1 = bfs(game.player_turn);
    // node2 = bfs(other_player);
    // return -(node1.length - node2.length);
    // the evaluation need to always be from the bot point of view
    // -(bot.length - opponent.length)
    bfs_node node1, node2;
    if (game.player_1.bot)
    {
        node1 = bfs(PLAYER_1);
        // node1.length += 1;
        node2 = bfs(PLAYER_2);
        if (node2.length == 1)
        {
            // he's really close to the win
            return INT32_MIN;
        }
        else
        {
            node2.length -= 1;
        }
    }
    else
    {
        // è il player_2
        node1 = bfs(PLAYER_2);
        node2 = bfs(PLAYER_1);
        // node2.length += 1;
        if (node2.length == 1)
        {
            // he's really close to the win
            return INT32_MIN;
        }
        else
        {
            node1.length -= -1;
        }
    }
    return -(node1.length - node2.length);
}

move minimax_alfa_beta(ui8 depth, int alpha, int beta)
{
    move best_move, move;
    int i;
    matrix_point previous_position;
    wall tmp_wall;
    player *player;
    // non so quale sia questa mossa ma so che ha uno score bassissimo
    best_move.score = INT32_MIN;
    // ora mi guardo tutte le mosse possibili di movimento
    // -> prende le coordinate del current_player e calcola a partire da quelle
    game.player_turn == PLAYER_1 ? (player = &game.player_1) : (player = &game.player_2);
    find_available_moves(&game);
    for (i = 0; i < player->num_of_possible_moves; i++)
    {
        // do the move m -> ho un matrix point
        previous_position = (matrix_point){player->x_matrix_coordinate, player->y_matrix_coordinate};
        minimax_move_player(player, player->possible_moves[i]);
        move.type_of_move.type = PLAYER;
        move.x = player->x_matrix_coordinate;
        move.y = player->y_matrix_coordinate;
        move.score = MIN_alfa_beta(depth - 1, alpha, beta);
        if (move.score >= best_move.score)
        {
            best_move.score = max(best_move.score, move.score);
            best_move = move;
        }
        alpha = max(alpha, best_move.score);
        if (beta <= alpha)
        {
            minimax_unmove_player(player, previous_position);
            break;
        }
        minimax_unmove_player(player, previous_position);
    }
    // ora mi guardo tutte le mosse possibili di posizionamento muri
    // TODO
    // ho abbastanza muri
    if (player->available_walls > 0)
    {
        find_all_possible_walls();
        for (i = 0; i < game.wall_moves.num_of_moves; i++)
        {
            // minimax_move_player(player, player->possible_moves[i]);
            tmp_wall = game.wall_moves.all_possible_walls[i];
            minimax_place_wall(game.wall_moves.all_possible_walls[i]);
            move.type_of_move.type = WALL;
            move.x = game.wall_moves.all_possible_walls[i].middle.x;
            move.y = game.wall_moves.all_possible_walls[i].middle.y;
            move.type_of_move.orientation = game.wall_moves.all_possible_walls[i].wall_orientation;
            move.score = MIN_alfa_beta(depth - 1, alpha, beta);
            if (move.score >= best_move.score)
            {
                best_move.score = max(best_move.score, move.score);
                best_move = move;
            }
            alpha = max(alpha, best_move.score);
            if (beta <= alpha)
            {
                minimax_unplace_wall(tmp_wall);
                break;
            }
            minimax_unplace_wall(tmp_wall);
        }
    }
    return best_move;
}

int MIN_alfa_beta(ui8 depth, int alpha, int beta)
{
    move best_move, move;
    matrix_point previous_position;
    wall tmp_wall;
    player *player;
    int i;
    game.player_turn == PLAYER_1 ? (player = &game.player_1) : (player = &game.player_2);
    // se il game è finito devo ritornare eval_ending
    if (minimax_game_over())
    {
        // return EVAL_ENDING -> + inf se ha vinto il bot, - inf se perdo
        if (game.player_1.bot)
        {
            // il player_1 è un bot, ha vinto?
            if (game.player_1.x_matrix_coordinate == 12)
            {
                return INT32_MAX;
            }
            else
            {
                // ha vinto l'opponent
                return INT32_MIN;
            }
        }
        else
        {
            // il player_2 è il bot
            if (game.player_2.x_matrix_coordinate == 0)
            {
                return INT32_MAX;
            }
            else
            {
                return INT32_MIN;
            }
        }
    }
    else if (depth == 0)
    {
        return state_eval();
    }
    else
    {
        best_move.score = INT32_MAX;
        // ora mi guardo tutte le mosse possibili di movimento
        // -> prende le coordinate del current_player e calcola a partire da quelle
        find_available_moves(&game);
        for (i = 0; i < player->num_of_possible_moves; i++)
        {
            // do the move m -> ho un matrix point
            previous_position = (matrix_point){player->x_matrix_coordinate, player->y_matrix_coordinate};
            minimax_move_player(player, player->possible_moves[i]);
            move.type_of_move.type = PLAYER;
            move.x = player->x_matrix_coordinate;
            move.y = player->y_matrix_coordinate;
            move.score = MAX_alfa_beta(depth - 1, alpha, beta);
            best_move.score = min(best_move.score, move.score);
            beta = min(beta, best_move.score);
            if (beta <= alpha)
            {
                minimax_unmove_player(player, previous_position);
                break;
            }
            // if (move.score < best_move.score)
            // {
            //     best_move = move;
            // }
            minimax_unmove_player(player, previous_position);
        }
        // TODO
        // ho abbastanza muri
        if (player->available_walls > 0)
        {
            find_all_possible_walls();
            for (i = 0; i < game.wall_moves.num_of_moves; i++)
            {
                // do the move m -> ho un matrix point
                // minimax_move_player(player, player->possible_moves[i]);
                tmp_wall = game.wall_moves.all_possible_walls[i];
                minimax_place_wall(game.wall_moves.all_possible_walls[i]);
                move.type_of_move.type = WALL;
                move.x = game.wall_moves.all_possible_walls[i].middle.x;
                move.y = game.wall_moves.all_possible_walls[i].middle.y;
                move.score = MAX_alfa_beta(depth - 1, alpha, beta);
                best_move.score = min(best_move.score, move.score);
                beta = min(beta, best_move.score);
                if (beta <= alpha)
                {
                    minimax_unplace_wall(tmp_wall);
                    break;
                }
                // if (move.score < best_move.score)
                // {
                //     best_move = move;
                // }
                minimax_unplace_wall(tmp_wall);
            }
        }
    }
    return best_move.score;
}

int MAX_alfa_beta(ui8 depth, int alpha, int beta)
{
    move best_move, move;
    matrix_point previous_position;
    wall tmp_wall;
    player *player;
    int i;
    game.player_turn == PLAYER_1 ? (player = &game.player_1) : (player = &game.player_2);
    // se il game è finito devo ritornare eval_ending
    if (minimax_game_over())
    {
        // return EVAL_ENDING -> + inf se ha vinto il bot, - inf se perdo
        if (game.player_1.bot)
        {
            // il player_1 è un bot, ha vinto?
            if (game.player_1.x_matrix_coordinate == 12)
            {
                return INT32_MAX;
            }
            else
            {
                // ha vinto l'opponent
                return INT32_MIN;
            }
        }
        else
        {
            // il player_2 è il bot
            if (game.player_2.x_matrix_coordinate == 0)
            {
                return INT32_MAX;
            }
            else
            {
                return INT32_MIN;
            }
        }
    }
    else if (depth == 0)
    {
        return state_eval();
    }
    else
    {
        best_move.score = INT32_MIN;
        // ora mi guardo tutte le mosse possibili di movimento
        // -> prende le coordinate del current_player e calcola a partire da quelle
        find_available_moves(&game);
        for (i = 0; i < player->num_of_possible_moves; i++)
        {
            // do the move m -> ho un matrix point
            previous_position = (matrix_point){player->x_matrix_coordinate, player->y_matrix_coordinate};
            minimax_move_player(player, player->possible_moves[i]);
            move.type_of_move.type = PLAYER;
            move.x = player->x_matrix_coordinate;
            move.y = player->y_matrix_coordinate;
            move.score = MIN_alfa_beta(depth - 1, alpha, beta);
            best_move.score = max(best_move.score, move.score);
            alpha = max(alpha, best_move.score);
            if (beta <= alpha)
            {
                minimax_unmove_player(player, previous_position);
                break;
            }
            // if (move.score < best_move.score)
            // {
            //     best_move = move;
            // }
            minimax_unmove_player(player, previous_position);
        }
        // TODO
        //       ho abbastanza muri if (player->available_walls > 0)
        {
            find_all_possible_walls();
            for (i = 0; i < game.wall_moves.num_of_moves; i++)
            {
                // do the move m -> ho un matrix point
                // minimax_move_player(player, player->possible_moves[i]);
                tmp_wall = game.wall_moves.all_possible_walls[i];
                minimax_place_wall(game.wall_moves.all_possible_walls[i]);
                move.type_of_move.type = WALL;
                move.x = game.wall_moves.all_possible_walls[i].middle.x;
                move.y = game.wall_moves.all_possible_walls[i].middle.y;
                move.type_of_move.orientation = game.wall_moves.all_possible_walls[i].wall_orientation;
                move.score = MIN_alfa_beta(depth - 1, alpha, beta);
                best_move.score = max(best_move.score, move.score);
                alpha = max(alpha, best_move.score);
                if (beta <= alpha)
                {
                    minimax_unplace_wall(tmp_wall);
                    break;
                }
                // if (move.score < best_move.score)
                // {
                //     best_move = move;
                // }
                minimax_unplace_wall(tmp_wall);
            }
        }
    }
    return best_move.score;
}

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
// minimax_res minimaxxx(ui8 depth, ui8 maximizingPlayer)
// {
//     int node_value;
//     int i;
//     minimax_res move, res;
//     wall tmp_wall;
//     ui8 x, y;

//     if (game.player_1.x_matrix_coordinate == 12)
//     {
//         // move.value = INT32_MIN;
//         move.value = INT32_MAX;
//         return move;
//     }

//     if (game.player_2.x_matrix_coordinate == 0)
//     {
//         // move.value = INT32_MIN;
//         move.value = INT32_MIN;
//         return move;
//     }

//     /**
//      * If we are in a "terminal" node, we return the heuristic for that node
//      */
//     if (depth == 0)
//     {
//         move.value = heuristic_value_of_node();
//         return move;
//     }

//     /**
//      * If we reached a winning move we need to stop the search, it's game over
//      */

//     /**
//      * The bot will always be called with maximizing as an objective
//      */
//     if (maximizingPlayer)
//     {
//         move.value = INT32_MIN;
//         find_available_moves(&game);
//         find_all_possible_walls();

//         x = game.player_1.x_matrix_coordinate;
//         y = game.player_1.y_matrix_coordinate;

//         for (i = 0; i < game.player_1.num_of_possible_moves; i++)
//         {
//             minimax_move_player(i, PLAYER_1);
//             res = minimax(depth - 1, 0);
//             if (res.value > move.value)
//             {
//                 move.value = res.value;
//                 move.x = game.player_1.possible_moves[i].x;
//                 move.y = game.player_1.possible_moves[i].y;
//                 move.wall_orientation = NONE;
//             }
//             minimax_unmove_player(x, y, PLAYER_1);
//         }
//         for (i = 0; i < game.wall_moves.num_of_moves; i++)
//         {
//             minimax_place_wall(i, PLAYER_1);
//             res = minimax(depth - 1, 0);
//             if (res.value > move.value)
//             {
//                 move.value = res.value;
//                 move.x = game.wall_moves.all_possible_walls[i].middle.x;
//                 move.y = game.wall_moves.all_possible_walls[i].middle.y;
//                 move.wall_orientation = game.wall_moves.all_possible_walls[i].wall_orientation;
//             }
//             minimax_remove_wall(i, PLAYER_1);
//         }
//     }
//     else
//     {
//         move.value = INT32_MAX;
//         find_available_moves(&game);
//         find_all_possible_walls();
//         x = game.player_2.x_matrix_coordinate;
//         y = game.player_2.y_matrix_coordinate;
//         for (i = 0; i < game.player_2.num_of_possible_moves; i++)
//         {
//             minimax_move_player(i, PLAYER_2);
//             res = minimax(depth - 1, 1);
//             if (res.value < move.value)
//             {
//                 move.value = res.value;
//                 move.x = game.player_2.possible_moves[i].x;
//                 move.y = game.player_2.possible_moves[i].y;
//             }
//             minimax_unmove_player(x, y, PLAYER_2);
//         }
//         for (i = 0; i < game.wall_moves.num_of_moves; i++)
//         {
//             minimax_place_wall(i, PLAYER_2);
//             res = minimax(depth - 1, 0);
//             if (res.value > move.value)
//             {
//                 move.value = res.value;
//                 move.x = game.wall_moves.all_possible_walls[i].middle.x;
//                 move.y = game.wall_moves.all_possible_walls[i].middle.y;
//                 move.wall_orientation = game.wall_moves.all_possible_walls[i].wall_orientation;
//             }
//             minimax_remove_wall(i, PLAYER_2);
//         }
//     }
//     return move;
// }

// int heuristic_value_of_node()
// {
//     // if (game.player_turn == PLAYER_1)
//     //     return 12 - game.player_1.x_matrix_coordinate;
//     // else
//     //     return game.player_2.x_matrix_coordinate;
//     return (game.player_1.x_matrix_coordinate - 12) + (game.player_2.x_matrix_coordinate);
// }

// void minimaxxx_move_player(ui8 index, active_player player)
// {
//     if (player == PLAYER_1)
//     {
//         game.board[game.player_1.x_matrix_coordinate][game.player_1.y_matrix_coordinate].availability = FREE;
//         game.player_1.x_matrix_coordinate = game.player_1.possible_moves[index].x;
//         game.player_1.y_matrix_coordinate = game.player_1.possible_moves[index].y;
//         game.board[game.player_1.x_matrix_coordinate][game.player_1.y_matrix_coordinate].availability = OCCUPIED;
//         game.player_turn = PLAYER_2;
//     }
//     else
//     {
//         game.board[game.player_2.x_matrix_coordinate][game.player_2.y_matrix_coordinate].availability = FREE;
//         game.player_2.x_matrix_coordinate = game.player_2.possible_moves[index].x;
//         game.player_2.y_matrix_coordinate = game.player_2.possible_moves[index].y;
//         game.board[game.player_2.x_matrix_coordinate][game.player_2.y_matrix_coordinate].availability = OCCUPIED;
//         game.player_turn = PLAYER_1;
//     }
// }

// void minimaxxxx_unmove_player(ui8 x, ui8 y, active_player player)
// {
//     if (player == PLAYER_1)
//     {
//         game.board[game.player_1.x_matrix_coordinate][game.player_1.y_matrix_coordinate].availability = FREE;
//         game.player_1.x_matrix_coordinate = x;
//         game.player_1.y_matrix_coordinate = y;
//         game.board[game.player_1.x_matrix_coordinate][game.player_1.y_matrix_coordinate].availability = OCCUPIED;
//         game.player_turn = PLAYER_1;
//     }
//     else
//     {
//         game.board[game.player_2.x_matrix_coordinate][game.player_2.y_matrix_coordinate].availability = FREE;
//         game.player_2.x_matrix_coordinate = x;
//         game.player_2.y_matrix_coordinate = y;
//         game.board[game.player_2.x_matrix_coordinate][game.player_2.y_matrix_coordinate].availability = OCCUPIED;
//         game.player_turn = PLAYER_2;
//     }
// }

// void minimaxxxx_place_wall(ui8 index, active_player player)
// {
//     if (player == PLAYER_1)
//     {
//         game.board[game.wall_moves.all_possible_walls[index].top.x][game.wall_moves.all_possible_walls[index].top.y].availability = OCCUPIED;
//         game.board[game.wall_moves.all_possible_walls[index].middle.x][game.wall_moves.all_possible_walls[index].middle.y].availability = OCCUPIED;
//         game.board[game.wall_moves.all_possible_walls[index].bottom.x][game.wall_moves.all_possible_walls[index].bottom.y].availability = OCCUPIED;
//         game.player_turn = PLAYER_2;
//     }
//     else
//     {
//         game.board[game.wall_moves.all_possible_walls[index].top.x][game.wall_moves.all_possible_walls[index].top.y].availability = OCCUPIED;
//         game.board[game.wall_moves.all_possible_walls[index].middle.x][game.wall_moves.all_possible_walls[index].middle.y].availability = OCCUPIED;
//         game.board[game.wall_moves.all_possible_walls[index].bottom.x][game.wall_moves.all_possible_walls[index].bottom.y].availability = OCCUPIED;
//         game.player_turn = PLAYER_1;
//     }
// }

// void minimaxxxx_remove_wall(ui8 index, active_player player)
// {
//     if (player == PLAYER_1)
//     {
//         game.board[game.wall_moves.all_possible_walls[index].top.x][game.wall_moves.all_possible_walls[index].top.y].availability = FREE;
//         game.board[game.wall_moves.all_possible_walls[index].middle.x][game.wall_moves.all_possible_walls[index].middle.y].availability = FREE;
//         game.board[game.wall_moves.all_possible_walls[index].bottom.x][game.wall_moves.all_possible_walls[index].bottom.y].availability = FREE;
//         game.player_turn = PLAYER_1;
//     }
//     else
//     {
//         game.board[game.wall_moves.all_possible_walls[index].top.x][game.wall_moves.all_possible_walls[index].top.y].availability = FREE;
//         game.board[game.wall_moves.all_possible_walls[index].middle.x][game.wall_moves.all_possible_walls[index].middle.y].availability = FREE;
//         game.board[game.wall_moves.all_possible_walls[index].bottom.x][game.wall_moves.all_possible_walls[index].bottom.y].availability = FREE;
//         game.player_turn = PLAYER_2;
//     }
// }
