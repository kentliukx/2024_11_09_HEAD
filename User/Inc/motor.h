//
// Created by kentl on 24-11-9.
//

#ifndef MOTOR_H
#define MOTOR_H

#include <sys/_stdint.h>

class M3508_Motor {
    float ratio_;// 电机减速比
    struct {
        int r;
        float angle;
    } angle_new;
    float delta_angle_;// deg 输出端新转动的角度
    float ecd_angle_;// deg 当前电机编码器角度
    float last_ecd_angle_;  // deg 上次电机编码器角度
    float delta_ecd_angle_; // deg 编码器端新转动的角度
    float rotate_speed_;// dps 反馈转子转速
    float current_;
    float temp;
public:
    uint8_t CANID;
    M3508_Motor();
    void canRxMsgCallback(uint8_t rx_data[8]);
    void store_angle_new(float delta_angle_);
};

void motor_handle();

#endif //MOTOR_H
