/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
// #include "../adc/adc.h"
#include "../led/led.h"
#include "../timer/timer.h"
#include "../../common.h"
#include "../game/quoridor.h"
#include "../graphics/graphics.h"
#include "../GLCD/GLCD.h"
#include "../game/walls.h"
#include "../game/player.h"
extern game_data game;
uint8_t select = 1;
/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

/*variabili per gestione debouncing dei buttons*/
volatile int down_0 = 0;
volatile int down_1 = 0;
volatile int down_2 = 0;
void RIT_IRQHandler(void)
{
	static int j_select = 0, j_left = 0, j_right = 0, j_down = 0, j_up = 0;

	/*Gestione Joystick Polling*/
	if ((LPC_GPIO1->FIOPIN & (1 << 25)) == 0)
	{ // joystick select
		j_select++;
		switch (j_select)
		{
		case 1:
			// qua dentro quello che vuoi fare
			// if the game is not running we need to ignore the interrupts
			if (game.game_status == WAIT_MODE)
				break;
			if (game.game_status == OVER)
				break;
			// If we end up here the game is in progress and we have a move
			// Select is used also for the walls, we need to know if we are in wall mode or move_mode
			if (game.input_mode == PLAYER_MOVEMENT)
			{
				// This will solidify whatever move
				swap_turn(&game, select);
			}
			else
			{
				// we need to confirm the wall placement
				// TODO: you can simplify this, simply solidify the placement based on the game mode (wtf? were you drunk?)
				// We need to do the validity checks here before solidifying the placement
				if (game.board[game.current_wall.top.x][game.current_wall.top.y].availability == OCCUPIED ||
					game.board[game.current_wall.middle.x][game.current_wall.middle.y].availability == OCCUPIED ||
					game.board[game.current_wall.bottom.x][game.current_wall.bottom.y].availability == OCCUPIED)
				{
					game.player_turn == PLAYER_1 ? (GUI_Text(X_MESSAGE_AREA, Y_MESSAGE_AREA,
															 (uint8_t *)"Cannot place wall here!", White, Black))
												 : (GUI_Text(X_MESSAGE_AREA, Y_MESSAGE_AREA,
															 (uint8_t *)"Cannot place wall here!", Red, Black));
					break;
				}
				// If we are here we can finalize the move
				// We first need to simulate the move
				game.board[game.current_wall.top.x][game.current_wall.top.y].availability = OCCUPIED;
				game.board[game.current_wall.middle.x][game.current_wall.middle.y].availability = OCCUPIED;
				game.board[game.current_wall.bottom.x][game.current_wall.bottom.y].availability = OCCUPIED;
				if (legal_wall_placement(&game))
				{
					game.player_turn == PLAYER_1 ? (game.player_1.available_walls--) : (game.player_2.available_walls--);
					game.player_turn == PLAYER_1 ? (p1_walls_update(game.player_1.available_walls)) : (p2_walls_update(game.player_2.available_walls));
					swap_turn(&game, select);
				}
				else
				{
					game.player_turn == PLAYER_1 ? (GUI_Text(X_MESSAGE_AREA, Y_MESSAGE_AREA,
															 (uint8_t *)"Cannot place wall here!", White, Black))
												 : (GUI_Text(X_MESSAGE_AREA, Y_MESSAGE_AREA,
															 (uint8_t *)"Cannot place wall here!", Red, Black));
					game.board[game.current_wall.top.x][game.current_wall.top.y].availability = FREE;
					game.board[game.current_wall.middle.x][game.current_wall.middle.y].availability = FREE;
					game.board[game.current_wall.bottom.x][game.current_wall.bottom.y].availability = FREE;
				}
			}
			break;
		default:
			break;
		}
	}
	else
		j_select = 0;

	if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0)
	{ // joystick down
		j_down++;
		switch (j_down)
		{
		case 1:
			// qua dentro quello che vuoi fare
			if (game.game_status == WAIT_MODE)
				break;
			if (game.game_status == OVER)
				break;
			// If we end up here the game is in progress and we have a move
			if (game.input_mode == PLAYER_MOVEMENT)
			{
				move_player(DOWN, &game);
			}
			else
			{
				// it's a wall placement
				move_wall(WALL_DOWN, &game);
			}

			break;
		default:
			break;
		}
	}
	else
		j_down = 0;

	if ((LPC_GPIO1->FIOPIN & (1 << 27)) == 0)
	{ // joystick left
		j_left++;
		switch (j_left)
		{
		case 1:
			// qua dentro quello che vuoi fare
			if (game.game_status == WAIT_MODE)
				break;
			if (game.game_status == OVER)
				break;
			// If we end up here the game is in progress and we have a move
			if (game.input_mode == PLAYER_MOVEMENT)
			{
				move_player(LEFT, &game);
			}
			else
			{
				// it's a wall placement
				move_wall(WALL_LEFT, &game);
			}
			break;
		default:
			break;
		}
	}
	else
		j_left = 0;

	if ((LPC_GPIO1->FIOPIN & (1 << 28)) == 0)
	{ // joystick right
		j_right++;
		switch (j_right)
		{
		case 1:
			// qua dentro quello che vuoi fare
			if (game.game_status == WAIT_MODE)
				break;
			if (game.game_status == OVER)
				break;
			// If we end up here the game is in progress and we have a move
			if (game.input_mode == PLAYER_MOVEMENT)
			{
				move_player(RIGHT, &game);
			}
			else
			{
				// it's a wall placement
				move_wall(WALL_RIGHT, &game);
			}
			break;
		default:
			break;
		}
	}
	else
		j_right = 0;

	if ((LPC_GPIO1->FIOPIN & (1 << 29)) == 0)
	{ // joystick up
		j_up++;
		switch (j_up)
		{
		case 1:
			if (game.game_status == WAIT_MODE)
				break;
			if (game.game_status == OVER)
				break;
			// If we end up here the game is in progress and we have a move
			if (game.input_mode == PLAYER_MOVEMENT)
			{
				move_player(UP, &game);
			}
			else
			{
				// it's a wall placement
				move_wall(WALL_UP, &game);
			}
			break;
		default:
			break;
		}
	}
	else
		j_up = 0;

	/*Gestione BUTTONS*/
	if (down_0 != 0)
	{
		down_0++;
		// controllo il piedino fisico 10->INT0, 11->KEY1, 12->KEY2
		if ((LPC_GPIO2->FIOPIN & (1 << 10)) == 0)
		{
			switch (down_0)
			{
			case 2:
				// this is the button that starts the game
				game_start(&game);
				break;
			// per controllare pressioni prolungate: es. 5[sec]/50[msec](RIT timer) = 100
			// case 100:
			//	break;
			default:
				// se voglio gestire il button on X click
				// if(down_0 % X == 0){
				// }
				break;
			}
		}
		else
		{ /* button released */
			down_0 = 0;
			NVIC_EnableIRQ(EINT0_IRQn);		  /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4 |= (1 << 20); /* External interrupt 0 pin selection */
		}
	}

	if (down_1 != 0)
	{
		down_1++;
		// controllo il piedino fisico 10->INT0, 11->KEY1, 12->KEY2
		if ((LPC_GPIO2->FIOPIN & (1 << 11)) == 0)
		{
			switch (down_1)
			{
			case 2:
				// we have to enable the wall mode
				if (game.game_status == RUNNING)
				{
					if (game.input_mode == PLAYER_MOVEMENT)
					{
						if (game.player_turn == PLAYER_1)
						{
							if (game.player_1.available_walls == 0)
							{
								GUI_Text(X_MESSAGE_AREA, Y_MESSAGE_AREA, "No walls available, move the", White, Black);
								GUI_Text(X_MESSAGE_AREA, Y_MESSAGE_AREA + 16, "token!", White, Black);
								break;
							}
						}
						else
						{
							if (game.player_2.available_walls == 0)
							{
								GUI_Text(X_MESSAGE_AREA, Y_MESSAGE_AREA, "No walls available, move the", Red, Black);
								GUI_Text(X_MESSAGE_AREA, Y_MESSAGE_AREA + 16, "token!", Red, Black);
								break;
							}
						}
						// game.player_turn == PLAYER_1 ? (update_player_token_pos(game.player_1.x_matrix_coordinate, game.player_1.y_matrix_coordinate,
						// 														game.player_1.x_matrix_coordinate, game.player_1.y_matrix_coordinate, PLAYER_1))
						// 							 : (update_player_token_pos(game.player_2.x_matrix_coordinate, game.player_2.y_matrix_coordinate,
						// 														game.player_2.x_matrix_coordinate, game.player_2.y_matrix_coordinate, PLAYER_2));
						// game.player_turn == PLAYER_1 ? (game.player_1.tmp_x_matrix_coordinate = game.player_1.x_matrix_coordinate) : (game.player_2.tmp_x_matrix_coordinate = game.player_2.x_matrix_coordinate);
						// game.player_turn == PLAYER_1 ? (game.player_1.tmp_y_matrix_coordinate = game.player_1.y_matrix_coordinate) : (game.player_2.tmp_y_matrix_coordinate = game.player_2.y_matrix_coordinate);
						// show_available_moves(&game, HIDE);
						// game.input_mode = WALL_MODE;
						// // We also need to initialize the wall struct
						// // initial position: [5][4]|[5][5|[5][6]
						// game.current_wall.top.x = 5;
						// game.current_wall.top.y = 4;
						// game.current_wall.middle.x = 5;
						// game.current_wall.middle.y = 5;
						// game.current_wall.bottom.x = 5;
						// game.current_wall.bottom.y = 6;
						// draw_walls();
					}
					else
					{
						// game.current_wall.top.x == game.current_wall.bottom.x ? (clear_wall(game.current_wall.top, HORIZONTAL)) : (clear_wall(game.current_wall.top, VERTICAL));
						// game.current_wall.top.x == game.current_wall.bottom.x ? (clear_wall(game.current_wall.middle, HORIZONTAL)) : (clear_wall(game.current_wall.middle, VERTICAL));
						// game.current_wall.top.x == game.current_wall.bottom.x ? (clear_wall(game.current_wall.bottom, HORIZONTAL)) : (clear_wall(game.current_wall.bottom, VERTICAL));
						// game.input_mode = PLAYER_MOVEMENT;
						// show_available_moves(&game, SHOW);
					}
				}
				break;
			default:
				// se voglio gestire il button on X click
				// if(down_1 % X == 0){
				// }
				break;
			}
		}
		else
		{ /* button released */
			down_1 = 0;
			NVIC_EnableIRQ(EINT1_IRQn);		  /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4 |= (1 << 22); /* External interrupt 0 pin selection */
		}
	}

	if (down_2 != 0)
	{
		down_2++;
		// controllo il piedino fisico 10->INT0, 11->KEY1, 12->KEY2
		if ((LPC_GPIO2->FIOPIN & (1 << 12)) == 0)
		{
			switch (down_2)
			{
			case 2:
				// We need to rotate the wall by 90 degrees
				if (game.game_status == RUNNING && game.input_mode == WALL_MODE)
				{
					// we need to do something
					move_wall(WALL_ROTATION, &game);
					draw_walls();
				}
				break;
			default:
				// se voglio gestire il button on X click
				// if(down_2 % X == 0){
				// }
				break;
			}
		}
		else
		{ /* button released */
			down_2 = 0;
			NVIC_EnableIRQ(EINT2_IRQn);		  /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4 |= (1 << 24); /* External interrupt 0 pin selection */
		}
	}

	reset_RIT();
	LPC_RIT->RICTRL |= 0x1; /* clear interrupt flag */

	return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
