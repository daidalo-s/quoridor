#ifndef GRAPHICS_QUORRIDOR
#define GRAPHICS_QUORRIDOR

#include "../common.h"

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

// In theory we should be in the starting point

// x -> top.y
#define HORIZONTAL_WALL_X_OFFSET(x) (5 + ((SQUARE_SIDE + WALL_SIZE) * (x / 2)))
// x -> top.x
#define HORIZONTAL_WALL_Y_OFFSET(x) ((5 + SQUARE_SIDE + 3) * ((x / 2) + 1))
// x -> top.y
#define VERTICAL_WALL_X_OFFSET(x) ((5 + SQUARE_SIDE + 3) * ((x / 2) + 1))
// x -> top.x
#define VERTICAL_WALL_Y_OFFSET(x) (5 + ((SQUARE_SIDE + WALL_SIZE) * (x / 2)))

#define WALL_LENGTH ((SQUARE_SIDE * 2) + 8)

/**
 * @brief This function simply draws the game board
 * OK
 */
void draw_board(void);

/**
 * @brief Given the active_player from game->player_turn, it draws
 * the corresponding token at the x-y passed as input
 */
void draw_player_token(ui8, ui8, active_player);

/**
 * @brief Given the two coordinates, clears the cell background with
 * black (== empty cell)
 */
void delete_player_token(ui8, ui8);

/**
 * @brief This function given the two new coordinates of the
 * active_player player, it draws the right token in the new
 * position
 */
void update_player_token_pos(ui8, ui8);

/**
 * @brief This function draws the cell at the coordinates x-y as
 * an available move cell
 */
void draw_available_move(ui8, ui8);

/**
 * @brief This function clears the cell at the coordinates x-y passed
 * in input with a black background
 */
void clear_available_move(ui8, ui8);

/**
 * @brief show_available_moves loops in the possible_moves array and
 * draws all of them using draw_available_move
 */
void show_available_moves(void);

/**
 * @brief This function deletes all the available moves drawn on the
 * board given an active_player
 */
void delete_available_moves(void);

/**
 * @brief This function updates the number of seconds remaining for
 * the move on screen
 */
void ticks_counter_update(ui8);

/**
 * @brief This function updates the number of residual walls for
 * player_1
 */
void p1_walls_update(ui8);

/**
 * @brief This function updates the number of residual walls for
 * player_2
 */
void p2_walls_update(ui8);

/**
 * @brief This function clears the text_area below the board
 */
void clear_text_area(void);

/**
 * @brief When the game ends, this function shows the winner message
 */
void show_winner_message(player);

/**
 * @brief This function draws the current wall at the coordinates
 * contained inside the struct
 */
void draw_current_wall(void);

/**
 * @brief This function deletes the current wall considering what
 * is present on the board already
 */
void delete_current_wall(void);

/**
 * @brief This if a helper function that returns the screen_coordinates
 * of the top cell of the current wall
 * @return screen_point
 */
screen_point get_current_wall_screen_coordinates(void);

void p1_no_more_walls(void);
void p2_no_more_walls(void);
void p1_illegal_wall(void);
void p2_illegal_wall(void);

#endif
