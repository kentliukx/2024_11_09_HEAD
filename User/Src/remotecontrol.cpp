//
// Created by kentl on 24-11-9.
//
#include "dma.h"
#include "usart.h"
#include "remotecontrol.h"
extern uint8_t buffer[20];
extern uint16_t rx_message[6];

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
    rx_message[0]=((uint16_t)buffer[0]*8+buffer[1]/32)%2048;
    rx_message[1]=((uint16_t)buffer[1]*64+buffer[2]/4)%2048;
    rx_message[2]=((uint16_t)buffer[2]*512+buffer[3]*2+buffer[4]/128)%2048;
    rx_message[3]=((uint16_t)buffer[4]*16+buffer[5]/16)%2048;
    rx_message[4]=((uint16_t)buffer[5]/4)%4;
    rx_message[5]=((uint16_t)buffer[5])%4;
}