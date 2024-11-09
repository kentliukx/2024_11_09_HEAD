//
// Created by kentl on 24-11-9.
//
#include "dma.h"
#include "usart.h"
#include "../Inc/remotecontrol.h"
extern uint8_t buffer[20];
extern uint16_t rx_message[6];

void remotecontrol_handle()
{
    if(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE)==1)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart3);
        HAL_UART_DMAStop(&huart3);
//        uint8_t len=20-__HAL_DMA_GET_COUNTER(huart3.hdmarx);
//        HAL_UART_Transmit_DMA(&huart3,buffer,len);
        HAL_UART_Receive_DMA(&huart3,buffer,20);
    }
}

void remotecontrol_calc()
{
    rx_message[0] = ((int16_t)buffer[0] | ((int16_t)buffer[1] << 8)) & 0x07FF;
    rx_message[1] = (((int16_t)buffer[1] >> 3) | ((int16_t)buffer[2] << 5)) & 0x07FF;
    rx_message[2] = (((int16_t)buffer[2] >> 6) | ((int16_t)buffer[3] << 2) | ((int16_t)buffer[4] << 10)) & 0x07FF;
    rx_message[3] = (((int16_t)buffer[4] >> 1) | ((int16_t)buffer[5]<<7)) & 0x07FF;
    rx_message[4] = ((buffer[5] >> 4) & 0x000C) >> 2;
    rx_message[5] = ((buffer[5] >> 4) & 0x0003);
}