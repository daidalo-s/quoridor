#include "./can_manager.h"
#include "../common.h"
#include "./CAN.h"

extern game_data game;

void send_ack()
{
    *(uint32_t *)CAN_TxMsg.data = 0xFF120304;
    CAN_TxMsg.len = 4;
    CAN_TxMsg.id = 1;
    CAN_TxMsg.format = STANDARD_FORMAT;
    CAN_TxMsg.type = DATA_FRAME;
    CAN_wrMsg(1, &CAN_TxMsg);
}

void send_last_move()
{
    *(uint32_t *)CAN_TxMsg.data = game.last_move;
    CAN_TxMsg.len = 4;
    CAN_TxMsg.id = 1;
    CAN_TxMsg.format = STANDARD_FORMAT;
    CAN_TxMsg.type = DATA_FRAME;
    CAN_wrMsg(1, &CAN_TxMsg);
}
