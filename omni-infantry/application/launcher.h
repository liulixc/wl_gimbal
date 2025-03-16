//
// Created by xhuanc on 2021/11/2.
//

#ifndef DEMO1_LAUNCHER_H
#define DEMO1_LAUNCHER_H

#include "can_receive.h"

/******************define******************/
#define FIRE_SPEED_30 6800  //27m/s上下
//#define FIRE_SPEED_30 2288  //温柔发射，大家都好（调试用）
\
#define TARGET_SHOOT_SPEED 26  //目标子弹射速m/s
#define DYNAMIC_SPEED_RATIO 50 //动态射速调整转速增益系数

#define FIRE_SPEED_LEVEL1 6500
#define FIRE_SPEED_LEVEL2 6500
#define FIRE_SPEED_LEVEL3 6500

#define TRIGGER_CONTINUES_SPEED -4000 //b拨弹电机速度  拨盘顺时针
//#define TRIGGER_CONTINUES_SPEED -500
#define TRIGGER_REVERSE_SPEED 4000
#define FIRE_L  0
#define FIRE_R  1
#define TRIGGER 0

#define DEGREE_45_TO_ENCODER -36934.0f
//#define FIRE_ON() KeyBoard.Q.click_flag==1
//#define FIRE_OFF() KeyBoard.Q.click_flag==0

//连发准备计时完成
#define CONTINUES_SHOOT_TIMING_COMPLETE() HAL_GetTick()-continue_shoot_time>200
#define CONTINUES_BLOCKED_JUDGE() (HAL_GetTick()-blocked_start_time>500)
#define TRIGGER_REVERSE_TIME_JUDGE() (HAL_GetTick()-reverse_start_time<100)

//摩擦轮转速PID
#define SHOOT_FIRE_L_PID_KP 30
#define SHOOT_FIRE_L_PID_KI 5.f
#define SHOOT_FIRE_L_PID_KD 0.f
#define SHOOT_FIRE_L_PID_MAX_OUT    16000
#define SHOOT_FIRE_L_PID_MAX_IOUT   5000

#define SHOOT_FIRE_R_PID_KP 30
#define SHOOT_FIRE_R_PID_KI 5.f
#define SHOOT_FIRE_R_PID_KD 0.f
#define SHOOT_FIRE_R_PID_MAX_OUT    16000
#define SHOOT_FIRE_R_PID_MAX_IOUT   5000

//拨弹电机角度环PID
#define SHOOT_TRI_ANGLE_PID_KP 0.45
//#define SHOOT_TRI_ANGLE_PID_KP 1.05
#define SHOOT_TRI_ANGLE_PID_KI 0.f
#define SHOOT_TRI_ANGLE_PID_KD 0.0f
#define SHOOT_TRI_ANGLE_PID_MAX_OUT 3500
#define SHOOT_TRI_ANGLE_PID_MAX_IOUT 2500

//拨弹电机速度环PID
#define SHOOT_TRI_SPEED_PID_KP  15
#define SHOOT_TRI_SPEED_PID_KI  0.00f
#define SHOOT_TRI_SPEED_PID_KD  0.f
#define SHOOT_TRI_SPEED_PID_MAX_OUT 11000
#define SHOOT_TRI_SPEED_PID_MAX_IOUT 8000

/******************struct&enum******************/

typedef enum{
    Fire_OFF=0,
    Fire_ON=1,
}fire_mode_e;

typedef enum{
    SHOOT_CLOSE=0,
    SHOOT_SINGLE,
    SHOOT_ING,
    SHOOT_CONTINUES,
}trigger_cmd;

typedef struct {
    fire_mode_e fire_mode;//摩擦轮状态

    fire_mode_e fire_last_mode;//摩擦轮上一次状态

    trigger_cmd trigger_cmd;    //发射机构单发还是

    trigger_cmd trigger_last_cmd;   //波轮上一模式

    motor_3508_t fire_l;

    motor_3508_t fire_r;

    motor_2006_t trigger;

}launcher_t;

extern void launcher_init();
extern void launcher_mode_set();
extern void launcher_control();



#endif //DEMO1_LAUNCHER_H
