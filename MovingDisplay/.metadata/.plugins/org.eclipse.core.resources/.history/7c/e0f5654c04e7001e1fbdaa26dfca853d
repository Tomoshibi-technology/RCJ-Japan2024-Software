/*
 * ws2812.cpp
 *
 *  Created on: Mar 20, 2024
 *      Author: famil
 */

#include "ws2812.h"

WS2812::WS2812(TIM_HandleTypeDef* htim, uint32_t tim_channel_x, DMA_HandleTypeDef* hdma){
    HTIM = htim;
    TIM_CHANNEL_X = tim_channel_x;
    HDMA =hdma;
}


void WS2812::show(){
    if(wr_buf_p != 0 || HDMA->State != HAL_DMA_STATE_READY){
        HAL_TIM_PWM_Stop_DMA(HTIM, TIM_CHANNEL_X);
        wr_buf_p = 0;
        for(uint8_t i=0; i <48; i++){wr_buf[i] = 0;}
        return;
    }
    for(uint_fast8_t i = 0; i < 8; i++){
        wr_buf[i   ] = 0;
        wr_buf[i+ 8] = 0;
        wr_buf[i+16] = 0;

        wr_buf[i+24] = 0;
        wr_buf[i+32] = 0;
        wr_buf[i+40] = 0;
    }
    HAL_TIM_PWM_Start_DMA(HTIM, TIM_CHANNEL_X, (uint32_t *)wr_buf, 48);
    HAL_Delay(8);
}

void WS2812::clear(){
    for(int i=0; i<NUM_PIXELS; i++){
        rgb_buf[i][0]=0;
        rgb_buf[i][1]=0;
        rgb_buf[i][2]=0;
    }
}

void WS2812::set_hsv(uint8_t id, int h, int s, int v){
	int r,g,b;
	if (s == 0) {
	    r = g = b = v;
	}else{
	    h = h * 6; // sector 0 to 5
	    int i = h / 256;
	    int f = h % 256;
	    int p = (v * (256 - s)) / 256;
	    int q = (v * (256 - (s * f) / 256)) / 256;
	    int t = (v * (256 - (s * (256 - f)) / 256)) / 256;

	    switch (i) {
	        case 0: r = v; g = t; b = p; break;
	        case 1: r = q; g = v; b = p; break;
	        case 2: r = p; g = v; b = t; break;
	        case 3: r = p; g = q; b = v; break;
	        case 4: r = t; g = p; b = v; break;
	        default: r = v; g = p;b = q; break;
	    }
	}
	set_rgb(id, r, g, b);

}

void WS2812::set_rgb(uint8_t id, uint8_t r, uint8_t g,uint8_t b){
    rgb_buf[id][0]=r;
    rgb_buf[id][1]=g;
    rgb_buf[id][2]=b;
}

void WS2812::do_forwardRewrite(){
    if(wr_buf_p < NUM_PIXELS) {
        for(uint_fast8_t i = 0; i < 8; i++) {
            wr_buf[i     ] = ((rgb_buf[wr_buf_p][1]>>(7-i))&1) ? HIGH : LOW;
            wr_buf[i +  8] = ((rgb_buf[wr_buf_p][0]>>(7-i))&1) ? HIGH : LOW;
            wr_buf[i + 16] = ((rgb_buf[wr_buf_p][2]>>(7-i))&1) ? HIGH : LOW;
        }
        wr_buf_p++;
    } else if (wr_buf_p < NUM_PIXELS + 2) {
        for(uint8_t i = 0; i < 24; i++){ wr_buf[i] = 0;}
        wr_buf_p++;
    }
}

void WS2812::do_backRewrite(){
    if(wr_buf_p < NUM_PIXELS) {
        for(uint_fast8_t i = 0; i < 8; ++i) {
            wr_buf[i + 24] = ((rgb_buf[wr_buf_p][1]>>(7-i))&1) ? HIGH : LOW;
            wr_buf[i + 32] = ((rgb_buf[wr_buf_p][0]>>(7-i))&1) ? HIGH : LOW;
            wr_buf[i + 40] = ((rgb_buf[wr_buf_p][2]>>(7-i))&1) ? HIGH : LOW;
        }
        wr_buf_p++;
    } else if (wr_buf_p < NUM_PIXELS + 2) {
        for(uint8_t i = 24; i < 48; i++){ wr_buf[i] = 0;};
        wr_buf_p++;
    } else {
        wr_buf_p = 0;
        HAL_TIM_PWM_Stop_DMA(HTIM, TIM_CHANNEL_X);
    }
}



