//
// Created by Lumos on 2024/4/21.
//

#include "bsp_cap.h"
#include "can.h"
#include "cmsis_os.h"

cap_data_t Cap;
static CAN_TxHeaderTypeDef tx_message;

uint8_t cap_update_flag = 0;
uint8_t cap_mode = CAP_MODE_SILENT;

void cap_init(uint8_t cap_mode_)
{
    uint8_t can_send_data[8];
    tx_message.StdId = CAP_INIT_ID;
    tx_message.IDE = CAN_ID_STD;
    tx_message.RTR = CAN_RTR_DATA;
    tx_message.DLC = 0x01;
    can_send_data[0] = cap_mode_;

    if(HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, (uint32_t*)CAN_TX_MAILBOX0) != HAL_OK)
    {
        if(HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, (uint32_t*)CAN_TX_MAILBOX1) != HAL_OK)
        {
            HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, (uint32_t*)CAN_TX_MAILBOX2);
        }
    }
}

void cap_control_ExceedOn(uint8_t pb_set, uint8_t cap_mode)
{
    uint8_t can_send_data[8];
    tx_message.StdId = CAP_CONTROL_ID;
    tx_message.IDE = CAN_ID_STD;
    tx_message.RTR = CAN_RTR_DATA;
    tx_message.DLC = 0x03;
    can_send_data[0] = pb_set;
    can_send_data[1] = CAP_EXCEED_MODE_ENABLE;
    can_send_data[2] = cap_mode;

    if(HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, (uint32_t*)CAN_TX_MAILBOX0) != HAL_OK)
    {
        if(HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, (uint32_t*)CAN_TX_MAILBOX1) != HAL_OK)
        {
            HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, (uint32_t*)CAN_TX_MAILBOX2);
        }
    }
}

void cap_control_ExceedOff(uint8_t pb_set, uint8_t cap_mode)
{
    uint8_t can_send_data[8];
    tx_message.StdId = CAP_CONTROL_ID;
    tx_message.IDE = CAN_ID_STD;
    tx_message.RTR = CAN_RTR_DATA;
    tx_message.DLC = 0x03;
    can_send_data[0] = pb_set;
    can_send_data[1] = CAP_EXCEED_MODE_DISABLE;
    can_send_data[2] = cap_mode;

    if(HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, (uint32_t*)CAN_TX_MAILBOX0) != HAL_OK)
    {
        if(HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, (uint32_t*)CAN_TX_MAILBOX1) != HAL_OK)
        {
            HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, (uint32_t*)CAN_TX_MAILBOX2);
        }
    }
}

void cap_test_loading()
{
    uint8_t can_send_data[8];
    tx_message.StdId = 0x06;
    tx_message.IDE = CAN_ID_STD;
    tx_message.RTR = CAN_RTR_DATA;
    tx_message.DLC = 0x08;
    can_send_data[0] = 0;
    can_send_data[1] = 0;
    can_send_data[2] = 0;
    can_send_data[3] = 0;
    can_send_data[4] = 0;
    can_send_data[5] = 0;
    can_send_data[6] = 0;
    can_send_data[7] = 0;

    if(HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, (uint32_t*)CAN_TX_MAILBOX0) != HAL_OK)
    {
        if(HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, (uint32_t*)CAN_TX_MAILBOX1) != HAL_OK)
        {
            HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, (uint32_t*)CAN_TX_MAILBOX2);
        }
    }
}
