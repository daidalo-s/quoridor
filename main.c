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
#include "menu/menu.h"
#include "CAN/CAN.h"

#ifdef SIMULATOR
extern uint8_t ScaleFlag;
#endif
int down;
game_data game;
menu game_menu;
can_move received_move;
/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main(void)
{
	SystemInit(); /* System Initialization (i.e., PLL)  */

	/*BUTTONs handling */
	BUTTON_init();
	CAN_Init();

	/*Setting the RIT to 50 msec       */
	init_RIT(0x004C4B40); /*100.000.000[Mhz]*0.05[sec]=5.000.000 = 0x004C4B40*/
	enable_RIT();

	/*JOYSTICK handling*/
	joystick_init();

	LCD_Initialization();

	menu_init();

	LPC_SC->PCON |= 0x1; /* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);

	while (1)
	{ /* Loop forever                       */
		__ASM("wfi");
	}
}
