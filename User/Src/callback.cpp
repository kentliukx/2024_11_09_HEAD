//
// Created by kentl on 24-11-9.
//
#include "../Inc/callback.h"
#include "../Inc/motor.h"
#include "../Inc/remotecontrol.h"
#include "tim.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//开始干活
{
    if (htim->Instance == TIM1)
    {
        motor_calc();
        //motor_package_send();
    }
}