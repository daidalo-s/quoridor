#ifndef WALLS_H
#define WALLS_H

#include "./quoridor.h"

// inside here all the functions related to the walls management
void move_wall(wall_move_type move, game_data *);
int legal_wall_placement(game_data *);
void cells_not_visited(game_data *);
int feasible_path(game_data *, active_player);
void push(game_data *, matrix_point);
matrix_point pop(game_data *);
void current_wall_init(void);
void delete_current_wall(void);
void find_all_possible_walls(void);
#endif