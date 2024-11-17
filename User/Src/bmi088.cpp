//
// Created by kentl on 24-11-17.
//
#include "../Inc/bmi088.h"
#include "spi.h"
#include "stdint.h"
#include "stm32f4xx_hal_spi.h"

float x_accel,y_accel,z_accel;
float x_gyro,y_gyro,z_gyro;
uint8_t tx_data[2],rx_data[10];
int16_t accel_range[4]={3,6,12,24}, gyro_range[5]={2000,1000,500,250,125};
int16_t x_temp,y_temp,z_temp;
const uint8_t accel_sel=1,gyro_sel=4;


void bmi088_init()
{
    // Soft Reset ACCEL

    tx_data[0]=0x7E;
    tx_data[1]=0xB6;
    HAL_GPIO_WritePin(CS1_Accel_GPIO_Port, CS1_Accel_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, tx_data, 2, 1000);
    HAL_Delay(1);//very important!!!!
    HAL_GPIO_WritePin(CS1_Accel_GPIO_Port, CS1_Accel_Pin, GPIO_PIN_SET);

    // Soft Reset GYRO

    tx_data[0]=0x14;
    tx_data[1]=0xB6;
    HAL_GPIO_WritePin(CS1_Gyro_GPIO_Port, CS1_Gyro_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, tx_data, 2, 1000);
    HAL_Delay(30);
    HAL_GPIO_WritePin(CS1_Gyro_GPIO_Port, CS1_Gyro_Pin, GPIO_PIN_SET);

    // Switch ACCEL to Normal Mode

    tx_data[0]=0x7D;
    tx_data[1]=0x04;
    HAL_GPIO_WritePin(CS1_Accel_GPIO_Port, CS1_Accel_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, tx_data, 2, 1000);
    HAL_Delay(1);
    HAL_GPIO_WritePin(CS1_Accel_GPIO_Port, CS1_Accel_Pin, GPIO_PIN_SET);

    // set range

    tx_data[0]=0x41;
    tx_data[1]=accel_sel;
    HAL_GPIO_WritePin(CS1_Accel_GPIO_Port, CS1_Accel_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, tx_data, 2, 1000);
    HAL_GPIO_WritePin(CS1_Accel_GPIO_Port, CS1_Accel_Pin, GPIO_PIN_SET);

    tx_data[0]=0x0F;
    tx_data[1]=gyro_sel;
    HAL_GPIO_WritePin(CS1_Gyro_GPIO_Port, CS1_Gyro_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, tx_data, 2, 1000);
    HAL_GPIO_WritePin(CS1_Gyro_GPIO_Port, CS1_Gyro_Pin, GPIO_PIN_SET);
}

void bmi088_read()
{

    tx_data[0]=0x12|0x80;
    HAL_GPIO_WritePin(CS1_Accel_GPIO_Port, CS1_Accel_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, tx_data, 1, 1000);
    HAL_SPI_Receive(&hspi1, rx_data, 7, 1000);
    HAL_GPIO_WritePin(CS1_Accel_GPIO_Port, CS1_Accel_Pin, GPIO_PIN_SET);

    x_temp=rx_data[2]*256+rx_data[1];
    y_temp=rx_data[4]*256+rx_data[3];
    z_temp=rx_data[6]*256+rx_data[5];
    x_accel=(float)accel_range[accel_sel]*x_temp/32768;
    y_accel=(float)accel_range[accel_sel]*y_temp/32768;
    z_accel=(float)accel_range[accel_sel]*z_temp/32768;

    tx_data[0]=0x02|0x80;
    HAL_GPIO_WritePin(CS1_Gyro_GPIO_Port, CS1_Gyro_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, tx_data, 1, 1000);
    HAL_SPI_Receive(&hspi1, rx_data, 7, 1000);
    HAL_GPIO_WritePin(CS1_Gyro_GPIO_Port, CS1_Gyro_Pin, GPIO_PIN_SET);

    x_temp=rx_data[1]*256+rx_data[0];
    y_temp=rx_data[3]*256+rx_data[2];
    z_temp=rx_data[5]*256+rx_data[4];
    x_gyro=(float)gyro_range[gyro_sel]*x_temp/32768;
    y_gyro=(float)gyro_range[gyro_sel]*y_temp/32768;
    z_gyro=(float)gyro_range[gyro_sel]*z_temp/32768;

}
