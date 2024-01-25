/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "../led/led.h"
#include "../RIT/RIT.h"
#include "../../common.h"
#include "../game/quoridor.h"
#include "../graphics/graphics.h"

extern int down;
extern game_data game;
/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER0_IRQHandler(void)
{
	// clear interrupt flag
	LPC_TIM0->IR = 1;
	// Decrementing the game_tick counter
	ticks_counter_update(game.game_tick);
	game.game_tick--;
	ticks_counter_update(game.game_tick);
	if (game.game_tick == 0)
	{
		// this is the last turn, we need to change_turn
		ticks_counter_update(game.game_tick);
		turn_manager(&game, 1);
		game.game_tick = 20;
	}

	return;
}

/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler(void)
{
	LPC_TIM1->IR = 1; /* clear interrupt flag */
	return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 2 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER2_IRQHandler(void)
{
	LPC_TIM2->IR = 1; /* clear interrupt flag */
	return;
}

/******************************************************************************
** Function name:		Timer3_IRQHandler
**
** Descriptions:		Timer/Counter 3 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER3_IRQHandler(void)
{
	LPC_TIM3->IR = 1; /* clear interrupt flag */
	return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
