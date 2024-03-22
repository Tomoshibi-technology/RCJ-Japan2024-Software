#include "ws2812c.h"

WS2812C::WS2812C(TIM_HandleTypeDef* htim, uint32_t tim_channel_x, DMA_HandleTypeDef* hdma){
    HTIM = htim;
    TIM_CHANNEL_X = tim_channel_x;
    HDMA =hdma;
}

void WS2812C::set_rgb(uint16_t PIXEL_NUM, uint8_t red, uint8_t green, uint8_t blue){
	if(PIXEL_NUM<LED_NUM){
		rgb_buf[PIXEL_NUM][0]=green;
		rgb_buf[PIXEL_NUM][1]=red;
		rgb_buf[PIXEL_NUM][2]=blue;
	}
}

void WS2812C::set_hsv(uint16_t PIXEL_NUM, int16_t h , int16_t s, int16_t v){
	uint8_t r, g, b;
	if(s == 0){
		r = g = b = v;
	}else{
		h = h * 6;
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
	set_rgb(PIXEL_NUM, r, g, b);
}

void WS2812C::show(){
	pwm_buf_status = 1;
}

void WS2812C::execute(){
	if(pwm_buf_status==1){
//		HAL_Delay((int)(LED_NUM * 32 / 1000) + 1);
//		if(HDMA->State != HAL_DMA_STATE_READY){
//			HAL_TIM_PWM_Stop_DMA(HTIM, TIM_CHANNEL_X);
//		}
		for(int i=0; i<RST; i++){
			pwm_buf[i]=0;
		}

		for(int i=0; i<LED_NUM; i++){
			for(int j=0; j<3; j++){
				for(int k=7; k>=0; k--){
					int x=(rgb_buf[i][j]>>k)&1;
					if(x==1){pwm_buf[RST+(i*24)+(j*8)+(7-k)]=HIGH;}
					else{pwm_buf[RST+(i*24)+(j*8)+(7-k)]=LOW;}
				}
			}
		}

		HAL_TIM_PWM_Start_DMA(HTIM, TIM_CHANNEL_X, pwm_buf, RST+(LED_NUM*24));
		pwm_buf_status = 0;
		// (uint32_t*)

	}else{

	}
}

void WS2812C::init(){
	for(int i=0; i<RST; i++){
	        pwm_buf[i]=0;
	}

	for(int i=RST; i<LED_NUM*24; i++){
		pwm_buf[i]=LOW;
	}

	HAL_TIM_PWM_Start_DMA(HTIM, TIM_CHANNEL_X, pwm_buf, RST+(LED_NUM*24));
	// (uint32_t*)
}

void WS2812C::clear(){
	for(int i=0; i<LED_NUM; i++){
		rgb_buf[i][0]=0;
		rgb_buf[i][1]=0;
		rgb_buf[i][2]=0;
	}
	pwm_buf_status = 1;
}
