#ifndef MAGDA
#define MAGDA

#include "../game/quoridor.h"

#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))

typedef struct minimax_res
{
    ui8 x;
    ui8 y;
    int value;
    wall_orientation wall_orientation;
} minimax_res;

minimax_res minimax(ui8, ui8);
int heuristic_value_of_node(void);
void minimax_move_player(ui8, ui8, ui8, active_player);
void minimax_unmove_player(ui8, ui8, active_player);
void minimax_place_wall(ui8, active_player);
void minimax_remove_wall(ui8, active_player);

#endif