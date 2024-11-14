//
// Created by kentl on 24-11-9.
//
#include "../Inc/callback.h"
#include "../Inc/motor.h"
#include "../Inc/remotecontrol.h"
#include "tim.h"

extern uint16_t ms;
extern int16_t tgt_speed,set_tgt_speed;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//开始干活
{
    if (htim->Instance == TIM1)
    {
        ms+=1;
        if(ms==1000) ms=0;//更新毫秒数

        // if(ms%200<100) tgt_speed=set_tgt_speed;
        // else tgt_speed=-set_tgt_speed;


        motor_calc();
        motor_package_send();
    }
}