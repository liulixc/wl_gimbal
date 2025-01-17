#include <stdbool.h>
#include "launcher.h"
#include "Gimbal.h"
#include "can_receive.h"
#include "Chassis.h"
#include "cmsis_os.h"
#include "remote.h"
#include "key_board.h"
#include "bsp_laser.h"
#include "filter.h"
#include "Referee.h"
#include "Detection.h"
#include "queue.h"
#include "STL.h"

uint8_t rc_last_sw_L;
uint8_t rc_last_sw_R;
uint8_t blocked_flag;
uint8_t reverse_flag;
uint32_t continue_shoot_time;//遥控器左边拨杆down的持续时间 或者 鼠标左键按下的持续时间
uint32_t blocked_start_time;
uint32_t reverse_start_time;
first_order_filter_type_t filter_trigger_rpm_in;
extern key_board_t KeyBoard;
extern RC_ctrl_t rc_ctrl;
extern gimbal_t gimbal;
extern chassis_t chassis;
extern robot_ctrl_info_t robot_ctrl;
bool is_blocked(float blockSpeed);
void launcher_relax_handle();
void trigger_block_handle(fp32 blockSpeed,fp32 reverse_time,fp32 reverseSpeed);
int thisRc=0,lastRc=0;
//通过赋值进行发射机构的初始化
launcher_t launcher;
Queue launchQueue;   //发射队列，储存弹丸发射速度，进行动态射速控制
int32_t total_ecd_ref=0;

void launcher_mode_set(){

    //摩擦轮关闭时,做拨杆向上拨一下开启摩擦轮
    //遥控器和键盘可以同步修改摩擦轮状态
    // 键盘直接修改相关按键click_flag的状态 通过click状态直接判断摩擦轮模式 避免遥控打开的摩擦轮被键盘关闭
    if((!switch_is_up(rc_last_sw_L)&&switch_is_up(rc_ctrl.rc.s[RC_s_L]))){
        if(KeyBoard.Q.click_flag==1 && (chassis.mode!=CHASSIS_RELAX&&chassis.mode!=CHASSIS_ONLY))
        {
            KeyBoard.Q.click_flag=0;
            launcher.fire_mode=Fire_OFF;
            laser_off();
            HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);
        }
        else if(KeyBoard.Q.click_flag==0&& (chassis.mode!=CHASSIS_RELAX&&chassis.mode!=CHASSIS_ONLY)){
            KeyBoard.Q.click_flag=1;
            launcher.fire_mode=Fire_ON;
            laser_on();
            HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
        }
    }

    //摩擦轮开启时,做拨杆向上拨一下关闭摩擦轮
    launcher.trigger_last_cmd = launcher.trigger_cmd;
    //拨轮控制 拨轮转动的条件是 当前摩擦轮是开的
    //要么遥控器拨杆向下播一下
    //要么鼠标左键点一下

    // 2023-04-22 临时修改单发逻辑，将鼠标按下视为拨杆按下，因为实测拨杆的单发逻辑是正常的。
    // 使用 shoot_enable 变量来替代下面原来的几个条件中拨杆下拨
    int shoot_enable = (switch_is_down(rc_ctrl.rc.s[RC_s_L]) ||
        KeyBoard.Mouse_l.status == KEY_CLICK)||robot_ctrl.fire_command!=0;

    if(launcher.fire_mode==Fire_ON&&( shoot_enable /* switch_is_down(rc_ctrl.rc.s[RC_s_L] */)
       || KeyBoard.Mouse_l.status==KEY_PRESS
       || launcher.trigger_cmd==SHOOT_ING
       || robot_ctrl.fire_command!=0)
    {

        if((!switch_is_down(rc_last_sw_L)&&(shoot_enable /* switch_is_down(rc_ctrl.rc.s[RC_s_L] */))
           ||KeyBoard.Mouse_l.status==KEY_CLICK && launcher.trigger_cmd!=SHOOT_ING)//launcher.trigger_cmd!=SHOOT_ING 表示当前不处于单发状态中
        {
            launcher.trigger_cmd=SHOOT_SINGLE;
            continue_shoot_time=HAL_GetTick();//计时
        }
        if(robot_ctrl.fire_command==1){
            launcher.trigger_cmd=SHOOT_SINGLE;
        }

        //计时达到触发连发的时间
        if(((shoot_enable /* switch_is_down(rc_ctrl.rc.s[RC_s_L] */)&&CONTINUES_SHOOT_TIMING_COMPLETE())
           ||(KeyBoard.Mouse_l.status==KEY_PRESS))
        {
            launcher.trigger_cmd=SHOOT_CONTINUES;
        }
        if(robot_ctrl.fire_command==2){
            launcher.trigger_cmd=SHOOT_CONTINUES;
        }
    }
    else //摩擦轮没开默认关闭
    {
        launcher.trigger_cmd=SHOOT_CLOSE;
    }
    if(robot_ctrl.fire_command==2){
        launcher.trigger_cmd=SHOOT_CONTINUES;
    }
    thisRc=rc_ctrl.rc.ch[4];
    if(thisRc!=0&&lastRc==0){
     //   launcher.trigger_cmd=SHOOT_SINGLE;  //调试用单发
    }
    lastRc=thisRc;
    rc_last_sw_L=rc_ctrl.rc.s[RC_s_L];
    rc_last_sw_R=rc_ctrl.rc.s[RC_s_R];
}
//发射机构控制
moving_Average_Filter speed_avg_18={
        .length=10,
};
uint8_t trigger_flag=0;
moving_Average_Filter bullet_speed_avg={
        .length=30,
};
bool thisReverseFlag =false,lastReverseFlag=false;
int next_goal=0,next_flag=false;   //记录单发堵转的目标ecd，以及是否进行单发堵转处理
float current_speed,last_speed,avg_speed;//记录当前子弹射速和上一发子弹射速，平均射速

float launch_add=0;//进行动态射速调整的发射pid增加值

void launcher_control() {

    if (gimbal.mode == GIMBAL_RELAX || (detect_list[DETECT_REMOTE].status == OFFLINE&&detect_list[DETECT_VIDEO_TRANSIMITTER].status==OFFLINE))
    {
        launcher_relax_handle();
    }
    else
    {
        if (launcher.fire_mode == Fire_ON) {
            launcher.fire_l.speed = -FIRE_SPEED_30;
            launcher.fire_r.speed = FIRE_SPEED_30;
            if (launcher.trigger_cmd == SHOOT_CLOSE) {
                launcher.trigger.speed = 0;
            } else if (launcher.trigger_cmd == SHOOT_SINGLE) { //收到单发命令
                launcher.trigger_cmd = SHOOT_ING;//进入正在单发状态
                if (next_flag) {
                    total_ecd_ref = next_goal;
                    next_flag = false;
                } else {
                    total_ecd_ref = launcher.trigger.motor_measure->total_ecd + DEGREE_45_TO_ENCODER;//单发拨动45
                }
                average_add(&bullet_speed_avg, Referee.ShootData.bullet_speed);
            } else if (launcher.trigger_cmd == SHOOT_ING) { //单发状态
                //判断是否到达目标位置
                if (KeyBoard.Mouse_l.status != KEY_PRESS) {
                    launcher.trigger_cmd = SHOOT_CLOSE;

                    launcher.trigger.speed = 0;
                }
            }
            launcher.trigger.speed = pid_calc(&launcher.trigger.angle_p, launcher.trigger.motor_measure->total_ecd,
                                              total_ecd_ref);
            //注：单发堵转应对逻辑
            //单发堵转后，电机反转一段时间后停止转动，记录当前一发的目标ecd值，
            //在下次进入单发状态时，将目标ecd值设置为上一发记录的目标ecd值，
            // 即完成视觉的要求，单发堵转时，堵的那一发不射出来，后续正常单发
            if (launcher.trigger_cmd != SHOOT_CONTINUES) {
                trigger_block_handle(1000, 80, 4000);
                thisReverseFlag = reverse_flag;
                if (lastReverseFlag && !thisReverseFlag) {
                    next_goal = total_ecd_ref;
                    next_flag = true;
                    total_ecd_ref = launcher.trigger.motor_measure->total_ecd;
                }
                lastReverseFlag = thisReverseFlag;

            }
            if (launcher.trigger_cmd == SHOOT_CONTINUES) {
                next_flag = false;
                total_ecd_ref = launcher.trigger.motor_measure->total_ecd;
                launcher.trigger.speed = TRIGGER_CONTINUES_SPEED;
                trigger_block_handle(1000, 100, 4000);
            }

        } else {
            launcher.fire_l.speed = 0;
            launcher.fire_r.speed = 0;
            launcher.trigger.speed = 0;
            total_ecd_ref = launcher.trigger.motor_measure->total_ecd;
            launcher.trigger_cmd = SHOOT_CLOSE;
        }

        if (ABS(total_ecd_ref - launcher.trigger.motor_measure->total_ecd) > 2000) {
            trigger_flag = 1;
        } else {
            trigger_flag = 0;
        }
    }
    //动态射速调整
    current_speed = Referee.ShootData.bullet_speed;
    if (current_speed != last_speed) {                //调试发现每发射速不一致，这里基本能保证每射出一发子弹，就进一次队列
        if (QueueSize(&launchQueue, &avg_speed) < 10) {//保持队列长度最大为10
            QueuePush(&launchQueue, current_speed);
        } else {
            QueuePush(&launchQueue, current_speed);
            QueuePop(&launchQueue);
        }
        if (QueueSize(&launchQueue, &avg_speed) > 7) {
//            launch_add+=(TARGET_SHOOT_SPEED-avg_speed)*DYNAMIC_SPEED_RATIO;
        }
    }
        //对动态射速调整输出限幅
        VAL_LIMIT(launch_add, -7000, 7000);
        last_speed = current_speed;
        launcher.trigger.give_current = pid_calc(&launcher.trigger.speed_p,
                                                 launcher.trigger.motor_measure->speed_rpm,
                                                 launcher.trigger.speed);

        launcher.fire_l.give_current = pid_calc(&launcher.fire_l.speed_p,
                                                launcher.fire_l.motor_measure->speed_rpm,
                                                launcher.fire_l.speed);
        launcher.fire_r.give_current = pid_calc(&launcher.fire_r.speed_p,
                                                launcher.fire_r.motor_measure->speed_rpm,
                                                launcher.fire_r.speed);
//        int launch_down = 0, down_flag = false;
//        if (abs(launcher.fire_l.motor_measure->speed_rpm - launcher.fire_r.motor_measure->speed_rpm) > 2000 &&
//            down_flag == false) {
//            launch_down = HAL_GetTick();
//            down_flag = true;
//        }
//        if (down_flag == true &&
//            abs(launcher.fire_l.motor_measure->speed_rpm - launcher.fire_r.motor_measure->speed_rpm) > 2000) {
//            if (HAL_GetTick() - launch_down > 1000) {
//                launcher.fire_l.speed = 0;
//                launcher.fire_r.speed = 0;
//                launcher.trigger.speed = 0;
//                total_ecd_ref = launcher.trigger.motor_measure->total_ecd;
//                launcher.trigger_cmd = SHOOT_CLOSE;
//            }
//
//        } else {
//            down_flag = false;
//        }

    }


    //发射机构失能
    void launcher_relax_handle() {
        launcher.trigger.speed = 0;
        launcher.fire_r.speed = 0;
        launcher.fire_l.speed = 0;
    }

    void launcher_init() {
        blocked_flag = false;//堵转标志位置0

        reverse_flag = false;//反转标志位置0

        launcher.trigger_cmd = SHOOT_CLOSE;//初始时发射机构默认关闭

        launcher.fire_last_mode = Fire_OFF;//初始时摩擦轮默认关闭

        launcher.fire_mode = Fire_OFF;//初始时摩擦轮默认关闭

        //获取发射机构电机数据结构体
        launcher.fire_l.motor_measure = &motor_3508_measure[FIRE_L];
        launcher.fire_r.motor_measure = &motor_3508_measure[FIRE_R];
        launcher.trigger.motor_measure = &motor_2006_measure[TRIGGER];

        //发射机构电机PID初始化
        launcher.fire_r.speed_p.p = SHOOT_FIRE_R_PID_KP;
        launcher.fire_r.speed_p.i = SHOOT_FIRE_R_PID_KI;
        launcher.fire_r.speed_p.d = SHOOT_FIRE_R_PID_KD;
        launcher.fire_r.speed_p.max_output = SHOOT_FIRE_R_PID_MAX_OUT;
        launcher.fire_r.speed_p.integral_limit = SHOOT_FIRE_R_PID_MAX_IOUT;

        launcher.fire_l.speed_p.p = SHOOT_FIRE_L_PID_KP;
        launcher.fire_l.speed_p.i = SHOOT_FIRE_L_PID_KI;
        launcher.fire_l.speed_p.d = SHOOT_FIRE_L_PID_KD;
        launcher.fire_l.speed_p.max_output = SHOOT_FIRE_L_PID_MAX_OUT;
        launcher.fire_l.speed_p.integral_limit = SHOOT_FIRE_L_PID_MAX_IOUT;

        launcher.trigger.angle_p.p = SHOOT_TRI_ANGLE_PID_KP;
        launcher.trigger.angle_p.i = SHOOT_TRI_ANGLE_PID_KI;
        launcher.trigger.angle_p.d = SHOOT_TRI_ANGLE_PID_KD;
        launcher.trigger.angle_p.max_output = SHOOT_TRI_ANGLE_PID_MAX_OUT;
        launcher.trigger.angle_p.integral_limit = SHOOT_TRI_ANGLE_PID_MAX_IOUT;

        launcher.trigger.speed_p.p = SHOOT_TRI_SPEED_PID_KP;
        launcher.trigger.speed_p.i = SHOOT_TRI_SPEED_PID_KI;
        launcher.trigger.speed_p.d = SHOOT_TRI_SPEED_PID_KD;
        launcher.trigger.speed_p.max_output = SHOOT_TRI_SPEED_PID_MAX_OUT;
        launcher.trigger.speed_p.integral_limit = SHOOT_TRI_SPEED_PID_MAX_IOUT;

        launcher.trigger.motor_measure->total_ecd = launcher.trigger.motor_measure->offset_ecd = launcher.trigger.motor_measure->ecd;
        //最开始的编码值作为拨轮电机的校准值
        first_order_filter_init(&filter_trigger_rpm_in, 1, 1);
    }

//判断是否堵转
/**
 *
 * @param blockSpeed 当转速差距大于blockSpeed时，判定堵转
 * @return 是否堵转
 */
    bool is_blocked(float blockSpeed) {
        //在标识位为0时，电机转速低于阈值时，判定堵转开始
        if (blocked_flag == false &&
            abs(launcher.trigger.motor_measure->speed_rpm - launcher.trigger.speed) > blockSpeed) {
            blocked_start_time = HAL_GetTick();//获取堵转开始时间
            blocked_flag = true;
        }

        //标识位为1时，已经开始堵转，判断是否堵转达到一定时间，若达到，则判定堵转
        if (blocked_flag == true &&
            abs(launcher.trigger.motor_measure->speed_rpm - launcher.trigger.speed) > blockSpeed) {
            if (CONTINUES_BLOCKED_JUDGE()) {
                blocked_flag = false;
                return true;
            }
        } else {
            blocked_flag = false;
        }
        return false;
    }
/**
 * 堵转处理函数
 * @param blockSpeed 期望速度和实际速度相差超过blockSpeed时判定堵转
 * @param reverse_time 反转持续时间
 * @param reverseSpeed 反转速度
 */
    void trigger_block_handle(fp32 blockSpeed, fp32 reverse_time, fp32 reverseSpeed) {
        //判断堵转并且反转标识为0时
        if ((is_blocked(blockSpeed) && reverse_flag == false)) {
            reverse_flag = true;//判定开始反转
            reverse_start_time = HAL_GetTick();//获取开始反转时间
        }

        //判定反转开始并且时间没有达到反转结束时间
        if (reverse_flag == true && (HAL_GetTick() - reverse_start_time < reverse_time)) {
            launcher.trigger.speed = reverseSpeed;//拨单电机设置为反转速度
        } else {
            reverse_flag = false;
        }
    }