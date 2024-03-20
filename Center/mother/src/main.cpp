#include <Arduino.h>

// #include "./fled/fled.h"
// #include <Adafruit_NeoPixel.h>
// #define LED_PIN0 PA6
// #define LED_NUM 300
// Adafruit_NeoPixel neopixel0 = Adafruit_NeoPixel(LED_NUM, LED_PIN0, NEO_GRB + NEO_KHZ800);
// FLED led0(&neopixel0, LED_NUM);

#include <HardwareSerial.h>
HardwareSerial PC(PA10, PA9); //UART1 RX, TX
//HardwareSerial ARM(PC7, PC6); //UART6 RX, TX
HardwareSerial POLE(PC7, PC6);

#include "./twelite/twelite.h"
HardwareSerial TWE(PD2, PC12); //UART2 RX, TX
TWELITE twelite(&TWE);


#define BCD(c) 5 * (5 * (5 * (5 * (5 * (5 * (5 * (c & 128) + (c & 64)) + (c & 32)) + (c & 16)) + (c & 8)) + (c & 4)) + (c & 2)) + (c & 1)

void setup() {
	PC.begin(115200);
	PC.println("start");

	POLE.begin(115200);

	twelite.init();

	pinMode(PC11,INPUT); // 下のスライド
}

int i=0;
void loop() {
	i++; if(i>255)i=5;

	byte control_send_data[4] = {i-4, i-3, i-2, i-1};
	// control_send_data[0] = byte(time_cnt % 240 +5);
	// control_send_data[1] = byte(time_cnt / 240 +5);
	// control_send_data[2] = byte(start_frg*150 + 50);
	// control_send_data[3] = byte(mode + 5);
	twelite.send(control_send_data);
	delay(30);

	PC.print("send ");
	PC.print(control_send_data[0]);
	PC.print(" ");
	PC.print(control_send_data[1]);
	PC.print(" ");
	PC.print(control_send_data[2]);
	PC.print(" ");
	PC.println(control_send_data[3]);
}
