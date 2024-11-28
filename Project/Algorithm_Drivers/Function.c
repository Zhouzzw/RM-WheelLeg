//=====================================================================================
//                    Function[常用函数]
//    Limit(float *Input, float Up, float Down)
//    Ignor(float *Input,  float Up, float Down)
//    Deadline(float *Input,  float Up, float Down)
//=====================================================================================

#include "Function.h"

/**
 * @brief 限幅
 *
 * @param 输入值指针
 * @param 限制范围
 */
void Limit_float(float *Input, float Up, float Down)
{
    if(Up < Down) return;
    if      (*Input > Up)   *Input = Up;
    else if (*Input < Down) *Input = Down;
}
void Limit_int(int *Input, int Up, int Down)
{
    if(Up < Down) return;
    if      (*Input > Up)   *Input = Up;
    else if (*Input < Down) *Input = Down;
}

/**
 * @brief 误差忽略
 *
 * @param 输入值指针
 * @param 忽略上边缘
 * @param 忽略下边缘
 */
void Ignor_float(float *Input,  float Up, float Down)
{
    if(*Input < Up && *Input > Down)
    *Input = 0;
}

/**
 * @brief 死区限制
 *
 * @param 输入值指针
 * @param 死区上边缘
 * @param 死区下边缘
 */
void Deadline_Float(float *Input,  float Up, float Down)
{
    if(Up < Down) return;
    if(*Input < Up && *Input > Down) *Input = 0;
}
