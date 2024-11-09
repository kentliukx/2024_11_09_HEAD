//
// Created by kentl on 24-11-9.
//
#include "dma.h"
#include "usart.h"
#include "remotecontrol.h"
extern uint8_t buffer[20];

void remotecontrol_handle()
{
    if(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE)==1)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart3);
        HAL_UART_DMAStop(&huart3);
        uint8_t len=20-__HAL_DMA_GET_COUNTER(huart3.hdmarx);
        HAL_UART_Transmit_DMA(&huart3,buffer,len);
        HAL_UART_Receive_DMA(&huart3,buffer,20);
    }
}

void remotecontrol_calc()
{

}