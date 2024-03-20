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


#include "./fled/fled.h"
#include "./fled/RMT_WS2812.hpp"
#define LED_A_PIN GPIO_NUM_18  // The data pin that the WS2812 strips are connected to.32
#define LED_B_PIN GPIO_NUM_21
#define LED_C_PIN GPIO_NUM_19
#define LED_D_PIN GPIO_NUM_15
#define LED_NUM 116

RMT_WS2812 rawpixel[4] = {
	RMT_WS2812(RMT_CHANNEL_0, LED_A_PIN, 116),
	RMT_WS2812(RMT_CHANNEL_1, LED_B_PIN, 116),
	RMT_WS2812(RMT_CHANNEL_2, LED_C_PIN, 116),
	RMT_WS2812(RMT_CHANNEL_3, LED_D_PIN, 10),
};

FLED tape_led[6] = {
	FLED(&rawpixel[0], 0, 58),
	FLED(&rawpixel[0], 59, 115),
	FLED(&rawpixel[1], 0, 58),
	FLED(&rawpixel[1], 59, 115),
	FLED(&rawpixel[2], 0, 58),
	FLED(&rawpixel[2], 59, 115),
};

FLED circuit_led(&rawpixel[3], 0, 9);


void setup() {
	Serial.begin(115200);
	Serial.println("Hello World");
	Serial.println("Version: " + String(VERSION));

	rawpixel[3].begin();
	rawpixel[3].clear();

	rawpixel[3].setPixel(3, 255, 0, 0);
	rawpixel[3].setBrightness(50);

	rawpixel[3].refresh();

	// for(int i=0; i<4; i++){
	// 	rawpixel[i].begin();
	// 	rawpixel[i].clear();
	// }
	// for(int i=0; i<6; i++){
	// 	tape_led[i].init();
	// }
	// circuit_led.init();
	// circuit_led.set_color_hsv_all(30, 100, 100);
	// circuit_led.show();

	ser_ctrl.init();
	power.init();
}

void loop() {
	// ser_ctrl.read();
	Serial.print(String(analogRead(VOL_PIN)) + " ");
	Serial.println("voltage: " + String(power.voltage()) + " " + String(power.percentage()));
	delay(100);
}