#include "PID.h"

void pid_init(pid_controller_t *pid){
    pid->i_part = 0;
    pid->prior_error = 0;
}

float pid_compute(pid_controller_t *pid, float error, float dt){

        float abs_error = error >= 0 ? error : -error;

        if(abs_error < pid->error_max){
            float p_part = (pid->kp)*error;

            pid->i_part += (pid->ki)*error*dt;
            pid->i_part = pid->i_part > pid->i_part_min ? pid->i_part : pid->i_part_min;
            pid->i_part = pid->i_part < pid->i_part_max ? pid->i_part : pid->i_part_max;

            float d_part = 0;
            if(!pid->is_first_compute){
                    d_part = (pid->kd)*(error - pid->prior_error)*dt;
                    pid->is_first_compute = false;
            }
            pid->prior_error = error;

            float pid_output = p_part + pid->i_part + d_part;
            pid_output = pid_output > pid->output_min ? pid_output : pid->output_min;
            pid_output = pid_output < pid->output_max ? pid_output : pid->output_max;

            return pid_output + pid->ouput_offset;
        }
        else{
            pid->i_part = 0;
            pid->prior_error = 0;
            pid->is_first_compute = true;
            if(error > 0)
                return pid->default_output;
            else
                return -pid->default_output;
        }
}
