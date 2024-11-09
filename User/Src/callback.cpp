//
// Created by kentl on 24-11-9.
//
#include "tim.h"
#include "../Inc/motor.h"
#include "../Inc/remotecontrol.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        remotecontrol_handle();
        motor_handle();
    }
}