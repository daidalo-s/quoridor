#ifndef WALLS_H
#define WALLS_H

#include "./quoridor.h"

// inside here all the functions related to the walls management

/**
 * @brief It inits the current wall in the middle of the
 * screen and draws it
 */
void current_wall_init(void);

/**
 * @brief This function deletes the current wall when the time runs
 * out
 */
void current_wall_move(void);

void move_wall(wall_move_type);

void confirm_wall_placement(void);

// Below all functions related to the legal wall placements
int legal_wall_placement(game_data *);
void cells_not_visited(game_data *);
int feasible_path(game_data *, active_player);
void push(game_data *, matrix_point);
matrix_point pop(game_data *);

void find_all_possible_walls(void);
#endif