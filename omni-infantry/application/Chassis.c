//
// Created by xhuanc on 2021/10/10.
//

/*include*/
#include "Chassis.h"
#include "Detection.h"
#include "bsp_cap.h"

/*变量*/
extern RC_ctrl_t rc_ctrl;

chassis_t chassis;
extern gimbal_t gimbal;
//上位机下发数据
extern robot_ctrl_info_t robot_ctrl;
//遥控器数据
extern key_board_t KeyBoard;
//电容数据
extern cap2_info_t cap2;
//发送机器人id
extern uint8_t control_flag;
vision_t vision_data;

extern ChassisMsg chassis_msg;

/*      函数及声明   */
static void chassis_init();

static void chassis_set_mode(chassis_t* chassis);

static void chassis_ctrl_info_get();

void chassis_device_offline_handle();

static void chassis_power_stop();

static float chassis_speed_change();

uint8_t test2=0;
/*程序主体*/
_Noreturn void chassis_task(void const *pvParameters) {

    vTaskDelay(CHASSIS_TASK_INIT_TIME);
    chassis_init();//底盘初始化

    //主任务循环
    while (1) {

//        vTaskSuspendAll(); //锁住RTOS内核防止控制过程中断，造成错误
        //更新PC的控制信息
        update_pc_info();

        //设置底盘模式
        chassis_set_mode(&chassis);

        //遥控器获取底盘方向矢量
        chassis_ctrl_info_get();

        //遥控断电失能
        chassis_device_offline_handle();

        vTaskDelay(1);

    }
}

float chassis_speed_change()
{
    float speed_change = 0;
    if (cap_mode == CAP_MODE_WORK) //开启电容 增加加速度
    {
        speed_change = 0.0062f;//最大加速度
    } else {
        switch (Referee.GameRobotStat.chassis_power_limit) {//最大限制功率
            case 40: {
                speed_change=0.0014f;
                break;
            }
            case 45: {
                speed_change=0.0018f;
                break;
            }
            case 50: {
                speed_change=0.0022f;
                break;
            }
            case 55: {
                speed_change=0.0026f;
                break;
            }
            case 60: {
                speed_change=0.003f;
                break;
            }
            case 65: {
                speed_change=0.0034f;
                break;
            }
            case 70: {
                speed_change=0.0038f;
                break;
            }
            case 75: {
                speed_change=0.0042f;
                break;
            }
            case 80: {
                speed_change=0.0046f;
                break;
            }
            case 85: {
                speed_change=0.0050f;
                break;
            }
            case 90: {
                speed_change=0.0054f;
                break;
            }
            case 95: {
                speed_change=0.0058f;
                break;
            }
            case 100: {
                speed_change=0.0062f;
                break;
            }
            default:{
                speed_change=0.0014f;
            }break;
        }
    }
    return speed_change;
}

static void chassis_pc_ctrl(){

    float speed_change=chassis_speed_change();//获取加速度

//    float speed_change=0.0005f;
    //键盘控制下的底盘以斜坡式变化
    if(KeyBoard.W.status==KEY_PRESS)//键盘前进键按下
    {
        chassis.vx_pc+=speed_change;//速度增量
    }
    else if(KeyBoard.S.status==KEY_PRESS)
    {
        chassis.vx_pc-=speed_change;
    }
    else{
        chassis.vx_pc=0;
    }


    VAL_LIMIT(chassis.chassis_ctrl_info.v_m_per_s,-MAX_CHASSIS_VX_SPEED,MAX_CHASSIS_VX_SPEED);

//    if(chassis.mode==CHASSIS_SPIN)//灯
//    {
//        HAL_GPIO_WritePin(LED6_PORT,LED6_PIN,GPIO_PIN_RESET);
//    } else{
//        HAL_GPIO_WritePin(LED6_PORT,LED6_PIN,GPIO_PIN_SET);
//    }

    if(KeyBoard.E.click_flag==1)//
    {
        chassis.mode=CHASSIS_SPIN;
    }

    if(chassis.mode==CHASSIS_SPIN)//灯
    {
//        led.mode=SPIN;//无led
    }
//    ui_robot_status.chassis_mode=chassis.chassis_ctrl_mode;

//    if(chassis.mode==CHASSIS_CLIMBING)//灯
//    {
//        HAL_GPIO_WritePin(LED5_PORT,LED5_PIN,GPIO_PIN_RESET);
//    } else{
//        HAL_GPIO_WritePin(LED5_PORT,LED5_PIN,GPIO_PIN_SET);
//    }
}

static void chassis_init() {
    //底盘驱动电机速度环初始化和电机数据结构体获取
    //初始时底盘模式为失能
    chassis.mode = chassis.last_mode = CHASSIS_RELAX;
    chassis.spin_mode=NORMAL_SPIN;

    chassis.chassis_ctrl_info.height_m=0.18f;

}

static void chassis_set_mode(chassis_t* chassis){

    if(chassis==NULL)
        return;

    if(switch_is_down(rc_ctrl.rc.s[RC_s_R]))
    {
        chassis->last_mode=chassis->mode;
        chassis->mode=CHASSIS_RELAX;
    }

    else if(switch_is_mid(rc_ctrl.rc.s[RC_s_R]))
    {
        chassis->last_mode=chassis->mode;
        chassis->mode=CHASSIS_ENABLE;
    }

    else if (switch_is_up(rc_ctrl.rc.s[RC_s_R]))
    {
        chassis->last_mode=chassis->mode;
        chassis->mode=CHASSIS_SPIN;
    }
    //底盘断电失能判断
    chassis_power_stop();

//    if(control_flag == VT_ONLINE)
//    {
//        chassis->last_mode=chassis->mode;
//        chassis->mode=CHASSIS_FOLLOW_GIMBAL;
//    }

    CAN_cmd_Mode(CAN_1,
                 CHASSIS_MODE_INFO,
                 chassis->mode);

    //UI更新---底盘模式
    ui_robot_status.chassis_mode=chassis->mode;

}

static void chassis_ctrl_info_get() {

    chassis_pc_ctrl();
    chassis.chassis_ctrl_info.v_m_per_s = (float) (rc_ctrl.rc.ch[CHASSIS_VX_CHANNEL]) * RC_TO_VX+chassis.vx_pc;
    if (rc_ctrl.rc.ch[CHASSIS_HEIGHT_CHANNEL]<-600)chassis.chassis_ctrl_info.height_m=MIN_L0;
    else if (rc_ctrl.rc.ch[CHASSIS_HEIGHT_CHANNEL]<100&&rc_ctrl.rc.ch[CHASSIS_HEIGHT_CHANNEL]>-100)chassis.chassis_ctrl_info.height_m=MID_L0;
    else if (rc_ctrl.rc.ch[CHASSIS_HEIGHT_CHANNEL]>600)chassis.chassis_ctrl_info.height_m=MAX_L0;
    VAL_LIMIT(chassis.chassis_ctrl_info.height_m,MIN_L0,MAX_L0);

    chassis.chassis_ctrl_info.yaw_angle_rad=gimbal.yaw.relative_angle_get*MOTOR_ANGLE_TO_RAD;
    CAN_cmd_angle(CAN_1,CHASSIS_ANGLE_INFO,chassis.chassis_ctrl_info.yaw_angle_rad,0);
    CAN_cmd_speed_height(CAN_1,CHASSIS_SPEED_HEIGHT_INFO,chassis.chassis_ctrl_info.v_m_per_s,chassis.chassis_ctrl_info.height_m);



}

void chassis_device_offline_handle() {
    if(detect_list[DETECT_REMOTE].status==OFFLINE && detect_list[DETECT_VIDEO_TRANSIMITTER].status==OFFLINE) {
        chassis.mode = CHASSIS_RELAX;//防止出现底盘疯转
    }
}

//底盘断电，过检录
static void chassis_power_stop()
{
    if(detect_list[DETECT_REFEREE].status==ONLINE) {
        if (Referee.GameRobotStat.power_management_chassis_output == 0) {
            chassis.mode = CHASSIS_RELAX;
        }
    }
}
