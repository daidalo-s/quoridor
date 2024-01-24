#include "./walls.h"
#include "../graphics/graphics.h"

extern game_data game;

// inside here all the functions related to the walls management

/**
 * @brief In here we init the current wall with the position and the orientation
 * and we draw it
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
 * @brief In this function we simply delete the current wall at the
 * coordinates inside the struct IF THERE IS NO WALL ALREADY PLACED
 * THERE
 */
void current_wall_move(void)
{
    delete_current_wall();
    // between the two operations we need to update the coordinates
    draw_current_wall();
}

// Inside of each move we have to check if it's feasible
// The only check we need to do is on the middle cell
// We also need to check if in the destination there is already a wall
// We must check that both top and bottom are in a free cell
void move_wall(wall_move_type move)
{

    int top_x = game.current_wall.top.x;
    int top_y = game.current_wall.top.y;
    int middle_x = game.current_wall.middle.x;
    int middle_y = game.current_wall.middle.y;
    int bottom_x = game.current_wall.bottom.x;
    int bottom_y = game.current_wall.bottom.y;

    if (move == WALL_UP)
    {
        // moving up means decreasing x
        // we need to add the cell check both for top and bottom to be free
        if (!(middle_x - 2 < 1))
        {
            // if we are still here the move is legal, we can assign the new coordinates
            delete_current_wall();
            game.current_wall.top.x = top_x - 2;
            game.current_wall.middle.x = middle_x - 2;
            game.current_wall.bottom.x = bottom_x - 2;
            draw_current_wall();
        }
    }
    else if (move == WALL_DOWN)
    {
        // moving down means increasing x
        if (!(middle_x + 2 > 11))
        {
            // The move is legal
            delete_current_wall();
            game.current_wall.top.x = top_x + 2;
            game.current_wall.middle.x = middle_x + 2;
            game.current_wall.bottom.x = bottom_x + 2;
            draw_current_wall();
        }
        // update_wall_drawing(WALL_DOWN);
    }
    else if (move == WALL_LEFT)
    {
        // moving left means decreasing y
        if (!(middle_y - 2 < 1))
        {
            // The move is legal
            delete_current_wall();
            game.current_wall.top.y = top_y - 2;
            game.current_wall.middle.y = middle_y - 2;
            game.current_wall.bottom.y = bottom_y - 2;
            draw_current_wall();
        }
    }
    else if (move == WALL_RIGHT)
    {
        // moving right means increasing y
        if (!(middle_y + 2 > 11))
        {
            // The move is legal
            delete_current_wall();
            game.current_wall.top.y = top_y + 2;
            game.current_wall.middle.y = middle_y + 2;
            game.current_wall.bottom.y = bottom_y + 2;
            draw_current_wall();
        }
        // update_wall_drawing(WALL_RIGHT);
    }
    else
    {
        // Rotation
        // Non c'è alcuna illegalità nella rotazione
        // middle remains the same
        // Depending from the orientation, we'll need to do different things
        // se orizzontale hanno la stessa x
        if (game.current_wall.wall_orientation == HORIZONTAL)
        {
            // The wall is horizontal, rotating to vertical
            delete_current_wall();
            game.current_wall.top.x = top_x - 1;
            game.current_wall.top.y = top_y + 1;
            game.current_wall.bottom.x = bottom_x + 1;
            game.current_wall.bottom.y = bottom_y - 1;
            game.current_wall.wall_orientation = VERTICAL;
            draw_current_wall();
        }
        else
        {
            // The wall is vertical, rotating to horizontal
            delete_current_wall();
            game.current_wall.top.x = top_x + 1;
            game.current_wall.top.y = top_y - 1;
            game.current_wall.bottom.x = bottom_x - 1;
            game.current_wall.bottom.y = bottom_y + 1;
            game.current_wall.wall_orientation = HORIZONTAL;
            draw_current_wall();
        }
    }
    return;
}

ui8 confirm_wall_placement(void)
{
    ui8 success = 0;
    if (game.board[game.current_wall.top.x][game.current_wall.top.y].availability == OCCUPIED ||
        game.board[game.current_wall.middle.x][game.current_wall.middle.y].availability == OCCUPIED ||
        game.board[game.current_wall.bottom.x][game.current_wall.bottom.y].availability == OCCUPIED)
    {
        game.player_turn == PLAYER_1 ? (p1_illegal_wall()) : (p2_illegal_wall());
        return success;
    }
    if (game.player_turn == PLAYER_1)
    {
        if (game.player_1.available_walls != 0)
        {
            place_current_wall();
            success = legal_wall_placement(&game);
            if (success)
            {
                game.player_1.available_walls--;
                p1_walls_update(game.player_1.available_walls);
                return success;
            }
            // if it's not legal we need to show the message
            remove_current_wall();
            p1_illegal_wall();
        }
        else
        {
            p1_no_more_walls();
        }
        // if he doesn't have any more walls, we need to show the message
    }
    else
    {
        if (game.player_2.available_walls != 0)
        {
            place_current_wall();
            success = legal_wall_placement(&game);
            if (success)
            {
                game.player_2.available_walls--;
                p2_walls_update(game.player_2.available_walls);
                return success;
            }
            remove_current_wall();
            p2_illegal_wall();
        }
        else
        {
            p2_no_more_walls();
        }
    }
    return success;
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
    bfs_node node = bfs(active_player);
    if (terminal_node(node, active_player))
        return 1;
    return 0;
}

bfs_node bfs(active_player player)
{
    // init
    bfs_node node;
    int x, y, length;
    node.length = 0;
    game.bfs_data.start = 0;
    game.bfs_data.end = 0;
    cells_not_visited(&game);

    if (player == PLAYER_1)
    {
        node.matrix_point.x = game.player_1.x_matrix_coordinate;
        node.matrix_point.y = game.player_1.y_matrix_coordinate;
    }
    else
    {
        node.matrix_point.x = game.player_2.x_matrix_coordinate;
        node.matrix_point.y = game.player_2.y_matrix_coordinate;
    }

    enqueue(node);

    while (game.bfs_data.start < game.bfs_data.end)
    {
        node = dequeue();
        if (terminal_node(node, player))
        {
            return node;
        }
        x = node.matrix_point.x;
        y = node.matrix_point.y;
        length = node.length;
        // for each subnode facciamo la enqueue

        if (!(x - 2 < 0 || game.board[x - 1][y].availability == OCCUPIED))
        {
            // going up is legal
            if (game.board[x - 2][y].availability == OCCUPIED)
            {
                // The opponent is facing us, we need to check if we can jump him
                if (!(x - 4 < 0 || game.board[x - 3][y].availability == OCCUPIED))
                {
                    add_bfs_node(x - 4, y, length);
                }
                else
                {
                    // checking if we can jump him right
                    if (!(y + 2 > 12 || game.board[x - 2][y + 1].availability == OCCUPIED))
                        add_bfs_node(x - 2, y + 2, length);
                    // checking if we can jump him left
                    if (!(y - 2 < 0 || game.board[x - 2][y - 1].availability == OCCUPIED))
                        add_bfs_node(x - 2, y - 2, length);
                }
            }
            else
            {
                add_bfs_node(x - 2, y, length);
            }
        }

        // DOWN
        // checking that the move is legal, now we are moving down so increasing X
        if (!(x + 2 > 12 || game.board[x + 1][y].availability == OCCUPIED))
        {
            // going down is legal
            if (game.board[x + 2][y].availability == OCCUPIED)
            {
                if (!(x + 4 > 12 || game.board[x + 3][y].availability == OCCUPIED))
                {
                    add_bfs_node(x + 4, y, length);
                }
                else
                {
                    // checking if we can jump him right
                    if (!(y + 2 > 12 || game.board[x + 2][y + 1].availability == OCCUPIED))
                        add_bfs_node(x + 2, y + 2, length);
                    // checking if we can jump him left
                    if (!(y - 2 < 0 || game.board[x + 2][y - 1].availability == OCCUPIED))
                        add_bfs_node(x + 2, y - 2, length);
                }
            }
            else
            {
                add_bfs_node(x + 2, y, length);
            }
        }
        // LEFT
        // checking that the move is legal, we are decrementing y
        if (!(y - 2 < 0 || game.board[x][y - 1].availability == OCCUPIED))
        {
            // going left is legal
            if (game.board[x][y - 2].availability == OCCUPIED)
            {
                if (!(y - 4 < 0 || game.board[x][y - 3].availability == OCCUPIED))
                {
                    add_bfs_node(x, y - 4, length);
                }
                else
                {
                    // checking if we can jump him right -> in this case means up
                    if (!(x - 2 < 0 || game.board[x - 1][y - 2].availability == OCCUPIED))
                        add_bfs_node(x - 2, y - 2, length);
                    // checking if we can jump him left -> in this case means down
                    if (!(x + 2 > 12 || game.board[x + 1][y - 2].availability == OCCUPIED))
                        add_bfs_node(x + 2, y - 2, length);
                }
            }
            else
            {
                add_bfs_node(x, y - 2, length);
            }
        }
        // RIGHT
        if (!(y + 2 > 12 || game.board[x][y + 1].availability == OCCUPIED))
        {
            // going right is legal
            if (game.board[x][y + 2].availability == OCCUPIED)
            {
                if (!(y + 4 > 12 || game.board[x][y + 3].availability == OCCUPIED))
                {
                    add_bfs_node(x, y + 4, length);
                }
                else
                {
                    // checking if we can jump him right -> in this case means up
                    if (!(x - 2 < 0 || game.board[x - 1][y + 2].availability == OCCUPIED))
                        add_bfs_node(x - 2, y + 2, length);
                    // checking if we can jump him left -> in this case means down
                    if (!(x + 2 > 12 || game.board[x + 1][y + 2].availability == OCCUPIED))
                        add_bfs_node(x + 2, y + 2, length);
                }
            }
            else
            {
                add_bfs_node(x, y + 2, length);
            }
        }
    }
    // if we get here it means that there is no passage
    return node;
}

void enqueue(bfs_node node)
{
    // push
    game.board[node.matrix_point.x][node.matrix_point.y].cell_status = VISITED;
    game.bfs_data.bfs_node_array[game.bfs_data.end] = node;
    game.bfs_data.end++;
}

bfs_node dequeue(void)
{
    bfs_node node;
    node = game.bfs_data.bfs_node_array[game.bfs_data.start];
    game.bfs_data.start++;
    return node;
}

ui8 terminal_node(bfs_node node, active_player player)
{
    if (player == PLAYER_1)
    {
        if (node.matrix_point.x == 12)
            return 1;
        return 0;
    }
    else
    {
        if (node.matrix_point.x == 0)
            return 1;
        return 0;
    }
}

void add_bfs_node(ui8 x, ui8 y, int length)
{
    bfs_node node;
    if (game.board[x][y].cell_status == NOT_VISITED)
    {
        node.length = length + 1;
        node.matrix_point.x = x;
        node.matrix_point.y = y;
        enqueue(node);
    }
}

void place_current_wall(void)
{
    game.board[game.current_wall.top.x][game.current_wall.top.y].availability = OCCUPIED;
    game.board[game.current_wall.middle.x][game.current_wall.middle.y].availability = OCCUPIED;
    game.board[game.current_wall.bottom.x][game.current_wall.bottom.y].availability = OCCUPIED;
}

void remove_current_wall(void)
{
    game.board[game.current_wall.top.x][game.current_wall.top.y].availability = FREE;
    game.board[game.current_wall.middle.x][game.current_wall.middle.y].availability = FREE;
    game.board[game.current_wall.bottom.x][game.current_wall.bottom.y].availability = FREE;
}

void find_all_possible_walls()
{
    ui8 i, j;
    // wall current_wall;
    game.wall_moves.num_of_moves = 0;
    // prima tutti gli orizzontali
    for (i = 1; i < 12; i += 2)
    {
        for (j = 0; j < 11; j += 2)
        {
            if (game.board[i][j].availability == FREE && game.board[i][j + 1].availability == FREE && game.board[i][j + 2].availability == FREE)
            {
                // we can add the wall
                game.current_wall.top.x = i;
                game.current_wall.top.y = j;
                game.current_wall.middle.x = i;
                game.current_wall.middle.y = j + 1;
                game.current_wall.bottom.x = i;
                game.current_wall.bottom.y = j + 2;
                game.current_wall.wall_orientation = HORIZONTAL;
                place_current_wall();
                if (feasible_path(&game, PLAYER_1) && feasible_path(&game, PLAYER_2))
                {
                    game.wall_moves.all_possible_walls[game.wall_moves.num_of_moves] = game.current_wall;
                    game.wall_moves.num_of_moves++;
                }

                remove_current_wall();
            }
        }
    }
    // poi i verticali
    for (i = 0; i < 11; i += 2)
    {
        for (j = 1; j < 12; j += 2)
        {
            if (game.board[i][j].availability == FREE && game.board[i + 1][j].availability == FREE && game.board[i + 2][j].availability == FREE)
            {
                game.current_wall.top.x = i;
                game.current_wall.top.y = j;
                game.current_wall.middle.x = i + 1;
                game.current_wall.middle.y = j;
                game.current_wall.bottom.x = i + 2;
                game.current_wall.bottom.y = j;
                game.current_wall.wall_orientation = VERTICAL;
                place_current_wall();
                if (feasible_path(&game, PLAYER_1) && feasible_path(&game, PLAYER_2))
                {
                    game.wall_moves.all_possible_walls[game.wall_moves.num_of_moves] = game.current_wall;
                    game.wall_moves.num_of_moves++;
                }
                remove_current_wall();
            }
        }
    }
}