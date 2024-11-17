//
// Created by kentl on 24-11-9.
//
#include "../Inc/callback.h"
#include "../Inc/motor.h"
#include "../Inc/remotecontrol.h"
#include "tim.h"
#include "../Inc/bmi088.h"

// uint16_t ms;
// int16_t tgt_speed_temp,set_tgt_speed_temp;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//开始干活
{
    if (htim->Instance == TIM1)
    {
        // ms+=1;
        // if(ms==1000) ms=0;//更新毫秒数
        // if(ms%500<250) tgt_speed_temp=set_tgt_speed_temp;
        // else tgt_speed_temp=-set_tgt_speed_temp;//调速度环的函数
        bmi088_read();
        motor_calc();
        motor_package_send();
    }
}