#ifndef QUORIDOR_H
#define QUORIDOR_H

#include "../common.h"

/**
 * @brief inside this file there are the prototypes of all the functions used in the
 * game and also all the type definitions
 */

// Functions prototypes
// TODO: check which of them are still here
void game_init(game_data *);
void game_run(void);
void init_players(game_data *);
void game_start(game_data *);
void swap_turn(game_data *, uint8_t);
void game_over(game_data *, player);
void save_last_move(game_data *);
void board_cell_type_init(game_data *);
void turn_manager(game_data *, ui8);
void p1_turn(game_data *);
void p2_turn(game_data *);
void confirm_player_move(game_data *);
void move_dispatcher(move_type, game_data *);
void select_button_pressed(void);
void key1_button_pressed(void);
void key2_button_pressed(void);
void reset_p1_token(void);
void reset_p2_token(void);
void wall_mode_enter(void);
void wall_mode_exit(void);
void p1_bot_turn(game_data *);
void p2_bot_turn(game_data *);
#endif
