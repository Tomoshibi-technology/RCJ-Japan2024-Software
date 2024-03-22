#include <Arduino.h>

#include <Adafruit_NeoPixel.h>
#define LED_PIN0 PB5
#define LED_NUM 20
Adafruit_NeoPixel neopixel0 = Adafruit_NeoPixel(LED_NUM, LED_PIN0, NEO_GRB + NEO_KHZ800);

#include <HardwareSerial.h>
HardwareSerial PC(PA10, PA9); //UART1 RX, TX
//HardwareSerial ARM(PC7, PC6); //UART6 RX, TX
// HardwareSerial POLE(PC7, PC6);

#include "./twelite/twelite.h"
HardwareSerial TWE(PD2, PC12); //UART2 RX, TX
TWELITE twelite(&TWE);

#define LED_PIN0 PB2
#define LED_PIN1 PA5

#define BCD(c) 5 * (5 * (5 * (5 * (5 * (5 * (5 * (c & 128) + (c & 64)) + (c & 32)) + (c & 16)) + (c & 8)) + (c & 4)) + (c & 2)) + (c & 1)

void setup() {
	PC.begin(115200);
	PC.println("start");

	twelite.init();

	pinMode(LED_PIN0, OUTPUT);
	pinMode(LED_PIN1, OUTPUT);

	neopixel0.begin();
	neopixel0.setBrightness(20);
	for(int i=0; i<LED_NUM; i++){
		neopixel0.setPixelColor(i, neopixel0.Color(2, 10, 10));
	}
	neopixel0.show();
}

int i=0;
byte data[4] = {0,0,0,0};


void loop() {
	i++; if(i>255)i=5;

	digitalWrite(LED_PIN0, HIGH);
	while(PC.available()>20){
		byte mydata = PC.read();
	}
	while(PC.available()){
		byte mydata = PC.read();
		// PC.write(mydata); // テスト用
		if(mydata == 250){			
			byte raw_data[4] = {0,0,0,0};
			bool receive_bag_flag = 0;

			for(int i=0; i<4; i++){
				if(PC.available()){
					raw_data[i] = PC.read();
					if(raw_data[i] == 250){
						receive_bag_flag = 1;
						break;
					}
				}else{
					receive_bag_flag = 1;
					break;
				}
			}
			if(!receive_bag_flag){
				for(int i=0; i<4; i++){
					data[i] = raw_data[i];
				}
			}
		}
	}
	digitalWrite(LED_PIN0, LOW);


	byte control_send_data[4] = {data[0], data[1], data[2], data[3]};
	// control_send_data[0] = byte(time_cnt % 240 +5);
	// control_send_data[1] = byte(time_cnt / 240 +5);
	// control_send_data[2] = byte(start_frg*150 + 50);
	// control_send_data[3] = byte(mode + 5);
	twelite.send(control_send_data);

	int mini_time = data[2]%10;
	if(mini_time < 5){
		digitalWrite(LED_PIN1, HIGH);
	}else{
		digitalWrite(LED_PIN1, LOW);
	}

	delay(50);

	// PC.print("send ");
	// PC.print(control_send_data[0]);
	// PC.print(" ");
	// PC.print(control_send_data[1]);
	// PC.print(" ");
	// PC.print(control_send_data[2]);
	// PC.print(" ");
	// PC.println(control_send_data[3]);
}
