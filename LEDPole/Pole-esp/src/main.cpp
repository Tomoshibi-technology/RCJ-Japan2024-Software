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

}


uint8_t ledH = 0;
uint8_t ledHigh = 59;
uint8_t pre_mic_value = 0;

int test = 100;

void led_set(int tape_n, int num, int H, int S, int V);

void loop() {
	// Serial.print("voltage: " + String(power.voltage()) + " " + String(power.percentage()) + "  ");


	int mic_value = analogRead(35)-1000; //0-2500
	mic_value = mic_value*0.4 + pre_mic_value*0.6;
	ledHigh = mic_value/42;
	Serial.print(mic_value);
	Serial.print("   ");
	Serial.println(ledHigh);


	ser_ctrl.read();
	// for(int i=0; i<12; i++){
	// 	Serial.print(ser_ctrl.data[i]);
	// 	Serial.print(" ");
	// }
	// Serial.println();

	circuit_led.setBrightness(250);
	for(int i=0; i<10; i++){
		circuit_led.setPixel_hsv(i, ser_ctrl.data[10], 250, 100);
	}



	for(int num=0;num<58;num++){
		if(num<ledHigh){
			for(int tape_n=0; tape_n<6; tape_n++){
				led_set(tape_n, num, ser_ctrl.data[10],250, 100);
			}
		}else{
			for(int tape_n=0; tape_n<6; tape_n++){
				led_set(tape_n, num, 10,10, 10);
			}
		}
	}

	led_set(5,5,10,10,10);

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

	if(!oddflg){
		tape_led[tape_n].setPixel_hsv(num,H,S,V);
	}else{
		tape_led[tape_n].setPixel_hsv(115-num,H,S,V);
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
