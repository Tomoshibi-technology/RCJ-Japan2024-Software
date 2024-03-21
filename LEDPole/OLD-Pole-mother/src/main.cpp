#include <Arduino.h>

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
HardwareSerial POLE(PD2, PC12);

#define BCD(c) 5 * (5 * (5 * (5 * (5 * (5 * (5 * (c & 128) + (c & 64)) + (c & 32)) + (c & 16)) + (c & 8)) + (c & 4)) + (c & 2)) + (c & 1)

struct Note {
	int pitch;    // 音の高さ
	float duration; // ビートの長さ
};

void setup() {
	PC.begin(115200);
	PC.println("start");

	POLE.begin(115200);
	
	// led0.init();
	// led0.set_color_rgb_all(50, 0, 0);
	// led0.show();

	//ーーーこれは必須ーーーーー
	control_init();
	//ーーーーーまじでーーーーー

	pinMode(PC11,INPUT); // 下のスライド
}

unsigned long loop_timer = 10000;
unsigned long sec_timer = 0;

uint32_t mycount = 0;

uint8_t kaeru0[9] = {0x0, 0x1, 0x2, 0x3, 0x2, 0x1, 0x0, 0xF, 0xF};
uint8_t kaeru1[56] = {0x0, 0x1, 0x2, 0x3, 0x2, 0x1, 0x0, 0xF,
											0x2, 0x3, 0x4, 0x5, 0x4, 0x3, 0x2, 0xF,
											0x0, 0xF, 0xF, 0x0, 0xF, 0xF, 0x0, 0xF, 0xF, 0x0, 0xF, 0xF, 
											0x0, 0xF, 0x0, 0xF, 0x1, 0xF, 0x1, 0xF,
											0x2, 0xF, 0x2, 0xF, 0x3, 0xF, 0x3, 0xF,
											0x2, 0xF, 0xF, 0x1, 0xF, 0xF, 0x0, 0xF, 0xF, 0xF, 0xF, 0xF
										};
uint8_t doremi[9] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0xF};

float hogeee = 0.45;
Note kaeru2[42] = {
	{0x0,1},{0x1,1},{0x2,1},{0x3,1},{0x2,1},{0x1,1},{0x0,2},
	{0x2,1},{0x3,1},{0x4,1},{0x5,1},{0x4,1},{0x3,1},{0x2,2},
	{0x0,1},{0xF,1},{0x0,1},{0xF,1},{0x0,1},{0xF,1},{0x0,1},{0xF,1},
	{0x0,hogeee},{0xF,0.5-hogeee},{0x0,hogeee},{0xF,0.5-hogeee},
	{0x1,hogeee},{0xF,0.5-hogeee},{0x1,hogeee},{0xF,0.5-hogeee},
	{0x2,hogeee},{0xF,0.5-hogeee},{0x2,hogeee},{0xF,0.5-hogeee},
	{0x3,hogeee},{0xF,0.5-hogeee},{0x3,hogeee},{0xF,0.5-hogeee},
	{0x2,1},{0x1,1},{0x0,2},{0xF,2}
};


void loop() {
	//ーーーこれは必須ーーーーー
	control_loop();
	//ーーーーーまじでーーーーー


	//ーーーーーーーーーーループ計測ーーーーーーーーーー
	// PC.print(micros() - loop_timer);
	loop_timer = micros();
	if(sec_timer+250 < millis()){
		sec_timer = millis();
		mycount++;
		// PC.printf("count: %08d\n", mycount);
	}

	uint8_t send_data[12] = {0,0, 100,0xF,40,50,60,70,80,90,100,110};

	if(digitalRead(PC11)){
		send_data[7] = 0x1; //音だけ
		//送信相手選択 bit目がid番号
		send_data[0] = 0b00000111; //0-7 
		send_data[1] = 0b00000000; //8-15

		//モード選択
		send_data[2] = 0x1; //メロディ 下ハモリ
		//スケールの指定
		send_data[2] |= 0x0 << 4; //Cmajor

		uint16_t mytime = millis()%30000; 
		uint16_t beat_time = 60000/180; // bpm=120
		
		uint16_t music_time = 0;
		bool music_time_flg[42] = {0};

		for(int i=0; i<42; i++){
			music_time += beat_time*kaeru2[i].duration;
			if(music_time-beat_time*kaeru2[i].duration<mytime && mytime<music_time-10){
				send_data[3] = kaeru2[i].pitch;
				send_data[3] |= 0xB<< 4; //オクターブの指定
			}else if(music_time-10<mytime && mytime<music_time){
				send_data[3] = 0xF;
			}
		}
	}else{
		// 光
		send_data[0] = 0b00000111; //7-0 
		send_data[1] = 0b00000000; //15-8

		// send_data[7] = 0x2; //光だけ
		// send_data[7] |= 0x2 << 4; //mode指定

		// send_data[8] = (0)%0xF; // 主H
		// send_data[8] |= (5)%0xF << 4; //副H

		// send_data[9] = 3; // 主S
		// send_data[9] |= 3 << 2; //副S
		// send_data[9] |= 1 << 4; //主V
		// send_data[9] |= 1 << 6; //副V

		// send_data[10] = (mycount)%255; // 主H
	}

	send_data[7] = twelite.receive_data[0];
	send_data[8] = twelite.receive_data[1];
	send_data[9] = twelite.receive_data[2];
	send_data[10] = twelite.receive_data[3];
	
	
	// if(twelite.receive_data[2]==200 ){ //スタートスイッチの有無
	// 	send_data[0] = 10;
	// }else{
	// 	send_data[0] = 2;
	// }

	// send_data[1] = twelite.receive_data[3]; //フェーズ 6-9

	//有線送信
	POLE.write(250);
	for(int i=0; i<12; i++){
		if(send_data[i] == 250) send_data[i] = 251;
		POLE.write(send_data[i]);
		// PC.print(send_data[i]);
	}
	

	// ーー無線の内容が見れます(別に消して大丈夫)ーー
	// if(twelite.read()){ //tweliteから受信成功したら1を返す
	// 	PC.print(micros() - loop_timer);
	// 	for(int i=0; i<4; i++){
	// 		PC.print("  :");
	// 		PC.print(twelite.receive_data[i]);
	// 	}
	// 	PC.println();
	// }
}
