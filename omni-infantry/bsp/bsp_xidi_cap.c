//
// Created by 86198 on 2024/5/15.
//
#include "bsp_xidi_cap.h"
cap_receive_data_t  capReceiveData;
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
void decode_xidi_data(uint8_t rx_data[8]){
    capReceiveData.voltage_in=rx_data[0];
    capReceiveData.voltage_out=rx_data[1];
    capReceiveData.current_in=rx_data[2];
    capReceiveData.current_out=rx_data[3];
    capReceiveData.power_in=rx_data[4];
    capReceiveData.power_reference=rx_data[5];
}
CAN_TxHeaderTypeDef txMessage;
uint8_t canSendData[8];
//发送反馈信息给云台c板
void send_data_to_control(int powerLimit){
    uint32_t send_mail_box;
    txMessage.StdId = 0x015;
    txMessage.IDE = CAN_ID_STD;
    txMessage.RTR = CAN_RTR_DATA;
    txMessage.DLC = 0x08;
    //控制板要求功率控制区间在30-100w
    if(powerLimit>100){
        powerLimit=100;
    }
    canSendData[0]=powerLimit;
    HAL_CAN_AddTxMessage(&hcan1, &txMessage, canSendData, &send_mail_box);
}