/**
 * @file quoridor.c
 * @author your name (you@domain.com)
 * @brief inside this file there is all the game logic
 * @version 0.1
 * @date 2024-01-10
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "quoridor.h"
#include "../timer/timer.h"
#include "lpc17xx.h"
#include "../graphics/graphics.h"
#include "../GLCD/GLCD.h"
#include "./player.h"
#include "./walls.h"
#include "../magda/magda.h"
#include "../menu/menu.h"
#include "../CAN/can_manager.h"

extern game_data game;
extern can_move received_move;

/**
 * @brief This function is responsible of starting the game.
 * If mode = 1 then player_1 will be controlled by the bot
 * @param game
 */
void game_start(game_data *game)
{
    board_cell_type_init(game);
    // Drawing the game board
    draw_board();
    // Initializing player data
    init_players(game);
    /**
     * This timer is the timer that will count the game time.
     * It's setted to 1s (25MHz*1sec).
     * The timer needs to reset and generate ad interrupt upon count, so SRI -> 011 = 3
     */
    init_timer(0, 0, 0, 3, 0x17D7840);
    draw_player_token(game->player_1.x_matrix_coordinate, game->player_1.y_matrix_coordinate, PLAYER_1);
    draw_player_token(game->player_2.x_matrix_coordinate, game->player_2.y_matrix_coordinate, PLAYER_2);
    // 0 since the time is not over
    turn_manager(game, 0);
}

void game_over(game_data *game, player winner)
{
    disable_timer(0);
    game->game_status = OVER;
    show_winner_message(winner);
}

/**
 * @brief This function initializes each cell of the board setting the right
 * cell type depending from the position
 * @param game
 */
void board_cell_type_init(game_data *game)
{
    int i = 0;
    int j = 0;
    // Initializing the board with the correct cell type
    for (i = 0; i < 13; i++)
    {
        for (j = 0; j < 13; j++)
        {
            // first of all we need to know if we are looking at a row with also
            // board cells or only walls
            if (i % 2 == 0)
            {
                // if we are here it means that this will be an "hybrid row"
                // a board cell is always at a even index
                if (j % 2 == 0)
                {
                    // We are looking at a board cell
                    game->board[i][j].cell = BOARD_CELL;
                    game->board[i][j].availability = FREE;
                }
                else
                {
                    // We are looking at a wall cell
                    game->board[i][j].cell = WALL_CELL;
                    game->board[i][j].availability = FREE;
                }
            }
            else
            {
                // If we are here its a only walls row
                game->board[i][j].cell = WALL_CELL;
                game->board[i][j].availability = FREE;
            }
        }
    }
}

/**
 * @brief In here we manage the turns.
 * PLAYER_1 always starts first.
 * @param game -> the game instance
 * @param time_over -> a flag, 0 if it's a move, 1 if the time is over
 */
void turn_manager(game_data *game, ui8 time_over)
{
    ui8 success;
    if (game->game_status == WAIT_MODE)
    {
        // we need to start the game
        game->game_status = RUNNING;
        enable_timer(0);
        // The game starts with p1 playing
        if (game->game_mode == SINGLE_HUMAN_VS_HUMAN)
        {
            game->bot_manager.bot_mode_enabled = 0;
            enable_timer(0);
            p1_turn(game);
        }
        else if (game->game_mode == SINGLE_HUMAN_VS_NPC)
        {
            game->bot_manager.bot_mode_enabled = 0;
            game->bot_manager.player = PLAYER_1;
            enable_timer(0);
            p1_bot_turn(game);
        }
        else if (game->game_mode == MULTI_HUMAN)
        {
            /**
             * Se arrivo qua so che devo giocare come umano. Ma chi sono?
             */
            // are we player_1?
            if (game->multi_board_master)
            {
                // siamo noi a doverci muovere
                p1_turn(game);
            }
            else
            {
                // dobbiamo aspettare una mossa
                p1_remote_turn();
            }
        }
        else
        {
            // se sono qui so che io devo giocare come NPC
            // MULTI_NPC
            if (game->multi_board_master)
            {
                // siamo noi a doverci muovere
                p1_bot_turn(game);
            }
            else
            {
                // dobbiamo aspettare una mossa
                p1_remote_turn();
            }
        }
    }
    else if (game->game_status == RUNNING)
    {
        // The time run out
        if (time_over)
        {
            if (game->input_mode == PLAYER_MOVEMENT)
            {
                // the current player was trying to move itself
                if (game->game_mode == SINGLE_HUMAN_VS_HUMAN)
                {
                    if (game->player_turn == PLAYER_1)
                    {
                        reset_p1_token();
                        save_last_move(game);
                        p2_turn(game);
                    }
                    else
                    {
                        reset_p2_token();
                        save_last_move(game);
                        p1_turn(game);
                    }
                }
                else if (game->game_mode == SINGLE_HUMAN_VS_NPC)
                {
                    reset_p2_token();
                    save_last_move(game);
                    p1_bot_turn(game);
                }
                else if (game->game_mode == MULTI_HUMAN)
                {
                    // qui io gioco come umano e l'altro stronzo non lo so
                    // o sono il master e quindi anche player_1 oppure sono player 2
                    if (game->multi_board_master)
                    {
                        // so che io sono player_1
                        if (game->player_turn == PLAYER_1)
                        {
                            send_dummy_move();
                            reset_p1_token();
                            p2_remote_turn();
                        }
                        else
                        {
                            // era il turno del bastardo che non ha mandato niente
                            disable_timer(0);
                            // TODO non lo so rick
                            // devo solamente aspettare la sua move, speriamo arrivi
                        }
                    }
                    else
                    {
                        // io non sono il master -> sono player_2
                        if (game->player_turn == PLAYER_2)
                        {
                            // minchia era il mio turno e non ho fatot niente
                            send_dummy_move();
                            reset_p2_token();
                            p1_remote_turn();
                        }
                        else
                        {
                            // era il turno del bastardo ma non mi ha ancora mandato nulla
                            disable_timer(0);
                            // TODO non lo so rick
                        }
                    }
                }
                else
                {
                    // TODO
                    // siamo con due board ma io sono un bot, non arriverò mai qua
                }
            }
            else
            {
                // the player was trying to place a wall
                if (game->game_mode == SINGLE_HUMAN_VS_HUMAN)
                {
                    if (game->player_turn == PLAYER_1)
                    {
                        delete_current_wall();
                        save_last_move(game);
                        p2_turn(game);
                    }
                    else
                    {
                        delete_current_wall();
                        save_last_move(game);
                        p1_turn(game);
                    }
                }
                else if (game->game_mode == SINGLE_HUMAN_VS_NPC)
                {
                    // player_1 is the npc but it's impossible his time run out
                    // TODO: it's impossible to get here, in the NPC we trust
                    // -> if we arrive here the player_2 didn't place the wall in time
                    delete_current_wall();
                    save_last_move(game);
                    p1_bot_turn(game);
                }
                else if (game->game_mode == MULTI_HUMAN)
                {
                    // TODO
                    // the current player confirmed a mvoe
                }
                else
                {
                    // TODO
                }
            }
        }
        else
        {
            // There is a go for the move
            if (game->input_mode == PLAYER_MOVEMENT)
            {
                if (game->game_mode == SINGLE_HUMAN_VS_HUMAN)
                {
                    confirm_player_move(game);
                    if (game->game_status != OVER)
                    {
                        delete_available_moves();
                        if (game->player_turn == PLAYER_1)
                        {
                            draw_player_token(game->player_1.x_matrix_coordinate, game->player_1.y_matrix_coordinate, PLAYER_1);
                            save_last_move(game);
                            p2_turn(game);
                        }
                        else
                        {
                            draw_player_token(game->player_2.x_matrix_coordinate, game->player_2.y_matrix_coordinate, PLAYER_2);
                            save_last_move(game);
                            p1_turn(game);
                        }
                    }
                }
                else if (game->game_mode == SINGLE_HUMAN_VS_NPC)
                {
                    // player_1 is the npc but it's impossible his time run out
                    if (game->player_turn == PLAYER_1)
                    {
                        // il bot ha appena finito
                        p2_turn(game);
                    }
                    else
                    {
                        // p2 ha confermato una mossa
                        confirm_player_move(game);
                        save_last_move(game);
                        if (game->game_status != OVER)
                        {
                            delete_available_moves();
                            draw_player_token(game->player_2.x_matrix_coordinate, game->player_2.y_matrix_coordinate, PLAYER_2);
                            p1_bot_turn(game);
                        }
                    }
                }
                else if (game->game_mode == MULTI_HUMAN)
                {
                    // ci sono due schede, io gioco come umano, ho confermato una mossa o ne è arrivata una
                    if (game->multi_board_master && game->player_turn == PLAYER_1)
                    {
                        // siamo noi che abbiamo confermato una mossa
                        confirm_player_move(game);
                        if (game->game_status != OVER)
                        {
                            delete_available_moves();
                            draw_player_token(game->player_1.x_matrix_coordinate, game->player_1.y_matrix_coordinate, PLAYER_1);
                            save_last_move(game);
                            p2_turn(game);
                            save_last_move(game);
                            return;
                        }
                        save_last_move(game);
                        send_last_move();
                        return;
                    }
                    else if (game->multi_board_master && game->player_turn == PLAYER_2_REMOTE)
                    {
                        // se sono master ma il turno è dell'altro bastardo mi deve arrivare una mossa
                        // e devo cambiare turno
                        // TODO: cambia turno dentro la show remote move
                        show_remote_move();
                    }
                    else if (!game->multi_board_master && game->player_turn == PLAYER_2)
                    {
                        // io sono player_2 e toccava a me, è una mia mossa
                        confirm_player_move(game);
                        if (game->game_status != OVER)
                        {
                            delete_available_moves();
                            draw_player_token(game->player_1.x_matrix_coordinate, game->player_1.y_matrix_coordinate, PLAYER_1);
                            save_last_move(game);
                            p2_turn(game);
                            save_last_move(game);
                            return;
                        }
                        save_last_move(game);
                        send_last_move();
                        return;
                    }
                    else if (!game->multi_board_master && game->player_turn == PLAYER_1_REMOTE)
                    {
                        // TODO: cambia turno dentro la show remote move
                        show_remote_move();
                    }
                }
                else
                {
                    // TODO
                }
            }
            else
            {
                // the game mode is in wall placement
                // this code is common to both of them
                if (game->game_mode == SINGLE_HUMAN_VS_HUMAN)
                {
                    success = confirm_wall_placement();
                    if (success)
                    {
                        if (game->player_turn == PLAYER_1)
                        {
                            save_last_move(game);
                            p2_turn(game);
                        }
                        else
                        {
                            save_last_move(game);
                            p1_turn(game);
                        }
                    }
                }
                else if (game->game_mode == SINGLE_HUMAN_VS_NPC)
                {
                    // il bot non deve chiedere il permesso a nessuno, arrivo qui solo se sono player_2
                    success = confirm_wall_placement();
                    if (success)
                    {
                        save_last_move(game);
                        p1_bot_turn(game);
                    }
                }
                else if (game->game_mode == MULTI_HUMAN)
                {
                    // io sono umano, toccava a me ed è una conferma del muro
                }
            }
        }
        return;
    }
}

void p1_turn(game_data *game)
{
    game->input_mode = PLAYER_MOVEMENT;
    game->player_turn = PLAYER_1;
    game->game_tick = 20;
    reset_timer(0);
    enable_timer(0);
    find_available_moves(game);
    show_available_moves();
    return;
}

void p2_turn(game_data *game)
{
    game->input_mode = PLAYER_MOVEMENT;
    game->player_turn = PLAYER_2;
    game->game_tick = 20;
    reset_timer(0);
    enable_timer(0);
    find_available_moves(game);
    show_available_moves();
    return;
}

void p1_remote_turn(void)
{
    // game.input_mode = PLAYER_MOVEMENT;
    game.player_turn = PLAYER_1_REMOTE;
    game.game_tick = 20;
    reset_timer(0);
    enable_timer(0);
}

void p2_remote_turn(void)
{
    // game.input_mode = PLAYER_MOVEMENT;
    game.player_turn = PLAYER_2_REMOTE;
    game.game_tick = 20;
    reset_timer(0);
    enable_timer(0);
}

/**
 * @brief Remember that the bot doesn't show anything, he just moves
 * @param game
 */
void p1_bot_turn(game_data *game)
{
    move move;
    game->input_mode = PLAYER_MOVEMENT;
    game->player_turn = PLAYER_1;
    game->game_tick = 20;
    game->player_1.bot = 1;
    ticks_counter_update(20);
    move = minimax(2);

    if (move.type_of_move.type == PLAYER)
    {
        // player_movement
        delete_player_token(game->player_1.x_matrix_coordinate, game->player_1.y_matrix_coordinate);
        game->player_1.tmp_x_matrix_coordinate = move.x;
        game->player_1.tmp_y_matrix_coordinate = move.y;
        confirm_player_move(game);
        save_last_move(game);
        draw_player_token(game->player_1.x_matrix_coordinate, game->player_1.y_matrix_coordinate, PLAYER_1);
    }
    else
    {
        // wall placement
        if (move.type_of_move.orientation == VERTICAL)
        {
            // muro verticale
            game->current_wall.top.x = move.x - 1;
            game->current_wall.top.y = move.y;
            game->current_wall.middle.x = move.x;
            game->current_wall.middle.y = move.y;
            game->current_wall.bottom.x = move.x + 1;
            game->current_wall.bottom.y = move.y;
            game->current_wall.wall_orientation = VERTICAL;
            place_current_wall();
            save_last_move(game);
            draw_current_wall();
            game->player_1.available_walls--;
            p1_walls_update(game->player_1.available_walls);
        }
        else
        {
            // muro orizzontale
            game->current_wall.top.x = move.x;
            game->current_wall.top.y = move.y - 1;
            game->current_wall.middle.x = move.x;
            game->current_wall.middle.y = move.y;
            game->current_wall.bottom.x = move.x;
            game->current_wall.bottom.y = move.y + 1;
            game->current_wall.wall_orientation = HORIZONTAL;
            place_current_wall();
            save_last_move(game);
            draw_current_wall();
            game->player_1.available_walls--;
            p1_walls_update(game->player_1.available_walls);
        }
    }
    game->input_mode = PLAYER_MOVEMENT;
    turn_manager(game, 0);
    return;
}

void p2_bot_turn(game_data *game)
{
    move move;
    game->input_mode = PLAYER_MOVEMENT;
    game->player_turn = PLAYER_2;
    game->game_tick = 20;
    game->player_2.bot = 1;
    ticks_counter_update(20);
    move = minimax(1);
    // move = minimax_alfa_beta(2, INT32_MIN, INT32_MAX);
    if (move.type_of_move.type == PLAYER)
    {
        // player_movement
        delete_player_token(game->player_2.x_matrix_coordinate, game->player_2.y_matrix_coordinate);
        game->player_2.tmp_x_matrix_coordinate = move.x;
        game->player_2.tmp_y_matrix_coordinate = move.y;
        confirm_player_move(game);
        draw_player_token(game->player_2.x_matrix_coordinate, game->player_2.y_matrix_coordinate, PLAYER_2);
    }
    else
    {
        // wall placement
        if (move.type_of_move.orientation == VERTICAL)
        {
            // muro verticale
            game->current_wall.top.x = move.x - 1;
            game->current_wall.top.y = move.y;
            game->current_wall.middle.x = move.x;
            game->current_wall.middle.y = move.y;
            game->current_wall.bottom.x = move.x + 1;
            game->current_wall.bottom.y = move.y;
            game->current_wall.wall_orientation = VERTICAL;
            place_current_wall();
            save_last_move(game);
            draw_current_wall();
            game->player_2.available_walls--;
            p2_walls_update(game->player_2.available_walls);
        }
        else
        {
            // muro orizzontale
            game->current_wall.top.x = move.x;
            game->current_wall.top.y = move.y - 1;
            game->current_wall.middle.x = move.x;
            game->current_wall.middle.y = move.y;
            game->current_wall.bottom.x = move.x;
            game->current_wall.bottom.y = move.y + 1;
            game->current_wall.wall_orientation = HORIZONTAL;
            place_current_wall();
            save_last_move(game);
            draw_current_wall();
            game->player_2.available_walls--;
            p2_walls_update(game->player_2.available_walls);
        }
    }
    game->input_mode = PLAYER_MOVEMENT;
    turn_manager(game, 0);
    return;
}

void reset_p1_token(void)
{
    delete_available_moves();
    delete_player_token(game.player_1.tmp_x_matrix_coordinate, game.player_1.tmp_y_matrix_coordinate);
    draw_player_token(game.player_1.x_matrix_coordinate, game.player_1.y_matrix_coordinate, PLAYER_1);
}

void reset_p2_token(void)
{
    delete_available_moves();
    delete_player_token(game.player_2.tmp_x_matrix_coordinate, game.player_2.tmp_y_matrix_coordinate);
    draw_player_token(game.player_2.x_matrix_coordinate, game.player_2.y_matrix_coordinate, PLAYER_2);
}

void confirm_player_move(game_data *game)
{
    if (game->player_turn == PLAYER_1)
    {
        // we need to confirm the player_1 move
        game->board[game->player_1.x_matrix_coordinate][game->player_1.y_matrix_coordinate].availability = FREE;
        game->player_1.x_matrix_coordinate = game->player_1.tmp_x_matrix_coordinate;
        game->player_1.y_matrix_coordinate = game->player_1.tmp_y_matrix_coordinate;
        game->board[game->player_1.x_matrix_coordinate][game->player_1.y_matrix_coordinate].availability = OCCUPIED;
        if (game->player_1.x_matrix_coordinate == 12)
        {
            game_over(game, game->player_1);
            return;
        }
    }
    else
    {
        game->board[game->player_2.x_matrix_coordinate][game->player_2.y_matrix_coordinate].availability = FREE;
        game->player_2.x_matrix_coordinate = game->player_2.tmp_x_matrix_coordinate;
        game->player_2.y_matrix_coordinate = game->player_2.tmp_y_matrix_coordinate;
        game->board[game->player_2.x_matrix_coordinate][game->player_2.y_matrix_coordinate].availability = OCCUPIED;
        if (game->player_2.x_matrix_coordinate == 0)
        {
            game_over(game, game->player_2);
            return;
        }
    }
}

void move_dispatcher(move_type direction, game_data *game)
{
    if (game->game_status == MENU_MODE)
    {
        menu_manager(direction);
        return;
    }

    if (game->game_status == RUNNING)
    {
        if (game->input_mode == PLAYER_MOVEMENT)
        {
            if (direction == DIAG_UP_RIGHT)
            {
                move_player(DIAG_UP_RIGHT, game, game->player_turn);
            }
            else if (direction == DIAG_UP_LEFT)
            {
                move_player(DIAG_UP_LEFT, game, game->player_turn);
            }
            else if (direction == DIAG_DOWN_RIGHT)
            {
                move_player(DIAG_DOWN_RIGHT, game, game->player_turn);
            }
            else if (direction == DIAG_DOWN_LEFT)
            {
                move_player(DIAG_DOWN_LEFT, game, game->player_turn);
            }
            else if (direction == UP)
            {
                move_player(UP, game, game->player_turn);
            }
            else if (direction == LEFT)
            {
                move_player(LEFT, game, game->player_turn);
            }
            else if (direction == RIGHT)
            {
                move_player(RIGHT, game, game->player_turn);
            }
            else if (direction == DOWN)
            {
                move_player(DOWN, game, game->player_turn);
            }
        }
        else
        {
            // Wall mode
            if (game->text_area_status == FULL)
                clear_text_area();
            if (direction == UP)
            {
                move_wall(WALL_UP);
            }
            else if (direction == LEFT)
            {
                move_wall(WALL_LEFT);
            }
            else if (direction == RIGHT)
            {
                move_wall(WALL_RIGHT);
            }
            else if (direction == DOWN)
            {
                // down
                move_wall(WALL_DOWN);
            }
            else
            {
                // rotation
                move_wall(WALL_ROTATION);
            }
        }
    }
}

/**
 * @brief We need to distinguish between wall mode and player mode and simply
 * call the function that handles it
 */
void select_button_pressed()
{
    if (game.game_status == MENU_MODE)
    {
        menu_manager(ROTATION);
        return;
    }
    turn_manager(&game, 0);
}

void key1_button_pressed()
{
    if (game.input_mode == PLAYER_MOVEMENT)
    {
        // call an appropriate function, we need to enter in wall mode
        wall_mode_enter();
        return;
    }
    if (game.input_mode == WALL_MODE)
    {
        if (game.text_area_status == FULL)
            clear_text_area();
        wall_mode_exit();
        return;
    }
}

void key2_button_pressed()
{
    if (game.input_mode == WALL_MODE)
    {
        // it's a wall rotation
        move_wall(WALL_ROTATION);
    }
}

/**
 * @brief We need to draw the current_wall
 */
void wall_mode_enter(void)
{
    // we undraw the available moves
    delete_available_moves();
    // reset the player token
    game.player_turn == PLAYER_1 ? (reset_p1_token()) : (reset_p2_token());
    // changing the game mode
    game.input_mode = WALL_MODE;
    // draw the current_wall wall
    current_wall_init();
}

/**
 * @brief We need to delete the current wall from the screen
 */
void wall_mode_exit(void)
{
    delete_current_wall();
    show_available_moves();
    game.input_mode = PLAYER_MOVEMENT;
}

/**
 * The structure is the following:
 * - player_id : 8 bits
 * - player_move / wall : 4 bits (0-player, 1-wall)
 * - vertical/horizontal : 4 bits (0 vertical, 1 horizontal)
 * - y player : 8 bits
 * - x player : 8 bits
 * @param game
 */
void save_last_move(game_data *game)
{
    ui8 tmp;
    // player id
    game->last_move = 0;
    tmp = game->player_turn;
    game->player_turn == PLAYER_1 ? (tmp = 0) : (tmp = 1);
    game->last_move |= tmp << 24;
    // player_move/wall
    if (game->input_mode == PLAYER_MOVEMENT)
    {
        // se era una move del player 8 bit a zero
        tmp = 0;
        game->last_move |= tmp << 16;
    }
    else
    {
        // abbiamo mosso un muro
        // 0 vertical, 1 horizontal
        if (game->current_wall.wall_orientation == HORIZONTAL)
        {
            // il muro era orizzontale
            tmp = 0x11;
            game->last_move |= tmp << 16;
        }
        else
        {
            // era verticale
            tmp = 0x10;
            game->last_move |= tmp << 16;
            // devo salvare le coordinate della middle
        }
    }
    if (game->input_mode == PLAYER_MOVEMENT)
    {
        // x e y del player
        if (game->player_turn == PLAYER_1)
        {
            tmp = game->player_1.y_matrix_coordinate;
            game->last_move |= tmp << 8;
            tmp = game->player_1.x_matrix_coordinate;
            game->last_move |= tmp;
        }
        else
        {
            tmp = game->player_2.y_matrix_coordinate;
            game->last_move |= tmp << 8;
            tmp = game->player_2.x_matrix_coordinate;
            game->last_move |= tmp;
        }
    }
    else
    {
        // x e y della middle del wall
        tmp = game->current_wall.middle.y;
        game->last_move |= tmp << 8;
        tmp = game->current_wall.middle.x;
        game->last_move |= tmp;
    }
}

void decode_last_move()
{
    // it's already done, the only thing we need to do is
    received_move.y <<= 1;
    received_move.x <<= 1;
    if (received_move.game_mode == 1)
    {
        received_move.y += 1;
        received_move.x += 1;
    }
    // now we can call the turn manager -> fuck the turn manager, we handle it
    // if we are here we received a move via CAN -> it must be an opponent move
    // TODO: be sure it can only be an opponent move
    // TODO: what if it's a dummy move?
    // se è una dummy è easy, mi prendo il turno e fine
    if (game.multi_board_master)
    {
        // we are the master -> the move is for player_2
        if (received_move.game_mode == 0)
        {
            // player_movement del player_2
            // i'm 100% sure he didn't move
            delete_player_token(game->player_2.x_matrix_coordinate, game->player_2.y_matrix_coordinate);
            game->player_2.x_matrix_coordinate = received_move.x;
            game->player_2.y_matrix_coordinate = received_move.y;
            draw_player_token(game->player_2.x_matrix_coordinate, game->player_2.y_matrix_coordinate, PLAYER_2);

            if (game->player_2.x_matrix_coordinate == 0)
            {
                game_over(&game, PLAYER_2);
            }
        }
        else
        {
            // it's a wall placement so we need to understand the orientation and than we have the middle
            if (received_move.orientation == 0)
            {
                // muro verticale
            }
            else
            {
                // muro orizzontale
            }
        }
    }
    else
    {
        // sono lo slave -> è una move per player_1
    }
}

void craft_dummy_move(void)
{
    ui8 tmp;
    // player id
    game->last_move = 0;
    tmp = game->player_turn;
    game->player_turn == PLAYER_1 ? (tmp = 0) : (tmp = 1);
    game->last_move |= tmp << 24;
    tmp = 0x01;
    game->last_move |= tmp << 16;
    tmp = 0x0000;
    game->last_move |= tmp;
    // send_last_move();
}

void show_remote_move(void)
{
    // dobbiamo mostrare a schermo la mossa remota e cambiare turno
}