//
// Created by kentl on 24-11-10.
//

#ifndef PID_H
#define PID_H

class PID
{
public:
    void init(float kp, float ki, float kd, float i_max_, float out_max_);
    float pidcalc(float ref,float fdb);
private:
    float kp_, ki_, kd_;
    float i_max, out_max;
    float output_;
    float ref_, fdb_;
    float err_, err_sum_, last_err_,err_d_;
    float pout_, iout_, dout_;
};

#endif //PID_H
