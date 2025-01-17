//
// Created by xhuanc on 2021/9/27.
//

#include "can_receive.h"
#include "cmsis_os.h"
#include "main.h"
#include "Chassis.h"
#include "math.h"
#include "Detection.h"
#include "launcher.h"
#include "Cap.h"
#include "bsp_cap.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

/******************** define *******************/

//电子数据解算,do while作为保护性代码，防止在展开时被错误编译
#define get_motor_measure(ptr, data)                                    \
    do{                                                                 \
        (ptr)->last_ecd = (ptr)->ecd;                                   \
        (ptr)->ecd = (uint16_t)((data)[0] << 8 | (data)[1]);            \
        (ptr)->speed_rpm = (uint16_t)((data)[2] << 8 | (data)[3]);      \
        (ptr)->given_current = (uint16_t)((data)[4] << 8 | (data)[5]);  \
        (ptr)->temperate = (data)[6];                                   \
    } while(0)

//电机总编码值的计算,do while作为保护性代码，防止在展开时被错误编译
#define get_motor_round_cnt(ptr)  \
    do{                            \
             if(ptr.ecd-ptr.last_ecd> 4192){ \
                ptr.round_cnt--;                    \
             }                   \
             else if(ptr.ecd-ptr.last_ecd< -4192)    \
             {                   \
                ptr.round_cnt++;            \
             }                   \
             ptr.total_ecd= ptr.round_cnt*8192+ptr.ecd-ptr.offset_ecd;\
                                 \
    }while(0)
//电机真实距离计算
#define get_motor_real_distance(ptr)\
{\
    get_motor_round_cnt(ptr);\
    ptr.torque_round_cnt=ptr.total_ecd/8192.f; \
    ptr.real_round_cnt=ptr.torque_round_cnt/19.f; \
    ptr.real_angle_deg=fmodf(ptr.real_round_cnt,1.0f);             \
    if(ptr.real_angle_deg<0.0f)ptr.real_angle_deg+=1.0f;                \
    ptr.real_angle_deg*=360.0f;     \
    ptr.total_dis=ptr.real_round_cnt*wheel_circumference;      \
}\

//3508减速比
#define motor_3508_reduction_ratio (3591.0f/187.0f)
//轮子周长
#define wheel_circumference (70*2*3.14f)//这个是普通步兵上的轮子周长
/******************** variable *******************/


motor_measure_t motor_3508_measure[2];
motor_measure_t motor_yaw_measure;
motor_measure_t motor_pitch_measure;
motor_measure_t motor_shoot_measure[4];//0:TRIGGER,建为数组方便以后添加
motor_measure_t motor_2006_measure[1];//TRIGGER
extern cap2_info_t cap2;

 ChassisMsg chassis_msg;
static CAN_TxHeaderTypeDef tx_message;
static uint8_t can_send_data[8];
int32_t cap_percentage;

int cap_can_cnt = 0;

void cap2_info_decode(cap2_info_t *cap,uint8_t *rx_data){
    cap->mode=rx_data[0];
//    cap->rec_cap_cmd=rx_data[1];
//    cap->cap_voltage=rx_data[2];
    cap->cap_voltage=(uint16_t)(rx_data[2]<<8|rx_data[3]);
//    cap->chassis_current=(uint16_t)(rx_data[4]<<8|rx_data[5])/1000;
    cap_percentage=cap->cap_voltage/22.f;

    cap_can_cnt++;
}

static int float_to_uint(float x, float x_min, float x_max, int bits) {
    /// Converts a float to an unsigned int, given range and number of bits///
    float span = x_max - x_min;
    float offset = x_min;
    return (int) ((x - offset) * ((float) ((1 << bits) - 1)) / span);
}

static float uint_to_float(int x_int, float x_min, float x_max, int bits) {
/// converts unsigned int to float, given range and number of bits ///
    float span = x_max - x_min;
    float offset = x_min;
    return ((float) x_int) * span / ((float) ((1 << bits) - 1)) + offset;
}

//车轮电机的发送函数
void CAN_cmd_motor(CAN_TYPE can_type, CanSendID CMD_ID, int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4) {
    uint32_t send_mail_box;
    tx_message.StdId = CMD_ID;
    tx_message.IDE = CAN_ID_STD;
    tx_message.RTR = CAN_RTR_DATA;
    tx_message.DLC = 0x08;
    can_send_data[0] = motor1 >> 8;
    can_send_data[1] = motor1;
    can_send_data[2] = motor2 >> 8;
    can_send_data[3] = motor2;
    can_send_data[4] = motor3 >> 8;
    can_send_data[5] = motor3;
    can_send_data[6] = motor4 >> 8;
    can_send_data[7] = motor4;

    if (can_type == CAN_1) {
        HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, &send_mail_box);
    } else if (can_type == CAN_2) {
        HAL_CAN_AddTxMessage(&hcan2, &tx_message, can_send_data, &send_mail_box);
    }
}

//控制底盘的模式、腿长高度
void CAN_cmd_Mode(CAN_TYPE can_type,
                    CanSendID CMD_ID,
                    uint8_t mode) {
    uint32_t send_mail_box;
    tx_message.StdId = CMD_ID;
    tx_message.IDE = CAN_ID_STD;
    tx_message.RTR = CAN_RTR_DATA;
    tx_message.DLC = 0x08;

    can_send_data[0] = 0;
    can_send_data[1] = mode;
    can_send_data[2] = 0;
    can_send_data[3] = 0;
    can_send_data[4] = 0;
    can_send_data[5] = 0;
    can_send_data[6] = 0;
    can_send_data[7] = 0;

    if (can_type == CAN_1) {
        HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, &send_mail_box);
    } else if (can_type == CAN_2) {
        HAL_CAN_AddTxMessage(&hcan2, &tx_message, can_send_data, &send_mail_box);
    }
}

//控制底盘的速度
void CAN_cmd_speed_height(CAN_TYPE can_type,
                        CanSendID CMD_ID,
                        float v_m_per_s,
                        float height) {
    uint32_t send_mail_box;
    tx_message.StdId = CMD_ID;
    tx_message.IDE = CAN_ID_STD;
    tx_message.RTR = CAN_RTR_DATA;
    tx_message.DLC = 0x08;

    uint32_t int_v_m_per_s = *((uint32_t *) &v_m_per_s);
    uint32_t int_height = *((uint32_t *) &height);

    can_send_data[0] = int_v_m_per_s>>24;
    can_send_data[1] = int_v_m_per_s>>16;
    can_send_data[2] = int_v_m_per_s>>8;
    can_send_data[3] = int_v_m_per_s;
    can_send_data[4] = int_height >> 24;
    can_send_data[5] = int_height >> 16;
    can_send_data[6] = int_height >> 8;
    can_send_data[7] = int_height;

    if (can_type == CAN_1) {
        HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, &send_mail_box);
    } else if (can_type == CAN_2) {
        HAL_CAN_AddTxMessage(&hcan2, &tx_message, can_send_data, &send_mail_box);
    }
}

//控制底盘的yaw和roll
void CAN_cmd_angle(CAN_TYPE can_type,
                   CanSendID CMD_ID,
                   float yaw_angle_rad,
                   float roll_angle_rad) {
    uint32_t send_mail_box;
    tx_message.StdId = CMD_ID;
    tx_message.IDE = CAN_ID_STD;
    tx_message.RTR = CAN_RTR_DATA;
    tx_message.DLC = 0x08;

    uint32_t int_yaw_angle_rad = *((uint32_t *) &yaw_angle_rad);
    uint32_t int_roll_angle_rad= *((uint32_t *) &roll_angle_rad);
    can_send_data[0] = int_roll_angle_rad>>24;
    can_send_data[1] = int_roll_angle_rad>>16;
    can_send_data[2] = int_roll_angle_rad>>8;
    can_send_data[3] = int_roll_angle_rad;
    can_send_data[4] = int_yaw_angle_rad>>24;
    can_send_data[5] = int_yaw_angle_rad>>16;
    can_send_data[6] = int_yaw_angle_rad >>8;
    can_send_data[7] = int_yaw_angle_rad;

    if (can_type == CAN_1) {
        HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, &send_mail_box);
    } else if (can_type == CAN_2) {
        HAL_CAN_AddTxMessage(&hcan2, &tx_message, can_send_data, &send_mail_box);
    }
}


fp32 motor_ecd_to_rad_change(uint16_t ecd, uint16_t offset_ecd) {
    int32_t relative_ecd = ecd - offset_ecd;
    if (relative_ecd > HALF_ECD_RANGE) {
        relative_ecd -= ECD_RANGE;
    } else if (relative_ecd < -HALF_ECD_RANGE) {
        relative_ecd += ECD_RANGE;
    }

    return ((fp32)relative_ecd * MOTOR_ECD_TO_RAD);
}


//计算距离零点的度数  -180-180
fp32 motor_ecd_to_angle_change(uint16_t ecd, uint16_t offset_ecd) {
    int32_t tmp = 0;
    if (offset_ecd >= 4096) {
        if (ecd > offset_ecd - 4096) {
            tmp = ecd - offset_ecd;
        } else {
            tmp = ecd + 8192 - offset_ecd;
        }
    } else {
        if (ecd > offset_ecd + 4096) {
            tmp = ecd - 8192 - offset_ecd;
        } else {
            tmp = ecd - offset_ecd;
        }
    }
    return (fp32) tmp / 8192.f * 360;
}

void CAN_cmd_cap2(cap2_info_t*cap) {
    uint32_t send_mail_box;
    tx_message.StdId = 0x002;
    tx_message.IDE = CAN_ID_STD;
    tx_message.RTR = CAN_RTR_DATA;
    tx_message.DLC = 0x06;
    for (uint8_t i = 0; i <=4 ; ++i) {
        can_send_data[i]=cap->send_data[i];
    }
    HAL_CAN_AddTxMessage(&hcan1, &tx_message, cap->send_data, &send_mail_box);
}

uint8_t test=0;
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef rx_header;

    uint8_t rx_data[8];

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);

    if (hcan == &hcan1) {
        switch (rx_header.StdId) {
            case CHASSIS_ANGLE_VEL_INFO:
            {
                uint32_t int_pitch_vel=0;
                uint32_t int_yaw_vel=0;
                int_pitch_vel |= ((uint32_t)rx_data[0]) <<24;
                int_pitch_vel |= ((uint32_t)rx_data[1]) <<16;
                int_pitch_vel |= ((uint32_t)rx_data[2]) <<8;
                int_pitch_vel |= rx_data[3];
                int_yaw_vel |=((uint32_t )rx_data[4])<<24;
                int_yaw_vel |=((uint32_t )rx_data[5])<<16;
                int_yaw_vel |=((uint32_t )rx_data[6])<<8;
                int_yaw_vel |=rx_data[7];

                chassis_msg.pitch_angle_rad=*(float *) &int_pitch_vel;
                chassis_msg.yaw_angle_rad=*(float *) &int_yaw_vel;

                test++;
            }
            case CAP_ERR_FEEDBACK_ID:
            {
                Cap.err_state[CAP_FIRMWARE_ERR] = rx_data[CAP_FIRMWARE_ERR];
                Cap.err_state[CAP_CAN_ERR] = rx_data[CAP_CAN_ERR];
                Cap.err_state[CAP_TEMP_ERR] = rx_data[CAP_TEMP_ERR];
                Cap.err_state[CAP_CALI_ERR] = rx_data[CAP_CALI_ERR];
                Cap.err_state[CAP_VOLT_ERR] = rx_data[CAP_VOLT_ERR];
                Cap.err_state[CAP_CURR_ERR] = rx_data[CAP_CURR_ERR];
                Cap.err_state[CAP_POWER_ERR] = rx_data[CAP_POWER_ERR];
                Cap.err_state[CAP_SAMP_ERR] = rx_data[CAP_SAMP_ERR];
                if(Cap.can_init_state != CAP_INIT_FINISHED)
                {
                    Cap.can_init_state = CAP_INIT_FINISHED;
                }
                break;
            }
            case CAP_INFO_FEEDBACK_ID:
            {
                Cap.capFeedback.esr_v = (uint16_t)((rx_data[0] << 8) | rx_data[1]);
                Cap.capFeedback.work_s1 = rx_data[2];
                Cap.capFeedback.work_s2 = rx_data[3];
                Cap.capFeedback.input_power = (uint16_t)((rx_data[4] << 8) | rx_data[5]);
                if(Cap.can_init_state != CAP_INIT_FINISHED)
                {
                    Cap.can_init_state = CAP_INIT_FINISHED;
                }
                detect_handle(DETECT_CAP);
                break;
            }
            case CAP_INIT_FEEDBACK_ID:
            {
                Cap.can_init_state = rx_data[0];
                hcan->ErrorCode = HAL_CAN_ERROR_NONE;
                break;
            }

            default:
                break;
        }
    } else if (hcan == &hcan2) {
        switch (rx_header.StdId) {

            case CAN_LAUNCHER_3508_FIRE_L: {
                get_motor_measure(&motor_3508_measure[0], rx_data);
                detect_handle(DETECT_LAUNCHER_3508_FIRE_L);
            }break;

            case CAN_LAUNCHER_3508_FIRE_R:{
                get_motor_measure(&motor_3508_measure[1], rx_data);
                detect_handle(DETECT_LAUNCHER_3508_FIRE_R);
            }break;

            case CAN_GIMBAL_6020_PITCH: {
                get_motor_measure(&motor_pitch_measure, rx_data);
                detect_handle(DETECT_GIMBAL_6020_PITCH);
            }break;

            case CAN_LAUNCHER_2006_TRIGGER: {
                get_motor_measure(&motor_2006_measure[0], rx_data);
                get_motor_round_cnt(motor_2006_measure[0]);//获取转动拨轮电机转动圈数和总编码值
                detect_handle(DETECT_LAUNCHER_2006_TRIGGER);
            }break;

            case CAN_GIMBAL_6020_YAW: {
                get_motor_measure(&motor_yaw_measure, rx_data);
                detect_handle(DETECT_GIMBAL_6020_YAW);
            }break;

            default:
                break;
        }
    }
}