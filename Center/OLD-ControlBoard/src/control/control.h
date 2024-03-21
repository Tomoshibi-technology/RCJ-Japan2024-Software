#ifndef _H_CONTROL_
#define _H_CONTROL_

#include <Arduino.h>

#define VERSION 1.01

#include "./speaker/speaker.h"
#define SPEAKER_PIN PB10
SPEAKER speaker(SPEAKER_PIN);

#include "./oled/oled.h"
#include <U8g2lib.h>
#define SDA_PIN PB9
#define SCL_PIN PB8
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0,  SCL_PIN, SDA_PIN, U8X8_PIN_NONE);
OLED oled(&u8g2);

#include "./power/power.h"
#define VOL_PIN PB1
#define VOL_MAX 12.6
#define VOL_MIN 11.1
POWER power(VOL_PIN, VOL_MAX, VOL_MIN);

#include "./dip/dip.h"
#define DIP_PIN0 PA8
#define DIP_PIN1 PA11
#define DIP_PIN2 PA12
#define DIP_PIN3 PA15
DIP dip(DIP_PIN0, DIP_PIN1, DIP_PIN2, DIP_PIN3);

#include "./button/button.h"
#define BUTTON_PIN0 PB5
#define BUTTON_PIN1 PB6
#define BUTTON_PIN2 PB7
BUTTON button(BUTTON_PIN0, BUTTON_PIN1, BUTTON_PIN2);

const String name[16] = {
	"Center",
	"Display",
	"ArmA",
	"ArmB",
	"LEDPole",
	"Robot5",
	"Robot6",
	"Robot7",
	"Robot8",
	"Robot9",
	"Robot10",
	"Robot11",
	"Robot12",
	"Robot13",
	"Robot14",
	"Robot15"
};


void control_init(){
	oled.init();
	delay(500);
	oled.display_version(VERSION);
	oled.show();

	power.init();
	
	dip.init();

	speaker.init();
	speaker.boot_music();

	button.init();

	delay(1000);
}

void control_loop(){
	// if(twelite.read()){ //tweliteから受信成功したら1を返す
	// 	PC.print(micros() - loop_timer);
	// 	for(int i=0; i<4; i++){
	// 		PC.print("  :");
	// 		PC.print(twelite.receive_data[i]);
	// 	}
	// 	PC.println();
	// }
	
	// //ーーーーーーーーーー効果音と光ーーーーーーーーーー

	bool led_flg = 0;
	if(btn_val[0]){ 
		speaker.ring(C6);
		led_flg = 1;
	}
	if(btn_val[1]) {
		speaker.ring(E6);
		led_flg = 1;
	}
	if(btn_val[2]){
		speaker.ring(G6);
		led_flg = 1;
	}
	if(!led_flg){
		speaker.mute();
	}



}


#endif


