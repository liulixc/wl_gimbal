//
// Created by 86198 on 2024/5/15.
//
#include "main.h"
#include "struct_typedef.h"
#include "bsp_can.h"
#include "can_receive.h"
#ifndef OMNI_INFANTRY_BSP_XIDI_CAP_H
#define OMNI_INFANTRY_BSP_XIDI_CAP_H
typedef  struct
{
    fp32 voltage_in;  //输入电压
    fp32 voltage_out;  //输出电压
    fp32 power_in;   //输入功率（从底盘拿的功率）
    fp32 current_in;  //输入电流
    fp32 current_out;   //输出电流
    fp32 power_reference;   //最大功率限制
}cap_receive_data_t;
void decode_xidi_data(uint8_t rx_data[8]);
void send_data_to_control(int powerLimit);
#endif //OMNI_INFANTRY_BSP_XIDI_CAP_H
