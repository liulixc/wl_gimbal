//
// Created by xhuanc on 2021/9/27.
//
/*轮子电机id：                                 解算坐标：      x(前)
            ****      前       ****                                 |
           * 2 *             * 1 *                                 |
            ****              ****                                 |
                                                                   |
           左                   右                    --------------z-----------y(右)
                                                                   |
            ****              ****                                 |
           * 3 *            * 4  *                                 |
            ****      后      ****                                 |

*/

#ifndef CAN_RECEIVE_H_
#define CAN_RECEIVE_H_
#include "struct_typedef.h"
#include "PID.h"
#include "Cap.h"
/******************** define *******************/

#define HALF_ECD_RANGE  4096
#define ECD_RANGE       8192
#define MOTOR_ECD_TO_RAD 0.000766990394f //      2*  PI  /8192
#define MOTOR_ECD_TO_ANGLE 0.0439453125f     //        360  /8192
#define MOTOR_RAD_TO_ANGLE 57.29577951308238f // 360/2PI
#define MOTOR_ANGLE_TO_RAD  0.0174532925199433f
/******************** struct *******************/

typedef enum {
    CAN_1,
    CAN_2,
}CAN_TYPE;

//CAN_ID 该枚举不区分CAN_1还是CAN_2
typedef enum
{
    //电机控制 发送ID,这是控制大疆电机，一个CAN_id控制四个电机
    CAN_MOTOR_0x200_ID = 0x200,
    CAN_MOTOR_0x1FF_ID = 0x1FF,
    CAN_MOTOR_0x2FF_ID = 0x2FF,//6020在1FF和2FF的标识符下，采取的是电压控制这里我先不改，0X1FE和0X2FE才是电流控制
    CAN_MOTOR_0x1FE_ID = 0x1FE,
    CAN_MOTOR_0x2FE_ID = 0x2FE,

    //底盘控制 发送ID
    CHASSIS_MODE_INFO = 0x101,
    CHASSIS_SPEED_HEIGHT_INFO = 0x102,
    CHASSIS_ANGLE_INFO = 0x103,

} CanSendID;

typedef enum{
    //0X200对应的电机ID(CAN2)
    CAN_LAUNCHER_3508_FIRE_L=0X201,
    CAN_LAUNCHER_3508_FIRE_R=0X202,
    CAN_LAUNCHER_2006_TRIGGER=0x203,//拨盘

//    CAN_LAUNCHER_2006_BARREL=0x206,//test

    //0X200对应的电机ID(CAN2)

    //0X1FF对应的电机ID(CAN2)
    CAN_GIMBAL_6020_YAW=0x205,
    CAN_GIMBAL_6020_PITCH=0x206,

    //底盘发来的数据
    CHASSIS_ANGLE_VEL_INFO = 0x111,

}CanReceiveDeviceId;



//电机的数据
typedef struct
{
    uint16_t ecd;
    int16_t speed_rpm;
    int16_t given_current;
    uint8_t temperate;
    int16_t last_ecd;

    int32_t round_cnt;   //电机旋转的总圈数
    int32_t total_ecd;   //电机旋转的总编码器数值

    uint16_t offset_ecd;//电机的校准编码值

    int32_t total_dis;   //电机总共走的距离

    float torque_round_cnt;//转子转过的角度（-360-360）
    float real_round_cnt;//轮子转过的角度
    float real_angle_deg;
} motor_measure_t;


typedef struct
{
    const motor_measure_t *motor_measure;

    fp32 speed;

    fp32 rpm_set;//好像没用到这个，用的都是上面的

    pid_t speed_p;

    int16_t give_current;


}motor_3508_t;

typedef struct
{

    motor_measure_t *motor_measure;
    //自然状态下pid

    pid_t angle_p;

    pid_t speed_p;
    //自瞄pid

    pid_t angle_p_auto;

    pid_t speed_p_auto;


    fp32 max_relative_angle; //°
    fp32 min_relative_angle; //°

    fp32 relative_angle_get;
    fp32 relative_angle_set; //°

    fp32 absolute_angle_get;
    fp32 absolute_angle_set;//rad

    fp32 gyro_set;  //转速设置
    int16_t give_current; //最终电流值

}motor_6020_t;


typedef struct
{
    motor_measure_t *motor_measure;

    pid_t angle_p;//角度环pid

    pid_t speed_p;//速度环pid

    fp32 speed;//转速期望值

    int16_t give_current;

}motor_2006_t;

typedef struct
{
    float pitch_gyro;
    float yaw_gyro;
}Chassis_measure_t;

typedef struct {
    float pitch_angle_rad;
    float yaw_angle_rad;
}ChassisMsg;//从底盘发过来的数据

////枚举 结构体
//typedef enum{
//    CHASSIS_DISABLE = 1,
//    CHASSIS_ENABLE,
//    CHASSIS_INIT,
//    CHASSIS_JUMP,
////    CHASSIS_SPIN,
//} ChassisCtrlMode;

//typedef struct {
//    union {
//        float value;
//        uint8_t data[4];
//    }vx;
//
//    union {
//        float value;
//        uint8_t data[4];
//    }vy;
//}Vector_get_from_top;

typedef union
{
    uint8_t original_data[3];
    uint32_t distance_data;
}tof_t;

/******************** extern *******************/



extern motor_measure_t motor_3508_measure[2];
extern motor_measure_t motor_yaw_measure;
extern motor_measure_t motor_pitch_measure;
extern motor_measure_t motor_2006_measure[1];
extern motor_measure_t motor_shoot_measure[4];

extern void CAN_cmd_motor(CAN_TYPE can_type,CanSendID CMD_ID,int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
void CAN_cmd_Mode(CAN_TYPE can_type,CanSendID CMD_ID,uint8_t mode);
void CAN_cmd_speed_height(CAN_TYPE can_type,CanSendID CMD_ID,float v_m_per_s,float height);
void CAN_cmd_angle(CAN_TYPE can_type,CanSendID CMD_ID,float yaw_angle_rad,float roll_angle_rad);

extern fp32 motor_ecd_to_rad_change(uint16_t ecd, uint16_t offset_ecd);

extern fp32 motor_ecd_to_angle_change(uint16_t ecd,uint16_t offset_ecd);


extern void CAN_cmd_cap2(cap2_info_t*cap);

#endif
