#include <Arduino.h>
#include <HardwareSerial.h>

#define VERSION 1.00

#include "./ser_ctrl/ser_ctrl.h"
HardwareSerial ctrl(2);
CTRL ser_ctrl(&ctrl);  

#include "./power/power.h"
#define VOL_PIN 32 //これ、無線機能のピンとかぶってるから注意
#define VOL_MAX 12.6
#define VOL_MIN 11.1
POWER power(VOL_PIN, VOL_MAX, VOL_MIN);


#include "./fled/RMT_WS2812.hpp"
#define LED_A_PIN GPIO_NUM_18  // The data pin that the WS2812 strips are connected to.32
#define LED_B_PIN GPIO_NUM_21
#define LED_C_PIN GPIO_NUM_19
#define LED_Circuit_PIN GPIO_NUM_15
#define LED_NUM 116

RMT_WS2812 tape_led[3] = {
	RMT_WS2812(RMT_CHANNEL_0, LED_A_PIN, 116),
	RMT_WS2812(RMT_CHANNEL_1, LED_B_PIN, 116),
	RMT_WS2812(RMT_CHANNEL_2, LED_C_PIN, 116),
};

RMT_WS2812 circuit_led(RMT_CHANNEL_3, LED_Circuit_PIN, 10);


void voltage_set(int voltage, int percentage);

int ID;

void setup() {
	Serial.begin(115200);
	Serial.println("Hello World");
	Serial.println("Version: " + String(VERSION));


	for(int i=0; i<3; i++){
		tape_led[i].begin();
		tape_led[i].clear();
	}
	circuit_led.begin();
	circuit_led.clear();

	ser_ctrl.init();
	power.init();

	pinMode(35, INPUT); //mic


	pinMode(4,INPUT);
	pinMode(27,INPUT);
	pinMode(33,INPUT);
	pinMode(39,INPUT);
	pinMode(36,INPUT);

	ID = digitalRead(4)*1 + digitalRead(27)*2 + digitalRead(33)*4 + digitalRead(39)*8 + digitalRead(36)*16;

}

void led_set(int tape_n, int num, int H, int S, int V);
 void led_set_high(int High, int Hue, int Sat, int Bri);
 void led_set_all(int Hue, int Sat, int Bri);
 void led_set_star(int High, int Hue, int Sat, int Bri, bool isDown);
 void led_set_band(int center, int width, int Hue, int Sat, int Bri);

 uint8_t pre_mic_value = 0;
 int beat_count = 0;
 int target_raw_count = 32;

 int up_star_time = 0;
 int down_star_time = 0;

 long my_10msec = 0;
 long pre_10msec = 0;
 bool flg_10msec = false;

 int mode4_n =0;
 int mode5_time =0;

 int mode6_count = 0;
 int start6 = 0;
 int width6 = 1;

 bool mode6_flg = false;

 int finish_V=32;
 bool flg_beat = false;

long music_n = 0;

void loop() {
	if(millis()-pre_10msec > 100){
		pre_10msec = millis();
		my_10msec++;
		flg_10msec = true;
	}else{
		flg_10msec = false;
	}
	Serial.print(my_10msec);
	Serial.print("   ");

	// Serial.print("voltage: " + String(power.voltage()) + " " + String(power.percentage()) + "  ");

	int mic_value = analogRead(35)-1000; //0-2500
	// int mic_value = log10(analogRead(35)-1000);
	mic_value = mic_value*0.4 + pre_mic_value*0.6;
	music_n++;
	int ledHigh;
	if((ID==1 || ID ==2) || (ID==7 || ID ==8)){
		ledHigh = mic_value/22;
	}else if(ID==3 || ID ==5){
		ledHigh = 29 + 28*sin(music_n/15.0);
	}else if(ID==4 || ID ==6){
		ledHigh = 29 - 28*sin(music_n/15.0);
	}
	Serial.print("___");
	Serial.println(ledHigh);

	//　受信するよ 
	ser_ctrl.read();
	for(int i=7; i<11; i++){
		Serial.print(ser_ctrl.data[i]);
		Serial.print(" ");
	}
	// Serial.println();

		// 整理するよ
 	int mode = ser_ctrl.data[7] - 5;
 	long raw_count = ser_ctrl.data[9]-5 + (ser_ctrl.data[8]-5)*240;
 	int ledHue = ser_ctrl.data[10]; 

 	// beat_count	計算するよ	
 	if(raw_count==0){
 		target_raw_count = 32;
 		beat_count = 0;
 		mode4_n = 0;
 		mode5_time = 0;
		finish_V=32;
		music_n = 0;
 	}
 	if(raw_count > target_raw_count){
 		beat_count++;
 		target_raw_count += 32;
 		flg_beat = true;
 	}else{
 		flg_beat = false;
 	}
 	Serial.print("  ");
 	Serial.print(raw_count);
 	Serial.print("  ");
 	Serial.println(beat_count);
	

	// ここはいつも同じ
	circuit_led.setBrightness(250);
	for(int i=0; i<10; i++){
		circuit_led.setPixel_hsv(i, ser_ctrl.data[10], 250, 100);
	} 

 	//　modeごとに光らせる。
 	if(mode == 1){ //Poleメイン
 		led_set_all(0,10,5);
 		led_set_high((beat_count-2)*2, ledHue,250,150);
 	}else if(mode == 2){ //Armメイン 
 		if(up_star_time==0)up_star_time = my_10msec;
 		led_set_all(ledHue,200,20);
 	}else if(mode == 3){ //Poleメイン
 		//点滅して色回そう
 		int myHue;
 		if(ID%2 == 0){
 			myHue = ledHue;
 		}else{
 			myHue = ledHue + 127;
 		}
 		if(beat_count%2 == 0){
 			myHue += 127;
 		}
 		led_set_all(myHue+127, 150, 150);
 	}else if(mode == 4){  //Movingメイン
 		led_set_all(ledHue,200,20);
 		if((ID+mode4_n)%2 == 1){
 			if(up_star_time==0 && down_star_time == -10){
 				up_star_time = my_10msec;
 				mode4_n += 1;
 			}
 		}else{
 			if(down_star_time== -10 && up_star_time == 0){
 				down_star_time = my_10msec;
 				mode4_n += 1;
 			}
 		}

 	}else if(mode == 5){ //みんなで
 		led_set_all(0,15,5);
 		if(mode5_time == 0){
 			mode5_time = my_10msec;
 		}
 		int myheight = my_10msec - mode5_time;
 		for(int num=0; num<myheight; num++){
 			led_set((num+beat_count)%6, num, ledHue,250,150);
 		}
 		for(int num=0; num<myheight-20; num++){
 			led_set((num+2+my_10msec)%6, num, ledHue+85,250,150);
 		}
 		for(int num=0; num<myheight-40; num++){
 			led_set((num+4+my_10msec)%6, num, ledHue+170,250,150);
 		}

 	}else if(mode == 6){ // ArmとPole
 		//led_set_all(0,10,5);
 		if(flg_10msec){
 			mode6_count++;
 		}

 		if(up_star_time==0 && ((193+ID==beat_count || 220+ID==beat_count)|| 240+ID==beat_count))up_star_time = my_10msec;
 		led_set_all(ledHue,200,20);

 	}else if(mode == 7){ // 全部、手拍子フェーズ
 		led_set_all(0,15,5);
 		led_set_high(ledHigh, ledHue, 250, 200);
 	}else if(mode == 8){ // 色消していく
 		if(flg_beat){
 			finish_V--;
 			if(finish_V<5){
 				finish_V = 0;
 			}
 		}
 		led_set_all(ledHigh,15,finish_V);


 	}else if(mode == 9){
 		led_set_all(0,0,0);
 	}else{
 		led_set_all(ledHue,220,150);
	}


	// 流れ星の処理
	if(up_star_time > 0){
		led_set_star(my_10msec- up_star_time, 50,200,250, false);
	}
	if(my_10msec-up_star_time > 63){
		up_star_time = 0;
	}
	if(down_star_time > -5){
		led_set_star(63 - (my_10msec-down_star_time), 50,200,250,true);
	}
	if(my_10msec-down_star_time > 67){
		down_star_time = -10;
	}
	// led_set_star(30,50,200,200);
		// led_set(0, 30, 50, 0, 10);


	// 最後
	for(int i=0; i<3; i++){
		tape_led[i].refresh();
	}

	voltage_set(power.voltage(), power.percentage());
	circuit_led.refresh();
}


void led_set(int tape_n, int num, int H, int S, int V){
	bool oddflg = false;
	if(tape_n == 0){
		tape_n = 0;
	}else if(tape_n == 1){
		tape_n = 0;
		oddflg = true;
	}else if(tape_n == 2){
		tape_n = 1;
	}else if(tape_n == 3){
		tape_n = 1;
		oddflg = true;
	}else if(tape_n == 4){
		tape_n = 2;
	}else if(tape_n == 5){
		tape_n = 2;
		oddflg = true;
	}
	if(0<=num && num<=57){
		if(!oddflg){
				tape_led[tape_n].setPixel_hsv(num,H,S,V);
		}else{
				tape_led[tape_n].setPixel_hsv(115-num,H,S,V);
		}
	}
}

void led_set_high(int High, int Hue, int Sat, int Bri){
	for(int num=0;num<58;num++){
		if(num<High){
			for(int tape_num=0; tape_num<6; tape_num++){
				led_set(tape_num, num, Hue, Sat, Bri);
			}
		}
	}
}


void led_set_all(int Hue, int Sat, int Bri){
	for(int tape_num=0;tape_num<6;tape_num++){
		for(int led_num=0; led_num<58;led_num++){
			led_set(tape_num, led_num, Hue, Sat, Bri);
		}
	}
}



void led_set_star(int High, int Hue, int Sat, int Bri, bool isDown){
	if(isDown == 0){
		for(int tape_num=0; tape_num<6; tape_num++){
			led_set(tape_num, High, Hue, Sat, Bri);
			led_set(tape_num, High-1, Hue, Sat*0.7, Bri*0.7);
			led_set(tape_num, High-2, Hue, Sat*0.5, Bri*0.5);
			led_set(tape_num, High-3, Hue, Sat*0.3, Bri*0.3);
			led_set(tape_num, High-4, Hue, Sat*0.1, Bri*0.2);
		}
	}else{
		for(int tape_num=0; tape_num<6; tape_num++){
			led_set(tape_num, High, Hue, Sat, Bri);
			led_set(tape_num, High+1, Hue, Sat*0.7, Bri*0.7);
			led_set(tape_num, High+2, Hue, Sat*0.5, Bri*0.5);
			led_set(tape_num, High+3, Hue, Sat*0.3, Bri*0.3);
			led_set(tape_num, High+4, Hue, Sat*0.1, Bri*0.2);
		}
	}
}


void led_set_band(int center, int width, int Hue, int Sat, int Bri){
	int myfrom = center - width/2;
	int myto = center + width/2;
	for(int tape_num=0; tape_num<6; tape_num++){
		for(int led_num=myfrom; led_num<=myto; led_num++){
			led_set(tape_num, led_num, Hue, Sat, Bri);
		}
	}
}


void voltage_set(int voltage, int percentage){
	//0-5をこれようにする
	
	int num = percentage / 20;
	int power = percentage % 20+5;

	int myh = 20+percentage;
	int mys = 250;
	int myv = 100;
	
	for(int i=0; i<5; i++){
		if(voltage == 0){
			circuit_led.setPixel_hsv(i, 150, 100, 30);
		}else if(percentage < 10){
			circuit_led.setPixel_hsv(i, 0, 255, 255);
		}else if(i < num){
			circuit_led.setPixel_hsv(i, myh, mys, myv);
		}else if(i == num){
			circuit_led.setPixel_hsv(i, myh, mys, myv * power / 25);
		}else{
			circuit_led.setPixel(i, 1, 1, 1);
		}
	}

	circuit_led.refresh();
}
