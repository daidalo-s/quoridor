#ifndef PLAYER_H
#define PLAYER_H

#include "quoridor.h"

// inside here all the functions related to the players management
void init_players(game_data *game);
void find_available_moves(game_data *);
void add_move(game_data *, matrix_point);
#endif