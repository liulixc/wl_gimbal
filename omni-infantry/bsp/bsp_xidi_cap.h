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
    fp32 voltage_in;  //�����ѹ
    fp32 voltage_out;  //�����ѹ
    fp32 power_in;   //���빦�ʣ��ӵ����õĹ��ʣ�
    fp32 current_in;  //�������
    fp32 current_out;   //�������
    fp32 power_reference;   //���������
}cap_receive_data_t;
void decode_xidi_data(uint8_t rx_data[8]);
void send_data_to_control(int powerLimit);
#endif //OMNI_INFANTRY_BSP_XIDI_CAP_H
