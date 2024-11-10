//
// Created by kentl on 24-11-9.
//

#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

class Motor {
    float ratio_;// 电机减速比
    float angle_;
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
    float max_angle_;
    float min_angle_;
public:
    uint16_t CANID;
    void canRxMsgCallback(uint8_t rx_data[8]);
    void store_angle_new();
    void init(uint16_t canid,float ratio,float max_angle,float min_angle);
    friend void motor_calc();

};

void motor_handle();

void motor_calc();

void motor_package_send();

#endif //MOTOR_H
