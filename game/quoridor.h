#ifndef QUORIDOR_H
#define QUORIDOR_H

#include <stdint.h>
/**
 * @brief inside this file there are the prototypes of all the functions used in the
 * game and also all the type definitions
 */

#define ui8 uint8_t
#define ui16 uint16_t
#define NUM_MOVES 4

typedef enum show_or_hide
{
    SHOW,
    HIDE
} show_or_hide;

typedef enum cell_availability
{
    FREE,
    OCCUPIED
} cell_availability;

typedef enum cell_type
{
    WALL_CELL,
    BOARD_CELL
} cell_type;

typedef enum player_color
{
    WHITE, // White = player_1
    RED    // Red = player_2
} player_color;

typedef enum active_player
{
    PLAYER_1,
    PLAYER_2
} active_player;

typedef enum game_status
{
    WAIT_MODE,
    RUNNING,
    OVER
} game_status;

typedef enum move_type
{
    UP,
    DOWN,
    LEFT,
    RIGHT
} move_type;

typedef enum wall_move_type
{
    WALL_ROTATION,
    WALL_UP,
    WALL_DOWN,
    WALL_LEFT,
    WALL_RIGHT
} wall_move_type;

typedef enum input_mode
{
    WALL_MODE,
    PLAYER_MOVEMENT
} input_mode;

typedef enum cell_status
{
    NOT_VISITED,
    VISITED
} cell_status;

typedef struct game_cell
{
    cell_type cell;
    cell_availability availability;
    cell_status cell_status;
} game_cell;

typedef struct matrix_point
{
    uint8_t x;
    uint8_t y;
} matrix_point;

typedef struct player
{
    player_color player_color;
    ui8 x_matrix_coordinate;
    ui8 y_matrix_coordinate;
    // used inside move_player, when pressing
    // select the position is finalized
    ui8 tmp_x_matrix_coordinate;
    ui8 tmp_y_matrix_coordinate;
    ui8 available_walls;
    // up - down - left - right
    matrix_point possible_moves[NUM_MOVES];
    ui8 num_of_possible_moves;
} player;

typedef struct screen_point
{
    ui16 x;
    ui16 y;
} screen_point;

typedef struct wall
{
    matrix_point top;
    matrix_point middle;
    matrix_point bottom;
} wall;

typedef struct stack
{
    matrix_point stack[49];
    int index;
} stack;

typedef struct game_data
{
    /* data */
    game_cell board[13][13];
    player player_1;
    player player_2;
    active_player player_turn;
    game_status game_status;
    uint32_t game_tick;
    input_mode input_mode;
    wall current_wall;
    stack stack;
    uint32_t last_move;
} game_data;

// Functions prototypes
void game_init(game_data *);
void game_run(void);
void init_players(game_data *);
void game_start(game_data *);
void move_player(move_type move, game_data *);
void swap_turn(game_data *, uint8_t);
void game_over(game_data *, player);
void save_last_move(game_data *);
void board_cell_type_init(game_data *);
void turn_manager(game_data *);
#endif
