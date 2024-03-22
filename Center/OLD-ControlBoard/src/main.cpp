#include <Arduino.h>


byte control_send_data[4] = {0, 0, 0, 0};
bool btn_val[3] = {0, 0, 0};

#include "./control/control.h"

#include "./fled/fled.h"
#include <Adafruit_NeoPixel.h>
#define LED_PIN0 PA6
#define LED_NUM 300
Adafruit_NeoPixel neopixel0 = Adafruit_NeoPixel(LED_NUM, LED_PIN0, NEO_GRB + NEO_KHZ800);
FLED led0(&neopixel0, LED_NUM);

#include <HardwareSerial.h>
HardwareSerial PC(PA10, PA9); //UART1 RX, TX
//HardwareSerial ARM(PC7, PC6); //UART6 RX, TX

HardwareSerial TWE(PA1, PA0); //UART2 RX, TX


#include "./twelite/twelite.h"
TWELITE twelite(&TWE);


unsigned long loop_timer = 10000;

bool start_frg = 0;
uint32_t pc_timer = 0;
uint32_t button_time = 0;

void setup() {
	PC.begin(115200);
	PC.println("start");
	
	led0.init();
	led0.set_color_rgb_all(50, 0, 0);
	led0.show();

	//ーーーこれは必須ーーーーー
	control_init();
	//ーーーーーまじでーーーーー
	twelite.init();

	pinMode(PB13, OUTPUT);
	pinMode(PC11,INPUT);
	pinMode(PB14, OUTPUT);
	PC.println("E");
}


uint32_t test_timer = 0;
uint32_t send_timer = 0;
void loop() {
	//ーーーこれは必須ーーーーー
	// control_loop();
	//ーーーーーまじでーーーーー

		//ーーーーーーーーーーボタンーーーーーーーーーーー
	button.read(btn_val);
	// byte send_data = btn_val[0]*10 + btn_val[1] * 20 + btn_val[2] * 40;


	while(PC.available() > 10){
		int a = PC.read();
		digitalWrite(PB15, HIGH);
	}
	if(PC.available()){
		byte read = PC.read();
		if(read == '1'){
			digitalWrite(PB13, HIGH);
			pc_timer++;
		}else if(read == '0'){
			digitalWrite(PB13, LOW);
		}
	}

	bool music_frg = 0;
	music_frg = start_frg;
	send_timer++;
	if(send_timer == 200){
		send_timer = 0;
		if(music_frg){
			PC.println("D");
		}else{
			PC.println("E");
		}
	}

	//ーーーーーーーーーーループ計測ーーーーーーーーーー
	// PC.print(micros() - loop_timer);
	// loop_timer = micros();


	//ーーーーーーーーーーボタンーー
	uint16_t btn_cnt = 0;
	while(btn_val[0] && start_frg == 0){
		if(btn_cnt > 10000){
			start_frg = 1;
			button_time = pc_timer;
			break;
		}else{
			btn_cnt++;
		}
	}

	//送るデータ決める
	uint16_t time_cnt = 0; 
	if(start_frg == 1){
		time_cnt = pc_timer - button_time;
	}else{
		time_cnt = 0;
	}

	byte mode = 0;

	if(digitalRead(PC11) == 1){
		digitalWrite(PB14, HIGH);
		//本番用

		if(time_cnt < 600){ //ロボットアーム
			mode = 1;
		}else if(time_cnt < 890){ //ディスプレイ
			mode = 2;
		}else if(time_cnt < 1180){ //ポール
			mode = 3;
		}else if(time_cnt <	1630){ //ディスプレイとポール
			mode = 4;
		}else if(time_cnt <	2200){ //全部
			mode = 5;
		}else{ //おわり
			mode = 6;
		}

	}else{
		digitalWrite(PB14, LOW);
		//test_mode
		// if(test_timer == 5){
		// 	PC.println("E");
		// }
		
		test_timer++;
		if(test_timer < 500){
			mode = 1;
		}else if(test_timer < 1000){
			mode = 2;
		}else if(test_timer < 1500){
			mode = 3;
		}else if(test_timer < 2000){
			mode = 4;
		}else if(test_timer < 2500){
			mode = 5;
		}else if(test_timer < 3000){	
			mode = 6;
		}else{
			test_timer = 0;
		}
	}


	control_send_data[0] = byte(time_cnt % 240 +5);
	control_send_data[1] = byte(time_cnt / 240 +5);
	control_send_data[2] = byte(start_frg*150 + 50);
	control_send_data[3] = byte(mode + 5);
	twelite.send(control_send_data);	

	
	// //ーーーーーーーーーー表示ーーーーーーーーーー
	oled.clear();
	oled.display_title(name[dip.read_ID()]+" V" + String(VERSION));
	oled.display_battary(power.voltage(), power.percentage());
	oled.half_display_num(
		// "D0 = "+String(twelite.receive_data[0]) + "  D1 = "+String(twelite.receive_data[1]),
		// "D2 = "+String(twelite.receive_data[2]) + "  D3 = "+String(twelite.receive_data[3])
		"D0 = "+String(control_send_data[0]) + "  D1 = "+String(control_send_data[1]),
		"D2 = "+String(control_send_data[2]) + "  D3 = "+String(control_send_data[3]) + " " + String(time_cnt/10)
	);
	oled.half_display_3button(btn_val);
	oled.show();

	
	// PC.print(micros() - loop_timer);
	// 	for(int i=0; i<4; i++){
	// 		PC.print("  :");
	// 		PC.print(control_send_data[i]);
	// 	}
	// PC.println();
	
	
	//ーーーーーーーーーーLED(自由に光らせてね)ーーーーーーーーーーー
	// led0.clear();
	// led0.set_color_hsv_all((millis()%2550)/10 , 150, 10);
	// led0.show();
}
