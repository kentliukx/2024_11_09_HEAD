//
// Created by kentl on 24-11-10.
//
#include "../Inc/PID.h"
#include "main.h"
extern uint8_t num_of_motors;
PID motor_pid[1];
void PID_init()
{
    motor_pid[0].init(1400,1,22000,200,25000);//10度专用参数

}




void PID::init(float kp, float ki, float kd, float i_max_, float out_max_)
{
    kp_ = kp;
    ki_ = ki;
    kd_ = kd;
    i_max = i_max_;
    out_max = out_max_;
    err_ = 0;
    err_sum_ = 0;
    last_err_ = 0;
}

float PID::pidcalc(float ref, float fdb)
{
    ref_ = ref;
    fdb_ = fdb;
    last_err_=err_;
    err_=ref_-fdb_;
    if(err_sum_+err_>i_max) err_sum_=i_max;
    else if(err_sum_+err_<-i_max) err_sum_=-i_max;
    else err_sum_+=err_;
    pout_=err_*kp_;
    iout_=err_sum_*ki_;
    err_d_=err_-last_err_;
    dout_=err_d_*kd_;

    fdf_=-24.89*fdb_+5544;

    if(pout_+dout_+iout_+fdf_>out_max) output_=out_max;
    else if(pout_+dout_+iout_+fdf_<-out_max) output_=-out_max;
    else output_=pout_+dout_+iout_+fdf_;
    return output_;
}