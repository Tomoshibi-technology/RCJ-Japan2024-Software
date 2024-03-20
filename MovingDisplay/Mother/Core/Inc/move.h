
#pragma once
#include <stdint.h>
#include <math.h>
#define PI 3.1415

class MOVE{
    private:
        uint8_t MOTOR_ID;
        int16_t MOTOR_DEGREE;
        int16_t motor_speed;
        float motor_rate = 0.7;
    
    public:
        MOVE(uint8_t motor_id, int16_t motor_degree);
        void calcurate(int16_t gyro_degree, int16_t goal_degree, int16_t goal_speed);
        void set_array(uint8_t *send_array);
};

