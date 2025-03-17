//
// Created by liuli on 25-3-10.
//

#include <math.h>
#include "ramp.h"

/**
 * @brief 初始化斜坡函数发生器
 *
 * @param ramp_source_type 斜坡函数结构体指针
 * @param increase 上升斜率（正值变化率）
 * @param decrease 下降斜率（正值变化率，实际方向由算法决定）
 *
 * @note 初始化时会重置所有状态量为0，并设置斜坡变化率参数
 *       特别适用于需要渐变控制的场景（如超级电容充放电控制）
 */
void ramp_init(ramp_function_source_t *ramp_source_type, fp32 increase, fp32 decrease)
{
    ramp_source_type->increase_value = increase;   // 设置上升步长（超级电容场景可增大）
    ramp_source_type->decrease_value = decrease;   // 设置下降步长
    ramp_source_type->rampFirst = ramp_first_real; // 设置斜坡初始状态标志
    ramp_source_type->target = 0.0f;               // 目标值归零
    ramp_source_type->out = 0.0f;                  // 当前输出值归零
    ramp_source_type->last_out = 0.0f;             // 上一次输出值归零
}

/**
 * @brief 斜坡函数计算（带方向感知的渐变算法）
 *
 * @param ramp_source_type 斜坡函数结构体指针
 * @param target 新的目标输入值
 *
 * @note 算法特性：
 * 1. 支持正负双向渐变控制
 * 2. 上升和下降可分别设置不同斜率
 * 3. 自动识别运动方向切换时的减速需求
 * 4. 接近目标值时自动切换为精确输出
 */
void ramp_calc(ramp_function_source_t *ramp_source_type, fp32 target)
{
    /*-- 首次运行特殊处理 --*/
    if (ramp_source_type->rampFirst == ramp_first_real)
    {
        /* 当当前值在目标值和上次输出构成的区间内时，保持当前值
           （用于处理初始化时的突变情况）*/
        if ((ramp_source_type->target >= ramp_source_type->now_real &&
             ramp_source_type->now_real >= ramp_source_type->last_out) ||
            (ramp_source_type->target <= ramp_source_type->now_real &&
             ramp_source_type->now_real <= ramp_source_type->last_out))
        {
            ramp_source_type->out = ramp_source_type->now_real;
        }
    }

    ramp_source_type->target = target; // 更新目标值

    /*-- 方向感知渐变算法 --*/
    if (ramp_source_type->last_out > 0.0f) // 当前为正值状态
    {
        if (ramp_source_type->target > ramp_source_type->last_out)
        {
            // 正值加速阶段（向更大正值变化）
            if (fabs(ramp_source_type->last_out - ramp_source_type->target) >
                ramp_source_type->increase_value)
            {
                ramp_source_type->out += ramp_source_type->increase_value;
            }
            else // 进入减速区时直接到达目标
            {
                ramp_source_type->out = ramp_source_type->target;
            }
        }
        else if (ramp_source_type->target < ramp_source_type->last_out)
        {
            // 正值减速阶段（向更小值或负值变化）
            if (fabs(ramp_source_type->last_out - ramp_source_type->target) >
                ramp_source_type->decrease_value)
            {
                ramp_source_type->out -= ramp_source_type->decrease_value;
            }
            else
            {
                ramp_source_type->out = ramp_source_type->target;
            }
        }
    }
    else if (ramp_source_type->last_out < 0.0f) // 当前为负值状态
    {
        if (ramp_source_type->target < ramp_source_type->last_out)
        {
            // 负值加速阶段（向更小负值变化）
            if (fabs(ramp_source_type->last_out - ramp_source_type->target) >
                ramp_source_type->increase_value)
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
            // 负值减速阶段（向更大值或正值变化）
            if (fabs(ramp_source_type->last_out - ramp_source_type->target) >
                ramp_source_type->decrease_value)
            {
                ramp_source_type->out += ramp_source_type->decrease_value;
            }
            else
            {
                ramp_source_type->out = ramp_source_type->target;
            }
        }
    }
    else // 当前为零值状态
    {
        if (ramp_source_type->target > ramp_source_type->last_out)
        {
            // 零值正向启动（从零开始增加）
            if (fabs(ramp_source_type->last_out - ramp_source_type->target) >
                ramp_source_type->increase_value)
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
            // 零值负向启动（从零开始减小）
            if (fabs(ramp_source_type->last_out - ramp_source_type->target) >
                ramp_source_type->increase_value)
            {
                ramp_source_type->out -= ramp_source_type->increase_value;
            }
            else
            {
                ramp_source_type->out = ramp_source_type->target;
            }
        }
    }

    /*-- 状态更新 --*/
    ramp_source_type->last_out = ramp_source_type->out; // 保存本次输出值供下次计算使用
}

void set_ramp_increase_decrease(ramp_function_source_t *ramp_source_type, fp32 increase,fp32 decrease)
{
    ramp_source_type->increase_value=increase;
    ramp_source_type->decrease_value=decrease;
}
