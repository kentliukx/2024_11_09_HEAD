//
// Created by kentl on 24-11-9.
//
#include "../Inc/motor.h"
#include "../Inc/PID.h"
#include "can.h"
#include "main.h"


extern uint8_t CAN_rx_message[20], CAN_tx_message[20];
extern CAN_RxHeaderTypeDef rx_header;
extern CAN_TxHeaderTypeDef tx_header;
extern uint8_t num_of_motors;
extern uint32_t sent_in_mailbox_num;
extern uint16_t DBUS_message[6];

Motor motor[1];
extern PID motor_pid[1];

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan)//中断回调
{
    HAL_CAN_GetRxMessage(&hcan1,CAN_RX_FIFO0,&rx_header,CAN_rx_message);
    motor_handle();
}



void motor_handle()
{
    for(int i=0;i<num_of_motors;i++)
        if(rx_header.StdId==motor[i].CANID)
        {
            motor[i].canRxMsgCallback(CAN_rx_message);
            break;
        }
}

void motor_calc()
{
    int16_t tgt_current;
    float ref=((float)DBUS_message[0]-364)/1320*(motor[0].max_angle_-motor[0].min_angle_)+motor[0].min_angle_;
    float fdb=motor[0].angle_;
    tgt_current=motor_pid[0].pidcalc(ref,fdb);
    CAN_tx_message[6]=tgt_current>>8;
    CAN_tx_message[7]=tgt_current&0xff;
}

void motor_package_send()
{
    tx_header.StdId=0x1FF;
    tx_header.IDE=CAN_ID_STD;
    tx_header.RTR=CAN_RTR_DATA;
    tx_header.DLC=8;

    if(DBUS_message[5]==2)//stop
        for(int i=0;i<8;i++) CAN_tx_message[i]=0;
    HAL_CAN_AddTxMessage(&hcan1,&tx_header,CAN_tx_message,&sent_in_mailbox_num);
}










void motor_init()
{
    motor[0].init(0x208,1,185,150);
}

void Motor::init(uint16_t canid,float ratio,float max_angle,float min_angle)
{
    CANID=canid;
    ratio_=ratio;
    max_angle_=max_angle;
    min_angle_=min_angle;

    last_ecd_angle_=0;
    angle_=0;
    angle_new.angle=0;
    angle_new.r=0;
}

void Motor::canRxMsgCallback(uint8_t rx_message[8])
{
    last_ecd_angle_=ecd_angle_;

    ecd_angle_=360.0/8191*(rx_message[0]*256+rx_message[1]);
    rotate_speed_=rx_message[2]*256+rx_message[3];
    current_=rx_message[4]*256+rx_message[5];
    temp=rx_message[6];

    if(ecd_angle_>last_ecd_angle_-180) delta_ecd_angle_=ecd_angle_-last_ecd_angle_;
    else delta_ecd_angle_=ecd_angle_-last_ecd_angle_+360;
    delta_angle_=delta_ecd_angle_*ratio_;
    store_angle_new();//保存电机输出端累计转过角度
    angle_+=delta_angle_;
}

void Motor::store_angle_new(){
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