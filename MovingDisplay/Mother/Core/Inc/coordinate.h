#pragma once

#include "main.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_it.h"
#include <stdint.h>

class COORDINATE{
    private:
    uint8_t current__bufnum;

    public:
    COORDINATE();
    int check(int x, int y, uint8_t buf[][3]);//return goal_travel_x
}