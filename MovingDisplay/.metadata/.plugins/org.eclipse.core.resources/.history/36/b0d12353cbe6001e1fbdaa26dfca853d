#include "ws2812b.h"

WS2812B::WS2812B(TIM_HandleTypeDef* htim, uint32_t tim_channel_x, DMA_HandleTypeDef* hdma){
    HTIM = htim;
    TIM_CHANNEL_X = tim_channel_x;
    HDMA =hdma;
}

void WS2812B::set_color(uint16_t PIXEL_NUM, uint8_t red, uint8_t green, uint8_t blue){
	if(PIXEL_NUM<LED_NUM){
		rgb_buf[PIXEL_NUM][0]=green;
		rgb_buf[PIXEL_NUM][1]=red;
		rgb_buf[PIXEL_NUM][2]=blue;
	}
}

void WS2812B::show(){
	pwm_buf_status = 1;
}

void WS2812B::execute(){
	if(pwm_buf_status==1){
		if(HDMA->State != HAL_DMA_STATE_READY){
			HAL_TIM_PWM_Stop_DMA(HTIM, TIM_CHANNEL_X);
		}

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

		HAL_TIM_PWM_Start_DMA(HTIM, TIM_CHANNEL_X, (uint32_t*)pwm_buf, RST+(LED_NUM*24));
		pwm_buf_status = 0;

	}else{

	}
}

void WS2812B::init(){
	for(int i=0; i<RST; i++){
	        pwm_buf[i]=0;
	}

	for(int i=RST; i<LED_NUM*24; i++){
		pwm_buf[i]=LOW;
	}

	HAL_TIM_PWM_Start_DMA(HTIM, TIM_CHANNEL_X, (uint32_t*)pwm_buf, RST+(LED_NUM*24));
}

void WS2812B::clear(){
	for(int i=0; i<LED_NUM; i++){
		rgb_buf[i][0]=0;
		rgb_buf[i][1]=0;
		rgb_buf[i][2]=0;
	}
	pwm_buf_status = 1;
}
