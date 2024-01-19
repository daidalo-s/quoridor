#include "./walls.h"
#include "../graphics/graphics.h"

extern game_data game;

// inside here all the functions related to the walls management

/**
 * @brief In here we init the current wall with the position and the orientation
 * and we draw it
 * TODO: this needs to be completed
 */
void current_wall_init(void)
{
    game.current_wall.wall_orientation = HORIZONTAL;
    game.current_wall.top.x = 5;
    game.current_wall.top.y = 4;
    game.current_wall.middle.x = 5;
    game.current_wall.middle.y = 5;
    game.current_wall.bottom.x = 5;
    game.current_wall.bottom.y = 6;
    draw_current_wall();
}

/**
 * @brief In this function we simply delete the current wall at
 * whatever coordinates
 */
void delete_current_wall(void)
{
}

// Inside of each move we have to check if it's feasible
// The only check we need to do is on the middle cell
// We also need to check if in the destination there is already a wall
// We must check that both top and bottom are in a free cell
void move_wall(wall_move_type move, game_data *game)
{

    int top_x = game->current_wall.top.x;
    int top_y = game->current_wall.top.y;
    int middle_x = game->current_wall.middle.x;
    int middle_y = game->current_wall.middle.y;
    int bottom_x = game->current_wall.bottom.x;
    int bottom_y = game->current_wall.bottom.y;

    if (move == WALL_UP)
    {
        // moving up means decreasing x
        // we need to add the cell check both for top and bottom to be free
        if (middle_x - 2 < 1)
            goto illegal_move;
        // if we are still here the move is legal, we can assign the new coordinates
        game->current_wall.top.x = top_x - 2;
        game->current_wall.middle.x = middle_x - 2;
        game->current_wall.bottom.x = bottom_x - 2;
        update_wall_drawing(WALL_UP);
    }
    else if (move == WALL_DOWN)
    {
        // moving down means increasing x
        if (middle_x + 2 > 11)
            goto illegal_move;
        // The move is legal
        game->current_wall.top.x = top_x + 2;
        game->current_wall.middle.x = middle_x + 2;
        game->current_wall.bottom.x = bottom_x + 2;
        update_wall_drawing(WALL_DOWN);
    }
    else if (move == WALL_LEFT)
    {
        // moving left means decreasing y
        if (middle_y - 2 < 1)
            goto illegal_move;
        // The move is legal
        game->current_wall.top.y = top_y - 2;
        game->current_wall.middle.y = middle_y - 2;
        game->current_wall.bottom.y = bottom_y - 2;
        update_wall_drawing(WALL_LEFT);
    }
    else if (move == WALL_RIGHT)
    {
        // moving right means increasing y
        if (middle_y + 2 > 11)
            goto illegal_move;
        // The move is legal
        game->current_wall.top.y = top_y + 2;
        game->current_wall.middle.y = middle_y + 2;
        game->current_wall.bottom.y = bottom_y + 2;
        update_wall_drawing(WALL_RIGHT);
    }
    else
    {
        // Rotation
        // Non c'è alcuna illegalità nella rotazione
        // middle remains the same
        // Depending from the orientation, we'll need to do different things
        // se orizzontale hanno la stessa x
        if (game->current_wall.top.x == game->current_wall.bottom.x)
        {
            // The wall is horizontal, rotating to vertical
            game->current_wall.top.x = top_x - 1;
            game->current_wall.top.y = top_y + 1;
            game->current_wall.bottom.x = bottom_x + 1;
            game->current_wall.bottom.y = bottom_y - 1;
            update_wall_drawing(WALL_ROTATION);
        }
        else
        {
            // The wall is vertical, rotating to horizontal
            game->current_wall.top.x = top_x + 1;
            game->current_wall.top.y = top_y - 1;
            game->current_wall.bottom.x = bottom_x - 1;
            game->current_wall.bottom.y = bottom_y + 1;
            update_wall_drawing(WALL_ROTATION);
        }
    }
illegal_move:
    return;
}

int legal_wall_placement(game_data *game)
{
    // 2. dfs
    if (feasible_path(game, (active_player)PLAYER_1) == 1 && feasible_path(game, (active_player)PLAYER_2) == 1)
        return 1;
    return 0;
}

void cells_not_visited(game_data *game)
{
    // We need to mark all cells as not visited
    int i;
    int j;
    for (i = 0; i < 13; i++)
    {
        for (j = 0; j < 13; j++)
        {
            game->board[i][j].cell_status = NOT_VISITED;
        }
    }
}

int feasible_path(game_data *game, active_player active_player)
{
    matrix_point start;
    matrix_point v;
    matrix_point tmp;
    // 1. all cells must be marked as not visited
    cells_not_visited(game);
    if (active_player == PLAYER_1)
    {
        start.x = game->player_1.x_matrix_coordinate;
        start.y = game->player_1.y_matrix_coordinate;
    }
    else
    {
        start.x = game->player_2.x_matrix_coordinate;
        start.y = game->player_2.y_matrix_coordinate;
    }

    push(game, start);
    while (game->stack.index != 0)
    {
        v = pop(game);
        if (active_player == PLAYER_1)
        {
            if (v.x == 12)
            {
                game->stack.index = 0;
                return 1;
            }
        }
        else
        {
            if (v.x == 0)
            {
                game->stack.index = 0;
                return 1;
            }
        }
        if (game->board[v.x][v.y].cell_status == NOT_VISITED)
        {
            game->board[v.x][v.y].cell_status = VISITED;
            // For all legal moves starting from v
            if (!(v.x - 2 < 0 || game->board[v.x - 1][v.y].availability == OCCUPIED) && game->board[v.x - 2][v.y].cell_status == NOT_VISITED)
            {
                tmp.x = v.x - 2;
                tmp.y = v.y;
                push(game, tmp);
            }

            if (!(v.x + 2 > 12 || game->board[v.x + 1][v.y].availability == OCCUPIED) && game->board[v.x + 2][v.y].cell_status == NOT_VISITED)
            {
                tmp.x = v.x + 2;
                tmp.y = v.y;
                push(game, tmp);
            }

            if (!(v.y - 2 < 0 || game->board[v.x][v.y - 1].availability == OCCUPIED) && game->board[v.x][v.y - 2].cell_status == NOT_VISITED)
            {
                tmp.x = v.x;
                tmp.y = v.y - 2;
                push(game, tmp);
            }

            if (!(v.y + 2 > 12 || game->board[v.x][v.y + 1].availability == OCCUPIED) && game->board[v.x][v.y + 2].cell_status == NOT_VISITED)
            {
                tmp.x = v.x;
                tmp.y = v.y + 2;
                push(game, tmp);
            }
        }
    }
    return -1;
}

void push(game_data *game, matrix_point vertex)
{
    game->stack.stack[game->stack.index] = vertex;
    game->stack.index++;
}

matrix_point pop(game_data *game)
{
    matrix_point vertex;
    game->stack.index--;
    vertex = game->stack.stack[game->stack.index];
    return vertex;
}