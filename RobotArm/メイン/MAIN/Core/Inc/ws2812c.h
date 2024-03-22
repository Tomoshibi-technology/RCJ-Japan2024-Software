#pragma once
#include <stdint.h>
#include "main.h"

class WS2812C{
    private:
        #define RST 250
  	  	#define LED_NUM 32
		#define HIGH 20
		#define LOW 10
        uint8_t TIM_CHANNEL_X;
        TIM_HandleTypeDef* HTIM;
        DMA_HandleTypeDef* HDMA;
        uint8_t rgb_buf[LED_NUM][3];
        uint32_t pwm_buf[RST+(LED_NUM*24)];

    public:
        WS2812C(TIM_HandleTypeDef* htim, uint32_t tim_channel_x, DMA_HandleTypeDef* hdma);
        uint8_t pwm_buf_status;
        void set_rgb(uint16_t PIXEL_NUM, uint8_t red, uint8_t green, uint8_t blue);
        void set_hsv(uint16_t PIXEL_NUM, int16_t h, int16_t s, int16_t v);
        void show();
        void init();
        void execute();
        void clear();
};
