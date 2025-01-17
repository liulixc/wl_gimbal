//
// Created by xhuanc on 2021/12/3.
// Update by zxk on 2024/3/12
//

#include "key_board.h"
#include "Referee.h"
#include "Detection.h"

key_board_t KeyBoard;
extern RC_ctrl_t rc_ctrl;
extern uint8_t control_flag;

static void key_update(Key *key, uint16_t key_status, uint16_t cnt);

void update_pc_info() {

    if(control_flag == ALL_ONLINE) //都在线,优先遥控器
    {
        /* 遥控器优先 */
        key_update(&KeyBoard.W, rc_ctrl.key.v & KEY_W, DEFAULT_CNT);//DEFAULT_CNT 为按键检测为长按的默认时间
        key_update(&KeyBoard.A, rc_ctrl.key.v & KEY_A, DEFAULT_CNT);
        key_update(&KeyBoard.S, rc_ctrl.key.v & KEY_S, DEFAULT_CNT);
        key_update(&KeyBoard.D, rc_ctrl.key.v & KEY_D, DEFAULT_CNT);
        key_update(&KeyBoard.Q, rc_ctrl.key.v & KEY_Q, DEFAULT_CNT);
        key_update(&KeyBoard.E, rc_ctrl.key.v & KEY_E, DEFAULT_CNT);
        key_update(&KeyBoard.R, rc_ctrl.key.v & KEY_R, DEFAULT_CNT);
        key_update(&KeyBoard.F, rc_ctrl.key.v & KEY_F, DEFAULT_CNT);
        key_update(&KeyBoard.G, rc_ctrl.key.v & KEY_G, DEFAULT_CNT);
        key_update(&KeyBoard.CTRL, rc_ctrl.key.v & KEY_CTRL, DEFAULT_CNT);
        key_update(&KeyBoard.SHIFT, rc_ctrl.key.v & KEY_SHIFT, DEFAULT_CNT);
        key_update(&KeyBoard.B, rc_ctrl.key.v & KEY_B, DEFAULT_CNT);
        key_update(&KeyBoard.C, rc_ctrl.key.v & KEY_C, DEFAULT_CNT);
        key_update(&KeyBoard.V, rc_ctrl.key.v & KEY_V, DEFAULT_CNT);
        key_update(&KeyBoard.X, rc_ctrl.key.v & KEY_X, DEFAULT_CNT);
        key_update(&KeyBoard.Z, rc_ctrl.key.v & KEY_Z, DEFAULT_CNT);

        /* 图传优先 */
//        key_update(&KeyBoard.W, Referee.keyboard.keyboard_value & KEY_W, DEFAULT_CNT);//DEFAULT_CNT 为按键检测为长按的默认时间
//        key_update(&KeyBoard.A, Referee.keyboard.keyboard_value & KEY_A, DEFAULT_CNT);
//        key_update(&KeyBoard.S, Referee.keyboard.keyboard_value & KEY_S, DEFAULT_CNT);
//        key_update(&KeyBoard.D, Referee.keyboard.keyboard_value & KEY_D, DEFAULT_CNT);
//        key_update(&KeyBoard.Q, Referee.keyboard.keyboard_value & KEY_Q, DEFAULT_CNT);
//        key_update(&KeyBoard.E, Referee.keyboard.keyboard_value & KEY_E, DEFAULT_CNT);
//        key_update(&KeyBoard.R, Referee.keyboard.keyboard_value & KEY_R, DEFAULT_CNT);
//        key_update(&KeyBoard.F, Referee.keyboard.keyboard_value & KEY_F, DEFAULT_CNT);
//        key_update(&KeyBoard.G, Referee.keyboard.keyboard_value & KEY_G, DEFAULT_CNT);
//        key_update(&KeyBoard.CTRL, Referee.keyboard.keyboard_value & KEY_CTRL, DEFAULT_CNT);
//        key_update(&KeyBoard.SHIFT, Referee.keyboard.keyboard_value & KEY_SHIFT, DEFAULT_CNT);
//        key_update(&KeyBoard.B, Referee.keyboard.keyboard_value & KEY_B, DEFAULT_CNT);
//        key_update(&KeyBoard.C, Referee.keyboard.keyboard_value & KEY_C, DEFAULT_CNT);
//        key_update(&KeyBoard.V, Referee.keyboard.keyboard_value & KEY_V, DEFAULT_CNT);
//        key_update(&KeyBoard.X, Referee.keyboard.keyboard_value & KEY_X, DEFAULT_CNT);
//        key_update(&KeyBoard.Z, Referee.keyboard.keyboard_value & KEY_Z, DEFAULT_CNT);
        /*鼠标部分*/
        key_update(&KeyBoard.Mouse_l, Referee.keyboard.left_button_down  & MOUSE_YES, MOUSE_CLICK_L_CNT);
        key_update(&KeyBoard.Mouse_r, Referee.keyboard.right_button_down & MOUSE_YES, MOUSE_CLICK_R_CNT);
    }

//    根据遥控器更改键鼠的按键状态
    if(control_flag == RC_ONLINE)
    {
        /*键盘部分*/
        key_update(&KeyBoard.W, rc_ctrl.key.v & KEY_W, DEFAULT_CNT);//DEFAULT_CNT 为按键检测为长按的默认时间
        key_update(&KeyBoard.A, rc_ctrl.key.v & KEY_A, DEFAULT_CNT);
        key_update(&KeyBoard.S, rc_ctrl.key.v & KEY_S, DEFAULT_CNT);
        key_update(&KeyBoard.D, rc_ctrl.key.v & KEY_D, DEFAULT_CNT);
        key_update(&KeyBoard.Q, rc_ctrl.key.v & KEY_Q, DEFAULT_CNT);
        key_update(&KeyBoard.E, rc_ctrl.key.v & KEY_E, DEFAULT_CNT);
        key_update(&KeyBoard.R, rc_ctrl.key.v & KEY_R, DEFAULT_CNT);
        key_update(&KeyBoard.F, rc_ctrl.key.v & KEY_F, DEFAULT_CNT);
        key_update(&KeyBoard.G, rc_ctrl.key.v & KEY_G, DEFAULT_CNT);
        key_update(&KeyBoard.CTRL, rc_ctrl.key.v & KEY_CTRL, DEFAULT_CNT);
        key_update(&KeyBoard.SHIFT, rc_ctrl.key.v & KEY_SHIFT, DEFAULT_CNT);
        key_update(&KeyBoard.B, rc_ctrl.key.v & KEY_B, DEFAULT_CNT);
        key_update(&KeyBoard.C, rc_ctrl.key.v & KEY_C, DEFAULT_CNT);
        key_update(&KeyBoard.V, rc_ctrl.key.v & KEY_V, DEFAULT_CNT);
        key_update(&KeyBoard.X, rc_ctrl.key.v & KEY_X, DEFAULT_CNT);
        key_update(&KeyBoard.Z, rc_ctrl.key.v & KEY_Z, DEFAULT_CNT);
        /*鼠标部分*/
        key_update(&KeyBoard.Mouse_l, rc_ctrl.mouse.press_l & MOUSE_YES, MOUSE_CLICK_L_CNT);
        key_update(&KeyBoard.Mouse_r, rc_ctrl.mouse.press_r & MOUSE_YES, MOUSE_CLICK_R_CNT);

    }

    //图传链路控制键鼠
    if(control_flag == VT_ONLINE)
    {
        /*键盘部分*/
        key_update(&KeyBoard.W, Referee.keyboard.keyboard_value & KEY_W, DEFAULT_CNT);//DEFAULT_CNT 为按键检测为长按的默认时间
        key_update(&KeyBoard.A, Referee.keyboard.keyboard_value & KEY_A, DEFAULT_CNT);
        key_update(&KeyBoard.S, Referee.keyboard.keyboard_value & KEY_S, DEFAULT_CNT);
        key_update(&KeyBoard.D, Referee.keyboard.keyboard_value & KEY_D, DEFAULT_CNT);
        key_update(&KeyBoard.Q, Referee.keyboard.keyboard_value & KEY_Q, DEFAULT_CNT);
        key_update(&KeyBoard.E, Referee.keyboard.keyboard_value & KEY_E, DEFAULT_CNT);
        key_update(&KeyBoard.R, Referee.keyboard.keyboard_value & KEY_R, DEFAULT_CNT);
        key_update(&KeyBoard.F, Referee.keyboard.keyboard_value & KEY_F, DEFAULT_CNT);
        key_update(&KeyBoard.G, Referee.keyboard.keyboard_value & KEY_G, DEFAULT_CNT);
        key_update(&KeyBoard.CTRL, Referee.keyboard.keyboard_value & KEY_CTRL, DEFAULT_CNT);
        key_update(&KeyBoard.SHIFT, Referee.keyboard.keyboard_value & KEY_SHIFT, DEFAULT_CNT);
        key_update(&KeyBoard.B, Referee.keyboard.keyboard_value & KEY_B, DEFAULT_CNT);
        key_update(&KeyBoard.C, Referee.keyboard.keyboard_value & KEY_C, DEFAULT_CNT);
        key_update(&KeyBoard.V, Referee.keyboard.keyboard_value & KEY_V, DEFAULT_CNT);
        key_update(&KeyBoard.X, Referee.keyboard.keyboard_value & KEY_X, DEFAULT_CNT);
        key_update(&KeyBoard.Z, Referee.keyboard.keyboard_value & KEY_Z, DEFAULT_CNT);
        /*鼠标部分*/
        key_update(&KeyBoard.Mouse_l, Referee.keyboard.left_button_down  & MOUSE_YES, MOUSE_CLICK_L_CNT);
        key_update(&KeyBoard.Mouse_r, Referee.keyboard.right_button_down & MOUSE_YES, MOUSE_CLICK_R_CNT);
    }

    if(control_flag == VT_ONLINE) //图传链路模拟遥控器拨杆
    {
        if(KeyBoard.Z.status==KEY_PRESS)//按Z键失能
        {
            rc_ctrl.rc.s[RC_s_L] = RC_SW_DOWN;
            rc_ctrl.rc.s[RC_s_R] = RC_SW_DOWN;
        }
        if(KeyBoard.B.status==KEY_PRESS)//按B键使能
        {
            rc_ctrl.rc.s[RC_s_L] = RC_SW_MID;
            rc_ctrl.rc.s[RC_s_R] = RC_SW_MID;
        }
        if(KeyBoard.Q.status == KEY_PRESS){   //防止遥控器离线时左拨杆在下,然后图传控制按q开摩擦轮的时候一直开拨弹轮
            rc_ctrl.rc.s[RC_s_L] = RC_SW_MID;
        }
    }


}

//根据遥控器更改键鼠的按键状态
static void key_update(Key *key, uint16_t key_status, uint16_t cnt) {
    if (key_status) {
        key->press_cnt++;
        if (key->status == KEY_RELAX) {
            key->last_status = KEY_RELAX;
            key->status = KEY_DOWN;
        } else if (key->status == KEY_DOWN && (HAL_GetTick() - key->click_cnt) > 100) {
            key->click_cnt = HAL_GetTick();
            key->last_status = KEY_DOWN;
            key->status = KEY_CLICK;
            if (key->click_flag == 1) {
                key->click_flag = 0;
            } else if (key->click_flag == 0) {
                key->click_flag = 1;
            }
        } else {
            key->status = KEY_RELAX;
        }

        if (key->press_cnt > cnt) {
            key->last_status = key->status;
            key->status = KEY_PRESS;
        }

    } else {
        key->last_status = key->status;
        key->status = KEY_RELAX;
        key->press_cnt = 0;
    }

}
