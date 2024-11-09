//
// Created by kentl on 24-11-9.
//
#include "dma.h"
#include "usart.h"
#include "../Inc/remotecontrol.h"
extern uint8_t DBUS_rx_message[20];
extern uint16_t DBUS_message[6];

void remotecontrol_callback()//中断回调
{
    if(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE)==1)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart3);
        HAL_UART_DMAStop(&huart3);
        HAL_UART_Receive_DMA(&huart3,DBUS_rx_message,20);
    }
}

void remotecontrol_handle()
{
    DBUS_message[0] = ((int16_t)DBUS_rx_message[0] | ((int16_t)DBUS_rx_message[1] << 8)) & 0x07FF;
    DBUS_message[1] = (((int16_t)DBUS_rx_message[1] >> 3) | ((int16_t)DBUS_rx_message[2] << 5)) & 0x07FF;
    DBUS_message[2] = (((int16_t)DBUS_rx_message[2] >> 6) | ((int16_t)DBUS_rx_message[3] << 2) | ((int16_t)DBUS_rx_message[4] << 10)) & 0x07FF;
    DBUS_message[3] = (((int16_t)DBUS_rx_message[4] >> 1) | ((int16_t)DBUS_rx_message[5]<<7)) & 0x07FF;
    DBUS_message[4] = ((DBUS_rx_message[5] >> 4) & 0x000C) >> 2;
    DBUS_message[5] = ((DBUS_rx_message[5] >> 4) & 0x0003);
}