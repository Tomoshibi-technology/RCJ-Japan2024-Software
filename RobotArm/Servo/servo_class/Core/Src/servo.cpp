/*
 * servo.cpp
 *
 *  Created on: Mar 17, 2024
 *      Author: tomoh
 */

#include "servo.h"

STS_SERVO::STS_SERVO(uint8_t id, UART_HandleTypeDef *uart)
{
    servo_array[2] = id;
    UART = uart;
}

void STS_SERVO::set_position(uint16_t degree)
{
    uint16_t bit_degree = 0;
    if (degree > 360)
    {
        degree = 360;
    }
    bit_degree = (degree << 12) / 360;
    servo_array[6] = bit_degree;
    servo_array[7] = bit_degree >> 8;
}

void STS_SERVO::calc_checksum()
{
    uint8_t checksum = 0;
    for (uint8_t i = 2; i <= 11; i++)
    {
        checksum += servo_array[i];
    }
    servo_array[12] = ~checksum;
}

void STS_SERVO::exe_servo(uint16_t degree)
{
    set_position(degree);
    calc_checksum();
    HAL_UART_Transmit(UART, servo_array, 13, 50);
    HAL_Delay(5);
}
