//
// Created by xhuanc on 2022/3/10.
//

#ifndef _DETECTION_H_
#define _DETECTION_H_
#include "struct_typedef.h"
#include <stdbool.h>

#define ONLINE 1
#define OFFLINE 0

typedef struct {
//    uint8_t enable;
    uint32_t last_online_time;
//    uint32_t current_time;
    uint8_t status;
    uint32_t offline_threshold;
    uint8_t warning_level;
//    void (*offline_handle)(void*);
}__packed detect_device_t;

typedef enum {
    DETECT_CHASSIS_3508_RF=0,
    DETECT_CHASSIS_3508_LF,
    DETECT_CHASSIS_3508_LB,
    DETECT_CHASSIS_3508_RB,
    DETECT_CHASSIS_TOF_LEFT,
    DETECT_CHASSIS_TOF_RIGHT,
    DETECT_GIMBAL_6020_PITCH,
    DETECT_GIMBAL_6020_YAW,
    DETECT_LAUNCHER_3508_FIRE_L,
    DETECT_LAUNCHER_3508_FIRE_R,
    DETECT_LAUNCHER_2006_TRIGGER,
    DETECT_REMOTE,
    DETECT_AUTO_AIM,
    DETECT_REFEREE,
    DETECT_CAP,
    DETECT_VIDEO_TRANSIMITTER,
    DETECT_DEVICE_LIST_LEN,
}detect_device_index;

typedef enum
{
    ALL_ONLINE = 1,
    RC_ONLINE,
    VT_ONLINE,
    ALL_OFFLINE,
}control_status;


extern detect_device_t detect_list[DETECT_DEVICE_LIST_LEN];
extern void detect_task(void const*pvParameters);
extern void detect_handle(uint8_t index);

#endif
