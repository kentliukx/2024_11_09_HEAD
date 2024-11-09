//
// Created by kentl on 24-11-9.
//
#include "../Inc/motor.h"
#include "can.h"

extern uint8_t CAN_rx_message[20], CAN_tx_message[20];
extern CAN_RxHeaderTypeDef rx_header, tx_header;
M3508_Motor motor[1];
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan)
{
    HAL_CAN_GetRxMessage(&hcan1,CAN_RX_FIFO0,&rx_header,CAN_rx_message);
}

void motor_handle()
{
    if(rx_header.StdId==motor[0].CANID)
        motor[0].canRxMsgCallback(CAN_rx_message);
}


void M3508_Motor::canRxMsgCallback(uint8_t CAN_rx_message[8])
{
    last_ecd_angle_=ecd_angle_;//更新上次编码器角度

    ecd_angle_=(float)CAN_rx_message[0]*256+(float)CAN_rx_message[1];
    rotate_speed_=CAN_rx_message[2]*256+CAN_rx_message[3];
    current_=CAN_rx_message[4]*256+CAN_rx_message[5];
    temp=CAN_rx_message[6];//get数据

    if(ecd_angle_>last_ecd_angle_-180) delta_ecd_angle_=ecd_angle_-last_ecd_angle_;
    else delta_ecd_angle_=ecd_angle_-last_ecd_angle_+360;//获取角度增量or减量,此时已经转过一整圈
    delta_angle_=delta_ecd_angle_*ratio_;
    store_angle_new(delta_angle_);//保存电机累计转过角度
}

void M3508_Motor::store_angle_new(float delta_angle_){
    angle_new.angle+=delta_angle_;
    if(angle_new.angle>=360){
        angle_new.angle-=360;
        angle_new.r+=1;
    }
    if(angle_new.angle<0){
        angle_new.angle+=360;
        angle_new.r-=1;
    }
}

M3508_Motor::M3508_Motor(){
    ratio_=(float)3591/187;
    angle_new.r=0;
    angle_new.angle=0;
    last_ecd_angle_=0;
    ecd_angle_=0;
}