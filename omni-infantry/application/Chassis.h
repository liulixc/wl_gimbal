//
// Created by xhuanc on 2021/10/10.
//

#ifndef CHASSIS_H_
#define CHASSIS_H_

/*include*/
#include "struct_typedef.h"
#include "FreeRTOS.h"
#include "can_receive.h"
#include "PID.h"
#include "remote.h"
#include "user_lib.h"
#include "queue.h"
#include "cmsis_os.h"
#include "user_lib.h"
#include "ramp.h"
#include "Gimbal.h"
#include "bsp_buzzer.h"
#include "math.h"
#include "Referee.h"
#include "Detection.h"
#include "protocol_shaob.h"
#include "packet.h"
#include "key_board.h"
#include "Cap.h"

/*define*/

//任务开始空闲一段时间
#define CHASSIS_TASK_INIT_TIME 157

#define CHASSIS_VX_CHANNEL 1

#define CHASSIS_HEIGHT_CHANNEL 0


/*
 *  s1                                                    s2
 *               |                            |
 *               |                            |
 *               |                            |
       ---------------------2       ---------------------0
 *               |左摇杆                       |右摇杆
 *               |                            |
 *               |                            |
 *               3                            1
 *
 */

#define chassis_start_buzzer buzzer_on  (31, 19999)
#define chassis_buzzer_off   buzzer_off()            //buzzer off，关闭蜂鸣器

//#define MAX_CH      ASSIS_VX_SPEED 2.0f
#define MAX_CHASSIS_VX_SPEED 2.0f


#define MAX_CHASSIS_VW_SPEED 4.4f
#define CHASSIS_SWING_SPEED -5.5f //-10.5f
#define CHASSIS_ARMOR_NOT_FACING_ENEMY_SPIN_SPEED 0.5 //装甲板没有面向敌人的速度
#define CHASSIS_ARMOR_FACING_ENEMY_SPIN_SPEED  3.0 //装甲板面向敌人的速度

#define MAX_CHASSIS_AUTO_VX_SPEED 8.0f //根据3508最高转速计算底盘最快移动速度应为3300左右
#define MAX_CHASSIS_AUTO_VY_SPEED 3.0f
#define MAX_CHASSIS_AUTO_VW_SPEED 1.5f//150


#define MIN_L0 0.10f
#define MAX_L0 0.35f
#define MID_L0 0.18f

#define  RC_TO_VX  (MAX_CHASSIS_VX_SPEED/660) //MAX_CHASSIS_VR_SPEED / RC_MAX_VALUE
#define  RC_TO_L0  (0.000001)
#define MAX_CHASSIS_YAW_INCREMENT 0.02f
#define RC_TO_YAW_INCREMENT (MAX_CHASSIS_YAW_INCREMENT/660)

typedef struct{
    float v_m_per_s;
    float x; // 位移
    float pitch_angle_rad;
    float yaw_angle_rad;
    float roll_angle_rad;
    float height_m;
    float spin_speed;
} ChassisCtrlInfo;

typedef enum
{
    CHASSIS_RELAX=1,
    CHASSIS_ENABLE,
    CHASSIS_SPIN,
    CHASSIS_ONLY,
    CHASSIS_FOLLOW_GIMBAL,
    CHASSIS_JUMP,
    CHASSIS_BLOCK,
} chassis_mode_e;

typedef enum
{
    NORMAL_SPIN,
    HIDDEN_ARMOR_SPEED_CHANGE
} chassis_spin_mode_e;

typedef struct
{
    chassis_mode_e mode;
    chassis_mode_e last_mode;
    chassis_spin_mode_e spin_mode;
    ChassisCtrlInfo chassis_ctrl_info;

    QueueHandle_t motor_data_queue;

    pid_t chassis_vw_pid;
    fp32 vx;
    fp32 vy;
    fp32 vw;

    fp32 vx_pc;
    fp32 vy_pc;
    fp32 vw_pc;

    float height;
    int init_flag;

}
chassis_t;


//函数声明
_Noreturn extern void chassis_task(void const *pvParameters);

//
#endif

