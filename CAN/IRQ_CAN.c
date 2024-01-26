/*----------------------------------------------------------------------------
 * Name:    Can.c
 * Purpose: CAN interface for for LPC17xx with MCB1700
 * Note(s): see also http://www.port.de/engl/canprod/sv_req_form.html
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <lpc17xx.h> /* LPC17xx definitions */
#include "CAN.h"	 /* LPC17xx CAN adaption layer */
#include "../GLCD/GLCD.h"
#include <stdio.h>
#include "../common.h"
#include "../menu/menu.h"
#include "../CAN/can_manager.h"

extern uint8_t icr; // icr and result must be global in order to work with both real and simulated landtiger.
extern uint32_t result;
extern CAN_msg CAN_TxMsg; /* CAN message for sending */
extern CAN_msg CAN_RxMsg; /* CAN message for receiving */
extern game_data game;
extern menu game_menu;
extern can_move received_move;

uint32_t data;
char str[2];
/*----------------------------------------------------------------------------
  CAN interrupt handler
 *----------------------------------------------------------------------------*/
void CAN_IRQHandler(void)
{

	/* check CAN controller 1 */
	icr = 0;
	icr = (LPC_CAN1->ICR | icr) & 0xFF; /* clear interrupts */

	if (icr & (1 << 0))
	{							  /* CAN Controller #1 meassage is received */
		CAN_rdMsg(1, &CAN_RxMsg); /* Read the message */
		LPC_CAN1->CMR = (1 << 2); /* Release receive buffer */

		// Ok qui dobbiamo fare decode del messaggio
		data = *(uint32_t *)CAN_RxMsg.data;
		received_move.player_id = data >> 24;
		// sprintf(str, "%d", player_id);
		// GUI_Text(0, 16, (uint8_t *)str, White, Black);
		received_move.game_mode = (data >> 20) & 0xF;
		// sprintf(str, "%d", game_mode);
		// GUI_Text(0, 32, (uint8_t *)str, White, Black);
		received_move.orientation = (data >> 16) & 0xF;
		// sprintf(str, "%d", orientation);
		// GUI_Text(0, 48, (uint8_t *)str, White, Black);
		received_move.x = (data >> 8) & 0xFF;
		// sprintf(str, "%d", y);
		// GUI_Text(0, 64, (uint8_t *)str, White, Black);
		received_move.y = data & 0xFF;
		// sprintf(str, "%d", x);
		// GUI_Text(0, 80, (uint8_t *)str, White, Black);
		if (received_move.player_id == 0xFF)
		{
			// it's the ack, we need to show the multi board menu
			// But it depends if we are the master or the slave
			if (game.multi_board_master == 0)
			{
				// we are the slave
				show_two_board_menu();
				GUI_Text(0, 0, (uint8_t *)"Connection established [slave]", White, Black);
			}
			else
			{
				// we are the master, it's the response, we can start
				LCD_Clear(Black);
				game_start(&game);
			}
		}
		else
		{
			// it's a regular move
			decode_last_move();
		}
	}
	else if (icr & (1 << 1))
	{	/* CAN Controller #1 meassage is transmitted */
		// do nothing in this example
		// puntiInviati1++;
	}

	/********************** DON'T GO BELOW **********************/
	/* check CAN controller 2 */
	icr = 0;
	icr = (LPC_CAN2->ICR | icr) & 0xFF; /* clear interrupts */

	if (icr & (1 << 0))
	{							  /* CAN Controller #2 meassage is received */
		CAN_rdMsg(2, &CAN_RxMsg); /* Read the message */
		LPC_CAN2->CMR = (1 << 2); /* Release receive buffer */

		// val_RxCoordX = (CAN_RxMsg.data[0] << 8);
		// val_RxCoordX = val_RxCoordX | CAN_RxMsg.data[1];

		// val_RxCoordY = (CAN_RxMsg.data[2] << 8);
		// val_RxCoordY = val_RxCoordY | CAN_RxMsg.data[3];

		// display.x = val_RxCoordX;
		// display.y = val_RxCoordY + 140;
		// TP_DrawPoint_Magnifier(&display);

		// puntiRicevuti2++;
	}
	else if (icr & (1 << 1))
	{	/* CAN Controller #2 meassage is transmitted */
		// do nothing in this example
		// puntiInviati2++;
	}
}
