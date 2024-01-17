/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: to control led through EINT buttons and manage the bouncing effect
 *        	- key1 switches on the led at the left of the current led on,
 *					- it implements a circular led effect.
 * Note(s): this version supports the LANDTIGER Emulator
 * Author: 	Paolo BERNARDI - PoliTO - last modified 15/12/2020
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Politecnico di Torino. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include "LPC17xx.H" /* LPC17xx definitions                */
#include "led/led.h"
#include "button_EXINT/button.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "joystick/joystick.h"
#include "game/quoridor.h"
#include "GLCD/GLCD.h"

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif
int down;
game_data game;
/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main(void)
{
	SystemInit(); /* System Initialization (i.e., PLL)  */

	/*GEstione BUTTONs */
	BUTTON_init();

	/*Gestione RIT 50 msec       */
	init_RIT(0x004C4B40); /*100.000.000[Mhz]*0.05[sec]=5.000.000 = 0x004C4B40*/
	enable_RIT();

	/*Gestione JOYSTICK*/
	joystick_init();
	LCD_Initialization();
	LCD_Clear(Black);

	game_init(&game);

	LPC_SC->PCON |= 0x1; /* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);

	while (1)
	{ /* Loop forever                       */
		__ASM("wfi");
	}
}
