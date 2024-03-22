/*
 * servo.h
 *
 *  Created on: Mar 17, 2024
 *      Author: tomoh
 */

#ifndef INC_SERVO_H_
#define INC_SERVO_H_
#include "main.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_it.h"
#include <stdint.h>

class STS_SERVO {
    private :
	    uint8_t servo_array [13] = {
            255, 255, 255, 9, 3, 42, 0, 0, 0, 0, 0, 0, 201
        };
        UART_HandleTypeDef* UART;
    public :
        STS_SERVO(uint8_t id, UART_HandleTypeDef* uart);
        void set_position(uint16_t degree);
        void calc_checksum();
        void exe_servo(uint16_t degree);
};


#endif /* INC_SERVO_H_ */