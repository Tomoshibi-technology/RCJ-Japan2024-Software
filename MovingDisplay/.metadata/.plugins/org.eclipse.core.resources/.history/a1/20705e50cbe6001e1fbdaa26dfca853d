#pragma once
#include <stdint.h>
#include "main.h"

class WS2812B{
    private:
        #define RST 50
  	  	#define LED_NUM 128
		#define HIGH 10
		#define LOW 5
        uint8_t TIM_CHANNEL_X;
        TIM_HandleTypeDef* HTIM;
        DMA_HandleTypeDef* HDMA;
        uint8_t rgb_buf[LED_NUM][3];
        uint8_t pwm_buf[RST+(LED_NUM*24)];

    public:
        WS2812B(TIM_HandleTypeDef* htim, uint32_t tim_channel_x, DMA_HandleTypeDef* hdma);
        uint8_t pwm_buf_status;
        void set_color(uint16_t PIXEL_NUM, uint8_t red, uint8_t green, uint8_t blue);
        void show();
        void init();
        void execute();
        void clear();
};
