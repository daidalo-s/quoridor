#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

#define ui8 uint8_t
#define ui16 uint16_t
#define NUM_MOVES 5

typedef enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    ROTATION,
    DIAG_UP_RIGHT,
    DIAG_UP_LEFT,
    DIAG_DOWN_RIGHT,
    DIAG_DOWN_LEFT
} move_type;

typedef enum
{
    HORIZONTAL,
    VERTICAL,
    NONE
} wall_orientation;

typedef enum
{
    SHOW,
    HIDE
} show_or_hide;

typedef enum
{
    FREE,
    OCCUPIED
} cell_availability;

typedef enum
{
    WALL_CELL,
    BOARD_CELL
} cell_type;

typedef enum
{
    WHITE, // White = player_1
    RED    // Red = player_2
} player_color;

typedef enum
{
    PLAYER_1,
    PLAYER_2
} active_player;

typedef enum
{
    WAIT_MODE,
    RUNNING,
    OVER,
    MENU_MODE
} game_status;

typedef enum
{
    WALL_ROTATION,
    WALL_UP,
    WALL_DOWN,
    WALL_LEFT,
    WALL_RIGHT
} wall_move_type;

typedef enum
{
    WALL_MODE,
    PLAYER_MOVEMENT
} input_mode;

typedef enum
{
    NOT_VISITED,
    VISITED
} cell_status;

typedef struct
{
    cell_type cell;
    cell_availability availability;
    cell_status cell_status;
} game_cell;

typedef struct
{
    uint8_t x;
    uint8_t y;
} matrix_point;

typedef struct
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

typedef struct
{
    matrix_point top;
    matrix_point middle;
    matrix_point bottom;
    wall_orientation wall_orientation;
} wall;

typedef struct
{
    matrix_point stack[49];
    int index;
} stack;

typedef struct
{
    wall all_possible_walls[200];
    ui8 num_of_moves;
} wall_moves;

typedef enum
{
    CLEAR,
    FULL
} text_area_status;

typedef struct
{
    matrix_point matrix_point;
    ui8 length;
    ui8 success;
} bfs_node;

typedef struct
{
    ui8 start;
    ui8 end;
    bfs_node bfs_node_array[80];
} bfs_data;

typedef struct
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
    wall_moves wall_moves;
    text_area_status text_area_status;
    bfs_data bfs_data;
} game_data;

typedef struct
{
    ui16 x;
    ui16 y;
} screen_point;

typedef struct
{
    ui8 x;
    ui8 y;
    int value;
    wall_orientation wall_orientation;
} minimax_res;

typedef enum
{
    MENU_INT0,
    GAME_MODE_SELECT,
    SINGLE_BOARD,
    MULTI_BOARD
} menu_window;

typedef struct
{
    menu_window menu_window;
    ui8 option_num;
} menu;

#endif