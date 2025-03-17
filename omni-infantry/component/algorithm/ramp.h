//
// Created by liuli on 25-3-10.
//

#ifndef WHEEL_LEG_RAMP_H
#define WHEEL_LEG_RAMP_H

#include "struct_typedef.h"
#include <main.h>

typedef enum
{
    ramp_first_real,
    ramp_first_target
}ramp_first ;

typedef  struct
{
    fp32 target;        //目标数据
    fp32 out;          //输出数据
    fp32 last_out;      //上次输出数据
    fp32 min_value;    //限幅最小值
    fp32 max_value;    //限幅最大值
    fp32 increase_value; //加速度
    fp32 decrease_value; //减速度
    fp32 now_real;      //当前真实值
    ramp_first rampFirst;//真实值/目标值 优先
}__packed ramp_function_source_t;


void ramp_init(ramp_function_source_t *ramp_source_type, fp32 increase, fp32 decrease);
void ramp_calc(ramp_function_source_t *ramp_source_type, fp32 input);
void set_ramp_increase_decrease(ramp_function_source_t *ramp_source_type, fp32 increase,fp32 decrease);




#endif //WHEEL_LEG_RAMP_H
