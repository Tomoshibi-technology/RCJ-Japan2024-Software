/*
 * ws2812.h
 *
 *  Created on: Mar 20, 2024
 *      Author: famil
 */

#ifndef INC_WS2812_H_
#define INC_WS2812_H_
#include <stdint.h>
#include "main.h"

class WS2812
{
private:
    uint8_t TIM_CHANNEL_X;
    TIM_HandleTypeDef* HTIM;
    DMA_HandleTypeDef* HDMA;

    #define HIGH 10
    #define LOW 5
    #define NUM_PIXELS 257

    uint8_t rgb_buf[NUM_PIXELS][3];
    uint8_t wr_buf[48]={0};
    uint_fast8_t wr_buf_p = 0;

public:
    WS2812(TIM_HandleTypeDef* htim, uint32_t tim_channel_x, DMA_HandleTypeDef* hdma);
    void show();
    void clear();
    void set_hsv(uint8_t id, int h, int s, int v);
    void set_rgb(uint8_t id, uint8_t r, uint8_t g,uint8_t b);
    void do_forwardRewrite();
    void do_backRewrite();
};







#endif /* INC_WS2812_H_ */
