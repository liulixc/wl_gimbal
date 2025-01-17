//
// Created by Lumos on 2024/4/21.
//

#ifndef OMNI_INFANTRY_BSP_CAP_H
#define OMNI_INFANTRY_BSP_CAP_H

#include "main.h"
#include "cmsis_os.h"

typedef enum
{
    CAP_EXCEED_MODE_DISABLE = 0x00,
    CAP_EXCEED_MODE_ENABLE = 0x01,
    CAP_INIT_MODE_24V = 0x00,
    CAP_INIT_MODE_28V = 0x01,
//    CAP_INIT_MODE_30V = 0x02,         //30V危险!!!
    CAP_MODE_SILENT = 0x00,
    CAP_MODE_WORK = 0x01,
    CAP_MODE_CHARGE = 0x02,
    CAP_INIT_FINISHED = 0xFF,
    CAP_INIT_FAILURED = 0x00,

    CAP_ERR_FEEDBACK_ID = 0x001,
    CAP_INIT_ID = 0x002,
    CAP_INFO_FEEDBACK_ID = 0x003,
    CAP_CONTROL_ID = 0x004,
    CAP_INIT_FEEDBACK_ID = 0x005
}Cap_e;

typedef enum
{
    CAP_FIRMWARE_ERR = 0,
    CAP_CAN_ERR,
    CAP_TEMP_ERR,
    CAP_CALI_ERR,
    CAP_VOLT_ERR,
    CAP_CURR_ERR,
    CAP_POWER_ERR,
    CAP_SAMP_ERR,
    CAP_ERR_SIZE
}Cap_err_e;

typedef struct
{
    uint16_t esr_v;         //ESR修正后的电容组电压
    uint8_t work_s1;        //工作强度1
    uint8_t work_s2;        //工作强度2
    uint16_t input_power;   //电源输入功率
}cap_feedback_t;

typedef struct
{
    uint8_t can_init_state;     //控制器是否准备就绪
    uint8_t err_state[8];       //反馈错误帧标志位
    cap_feedback_t capFeedback;
}cap_data_t;

extern cap_data_t Cap;

extern uint8_t cap_mode;

extern void cap_init(uint8_t cap_mode_);
extern void cap_control_ExceedOn(uint8_t pb_set, uint8_t cap_mode);
extern void cap_control_ExceedOff(uint8_t pb_set, uint8_t cap_mode);
extern void cap_test_loading();

#endif //OMNI_INFANTRY_BSP_CAP_H


