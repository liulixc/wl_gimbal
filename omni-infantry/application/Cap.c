//
// Created by xhuanc on 2022/5/22.
//

#include "Cap.h"
#include "cmsis_os.h"
#include "Chassis.h"
#include "can_receive.h"
#include "Referee.h"
#include "key_board.h"
#include "bsp_led.h"
#include "bsp_adc.h"
#include "bsp_cap.h"

extern key_board_t KeyBoard;
extern RC_ctrl_t rc_ctrl;
uint8_t chassis_power_output;

void cap_info_update()
{
//    if (Referee.GameRobotStat.current_HP <= 0 || Referee.GameRobotStat.power_management_chassis_output <= 0)
//    {
//        if (cap_mode != CAP_MODE_SILENT)
//        {
//            cap_mode = CAP_MODE_SILENT;
//        }
//    }
//    else
    {
//        if (KeyBoard.SHIFT.status == KEY_PRESS || rc_ctrl.rc.s[0] == 1)
//        {
//            cap_mode = CAP_MODE_WORK;
//        }
//        else
//        {
//            cap_mode = CAP_MODE_SILENT;
//        }
    }
}

uint8_t isExceedOn = 0;
void cap_task(void const *pvParameters)
{
    osDelay(CAP_TASK_INIT_TIME);
    while (1)
    {
        if(Cap.can_init_state == CAP_INIT_FINISHED)
        {
            chassis_power_output = Referee.GameRobotStat.power_management_chassis_output;
            cap_init(CAP_INIT_MODE_28V);
            cap_info_update();
            if(rc_ctrl.rc.ch[4] > 100)
            {
                cap_control_ExceedOn(Referee.GameRobotStat.chassis_power_limit - 8, CAP_MODE_WORK);
                buzzer_on(95, 10000);
                isExceedOn = 1;
            }
            else
            {
                cap_control_ExceedOff(Referee.GameRobotStat.chassis_power_limit - 8, CAP_MODE_WORK);
                buzzer_on(95, 0);
                isExceedOn = 0;
            }
//            cap_test_loading();
        }
        osDelay(100);
    }
}