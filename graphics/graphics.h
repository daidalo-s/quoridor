#ifndef GRAPHICS_QUORRIDOR
#define GRAPHICS_QUORRIDOR

#include "../game/quoridor.h"

#define SQUARE_SIDE 26
#define WALL_SIZE 8
#define ITERATION_OFFSET (SQUARE_SIDE + WALL_SIZE)
// in teoria è 235+15 = 240
#define X_RECTANGLE_AREA_BEGIN (6 * ITERATION_OFFSET + SQUARE_SIDE + 15)
#define X_RECTANGLE_AREA_END (320 - 10)

// 6 + 1 + 4 + 28
#define P1_WALLS_X 39
// X_RECTANGLE_AREA_BEGIN + 1 + 18 + 16 + 4
#define P1_WALLS_Y (X_RECTANGLE_AREA_BEGIN + 39)

//  6 + (2 * (72 + 6)) + 1 + 4 + 28
#define P2_WALLS_X 195
// X_RECTANGLE_AREA_BEGIN + 1 + 18 + 16 + 4
#define P2_WALLS_Y (X_RECTANGLE_AREA_BEGIN + 39)

// 6 + (1 * (72 + 6)) + 24
#define X_TICKS 108
// X_RECTANGLE_AREA_BEGIN + 1 + 27
#define Y_TICKS (X_RECTANGLE_AREA_BEGIN + 28)

#define WALLS_ITERATION_OFFSET (SQUARE_SIDE + 3)

#define Y_DATA_AREA 280
#define HEIGHT_DATA_AREA 35
#define WIDTH_DATA_AREA 230

#define X_P1_TEXT_AREA_BEGIN 10
#define Y_P1_TEXT_AREA_BEGIN (Y_DATA_AREA + 9)

// (10 * 8) + 18
#define X_TIME_AREA_BEGIN (X_P1_TEXT_AREA_BEGIN + 98)

// 3 * 8 + 18
#define X_P2_TEXT_AREA_BEGIN (X_TIME_AREA_BEGIN + 42)

// Below there are all the indexes for updating the data area
// 9 * 8
#define P1_WALLS_NUMBER_X (X_P1_TEXT_AREA_BEGIN + 72)
#define P1_WALLS_NUMBER_Y Y_P1_TEXT_AREA_BEGIN

#define TICKS_NUMBER_X X_TIME_AREA_BEGIN
#define TICKS_NUMBER_Y Y_P1_TEXT_AREA_BEGIN

#define P2_WALLS_NUMBER_X (X_P2_TEXT_AREA_BEGIN + 72)
#define P2_WALLS_NUMBER_Y Y_P1_TEXT_AREA_BEGIN

#define X_MESSAGE_AREA 5
#define Y_MESSAGE_AREA 240

typedef enum wall_orientation
{
    VERTICAL,
    HORIZONTAL
} wall_orientation;

void draw_board(void);
void update_player_token_pos(uint8_t, uint8_t, uint8_t, uint8_t, active_player);
void draw_available_move(ui8, ui8);
void clear_available_move(uint8_t, uint8_t);
void ticks_counter_update(uint8_t);
void p1_walls_update(uint8_t);
void p2_walls_update(uint8_t);
void draw_player_token(ui8, ui8, active_player);
void delete_player_token(ui8, ui8, active_player);
// TODO: da qua in poi da fixare
void draw_walls(void);
void update_wall_drawing(wall_move_type);
void clear_wall(screen_point, wall_orientation);
void show_winner_message(player);
void clear_text_area(void);
void show_available_moves(game_data *);
#endif
