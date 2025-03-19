//
// Created by xhuanc on 2021/10/13.
//

#ifndef GIMBAL_H_
#define GIMBAL_H_
/*      Include     */

#include "can_receive.h"
#include "PID.h"
#include "remote.h"
#include "AHRS.h"
#include "cmsis_os.h"
#include "launcher.h"
#include "can_receive.h"
#include "user_lib.h"
#include "protocol_shaob.h"
#include "filter.h"
#include "Detection.h"
#include "packet.h"
#include "Chassis.h"
/*      define     */

#define GIMBAL_TASK_INIT_TIME 3000
#define YAW_CHANNEL 2
#define PITCH_CHANNEL 3
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

//pitch最大最小绝对角
#define GIMBAL_PITCH_MAX_ABSOLUTE_ANGLE (28)
#define GIMBAL_PITCH_MIN_ABSOLUTE_ANGLE (-24)

//pitch yaw 初始ecd值
#define GIMBAL_PITCH_OFFSET_ECD 5745
#define GIMBAL_YAW_OFFSET_ECD 7506

#define RC_TO_YAW 0.001f
#define RC_TO_PITCH 0.0008f
//最大最小绝对相对角度
#define MAX_ABS_ANGLE (30)
#define MIN_ABS_ANGLE (-30)
#define MAX_RELA_ANGLE (20)//35
#define MIN_RELA_ANGLE (-31)//-20
//云台转动速度系数
#define GIMBAL_RC_MOVE_RATIO_PIT 0.5f//0.5f
#define GIMBAL_RC_MOVE_RATIO_YAW 0.8f//0.8f
#define GIMBAL_YAW_PATROL_SPEED 0.08f



#define GIMBAL_PITCH_PATROL_SPEED 0.06f


#define GIMBAL_YAW_ANGLE_PID_KP     0.6f
#define GIMBAL_YAW_ANGLE_PID_KI     0.0001f
#define GIMBAL_YAW_ANGLE_PID_KD     8.f
#define GIMBAL_YAW_ANGLE_MAX_OUT    2000.f
#define GIMBAL_YAW_ANGLE_MAX_IOUT   1.0f//

#define GIMBAL_YAW_SPEED_PID_KP     5000.0f
#define GIMBAL_YAW_SPEED_PID_KI     50.0f
#define GIMBAL_YAW_SPEED_PID_KD     10.0f
#define GIMBAL_YAW_SPEED_MAX_OUT    30000.f
#define GIMBAL_YAW_SPEED_MAX_IOUT   3000.f


#define GIMBAL_PITCH_ANGLE_PID_KP   1.f
#define GIMBAL_PITCH_ANGLE_PID_KI   0.001f//1
#define GIMBAL_PITCH_ANGLE_PID_KD   8.f//280
#define GIMBAL_PITCH_ANGLE_MAX_OUT  400.f
#define GIMBAL_PITCH_ANGLE_MAX_IOUT 2.f

#define GIMBAL_PITCH_SPEED_PID_KP   2000.f//300
#define GIMBAL_PITCH_SPEED_PID_KI   50.f//5
#define GIMBAL_PITCH_SPEED_PID_KD   10.f//40
#define GIMBAL_PITCH_SPEED_MAX_OUT  30000.f//30000
#define GIMBAL_PITCH_SPEED_MAX_IOUT 1000.f//13000


//自瞄pid常量
//yaw

#define GIMBAL_YAW_ANGLE_PID_KP_AUTO     1.22f//15f
#define GIMBAL_YAW_ANGLE_PID_KI_AUTO     0.001f//0.005f
#define GIMBAL_YAW_ANGLE_PID_KD_AUTO     20.0f//300.f
#define GIMBAL_YAW_ANGLE_MAX_OUT_AUTO    2000.f
#define GIMBAL_YAW_ANGLE_MAX_IOUT_AUTO   5.f//60

#define GIMBAL_YAW_SPEED_PID_KP_AUTO     5000.f//180
#define GIMBAL_YAW_SPEED_PID_KI_AUTO     50.0f//50.0f
#define GIMBAL_YAW_SPEED_PID_KD_AUTO     10.0f//
#define GIMBAL_YAW_SPEED_MAX_OUT_AUTO    30000.f
#define GIMBAL_YAW_SPEED_MAX_IOUT_AUTO   3000.f

//pitch
#define GIMBAL_PITCH_ANGLE_PID_KP_AUTO   1.f
#define GIMBAL_PITCH_ANGLE_PID_KI_AUTO   0.001f
#define GIMBAL_PITCH_ANGLE_PID_KD_AUTO   8.f
#define GIMBAL_PITCH_ANGLE_MAX_OUT_AUTO  400.f
#define GIMBAL_PITCH_ANGLE_MAX_IOUT_AUTO 2.f

#define GIMBAL_PITCH_SPEED_PID_KP_AUTO   2000.f
#define GIMBAL_PITCH_SPEED_PID_KI_AUTO   50.f
#define GIMBAL_PITCH_SPEED_PID_KD_AUTO   10.f
#define GIMBAL_PITCH_SPEED_MAX_OUT_AUTO  30000.f
#define GIMBAL_PITCH_SPEED_MAX_IOUT_AUTO 3000.f


/*      结构体和枚举     */

typedef enum {
    GIMBAL_RELAX=1,//云台失能
    GIMBAL_BACK,//云台回中
    GIMBAL_ACTIVE,
    GIMBAL_AUTO,
    GIMBAL_BUFF, //大符
    GIMBAL_SBUFF, //小符
    GIMBAL_FORECAST,//预测
}gimbal_mode_e;

typedef struct {
    motor_6020_t yaw;
    motor_6020_t pitch;
    motor_2006_t trigger;
    gimbal_mode_e mode;
    gimbal_mode_e last_mode;

//    AHRS_Eulr_t*Eulr;   //姿态角

    fp32 relative_gyro_yaw;
    fp32 absolute_gyro_yaw;
    fp32 relative_gyro_pitch;
    fp32 absolute_gyro_pitch;
    int32_t yaw_imu_offset_angle;
    float horizon_angle;

}gimbal_t;

_Noreturn extern void gimbal_task(void const*pvParameters);


#endif
