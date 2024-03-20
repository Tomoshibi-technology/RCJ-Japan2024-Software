#pragma once
#include "main.h"
#include "stm32c0xx_hal_conf.h"
#include "stm32c0xx_it.h"
#include <math.h>

class AS5600{
    private:
        I2C_HandleTypeDef* I2C;
        uint8_t AS5600_ADDR = 0x36<<1;
        uint8_t ANGLE_ADDR = 0x0E;
        uint8_t rcvBuf[2];
        long totalAng = 0;
        long ptotalAng = 0;
        uint16_t Angle;
        uint16_t pAngle;

    public:
        AS5600(I2C_HandleTypeDef* i2c);
        long get_travel();
};