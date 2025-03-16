//
// Created by liuli on 25-3-10.
//

#include <math.h>
#include "ramp.h"

void ramp_init(ramp_function_source_t *ramp_source_type, fp32 increase, fp32 decrease)
{
    ramp_source_type->increase_value=increase;//在使用超级电容时可以变得更大一些
    ramp_source_type->decrease_value=decrease;
    ramp_source_type->rampFirst=ramp_first_real;
    ramp_source_type->target = 0.0f;
    ramp_source_type->out = 0.0f;
    ramp_source_type->last_out=0.0f;
}

void ramp_calc(ramp_function_source_t *ramp_source_type, fp32 target)
{
    if(ramp_source_type->rampFirst==ramp_first_real)
    {
        if ((ramp_source_type->target >= ramp_source_type->now_real && ramp_source_type->now_real>=ramp_source_type->last_out)
            || ( ramp_source_type->target <= ramp_source_type->now_real && ramp_source_type->now_real<=ramp_source_type->last_out))
        {
            ramp_source_type->out=ramp_source_type->now_real;
        }
    }
    ramp_source_type->target = target;

    if (ramp_source_type->last_out > 0.0f)
    {
        if (ramp_source_type->target > ramp_source_type->last_out)
        {
            // 正值加速
            if (fabs(ramp_source_type->last_out - ramp_source_type->target) > ramp_source_type->increase_value)
            {
                ramp_source_type->out += ramp_source_type->increase_value;
            }
            else
            {
                ramp_source_type->out = ramp_source_type->target;
            }
        }
        else if (ramp_source_type->target < ramp_source_type->last_out)
        {
            // 正值减速
            if (fabs(ramp_source_type->last_out - ramp_source_type->target) > ramp_source_type->decrease_value)
            {
                ramp_source_type->out -= ramp_source_type->decrease_value;
            }
            else
            {
                ramp_source_type->out = ramp_source_type->target;
            }
        }
    }
    else if (ramp_source_type->last_out < 0.0f)
    {
        if (ramp_source_type->target < ramp_source_type->last_out)
        {
            // 负值加速
            if (fabs(ramp_source_type->last_out - ramp_source_type->target) > ramp_source_type->increase_value)
            {
                ramp_source_type->out -= ramp_source_type->increase_value;
            }
            else
            {
                ramp_source_type->out = ramp_source_type->target;
            }
        }
        else if (ramp_source_type->target > ramp_source_type->last_out)
        {
            // 负值减速
            if (fabs(ramp_source_type->last_out - ramp_source_type->target) > ramp_source_type->decrease_value)
            {
                ramp_source_type->out += ramp_source_type->decrease_value;
            }
            else
            {
                ramp_source_type->out = ramp_source_type->target;
            }
        }
    }
    else
    {
        if (ramp_source_type->target > ramp_source_type->last_out)
        {
            // 0值正加速
            if (fabs(ramp_source_type->last_out - ramp_source_type->target) > ramp_source_type->increase_value)
            {
                ramp_source_type->out += ramp_source_type->increase_value;
            }
            else
            {
                ramp_source_type->out = ramp_source_type->target;
            }
        }
        else if (ramp_source_type->target < ramp_source_type->last_out)
        {
            // 0值负加速
            if (fabs(ramp_source_type->last_out - ramp_source_type->target) > ramp_source_type->increase_value)
            {
                ramp_source_type->out -= ramp_source_type->increase_value;
            }
            else
            {
                ramp_source_type->out = ramp_source_type->target;
            }
        }
    }

    //善后
    ramp_source_type->last_out=ramp_source_type->out;

}