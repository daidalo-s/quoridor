#ifndef PLAYER_H
#define PLAYER_H

#include "quoridor.h"

// inside here all the functions related to the players management
void init_players(game_data *game);
void find_available_moves(game_data *);
void add_move(game_data *, matrix_point);

/**
 * @brief This function is called by the RIT IRQ when a joystick
 * move is detected
 */
void move_player(move_type, game_data *, active_player);
#endif