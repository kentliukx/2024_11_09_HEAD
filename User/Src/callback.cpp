//
// Created by kentl on 24-11-9.
//
#include "tim.h"
#include "remotecontrol.h"
#include "motor.h"
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        remotecontrol_calc();
        Motor::speed_calc();
        Motor::package_Send();
    }
}