#include <Arduino.h>
#include <HardwareSerial.h>

#define VERSION 1.00

#include "./fled/fled.h"
#include <Adafruit_NeoPixel.h>
#define LED_A_PIN 32  // The data pin that the WS2812 strips are connected to.32
#define LED_B_PIN 14
#define LED_C_PIN 13
#define LED_D_PIN 33
#define LED_E_PIN 27
#define LED_NUM 60

Adafruit_NeoPixel rawpixel[5] = {
	Adafruit_NeoPixel(60, LED_A_PIN, NEO_GRB + NEO_KHZ800),
	Adafruit_NeoPixel(120, LED_B_PIN, NEO_GRB + NEO_KHZ800),
	Adafruit_NeoPixel(60, LED_C_PIN, NEO_GRB + NEO_KHZ800),
	Adafruit_NeoPixel(120, LED_D_PIN, NEO_GRB + NEO_KHZ800),
	Adafruit_NeoPixel(40, LED_E_PIN, NEO_GRB + NEO_KHZ800),
};

FLED led[6] = {
	FLED(&rawpixel[0], 0, 59),
	FLED(&rawpixel[1], 119, 60),
	FLED(&rawpixel[1], 0, 59),
	FLED(&rawpixel[2], 0, 59),
	FLED(&rawpixel[3], 119, 60),
	FLED(&rawpixel[3], 0, 59)
};

FLED circuit_led[4] = {
	FLED(&rawpixel[4], 0, 9), //LED_FROM, LED_TO
	FLED(&rawpixel[4], 10, 19),
	FLED(&rawpixel[4], 20, 29),
	FLED(&rawpixel[4], 30, 39),
};


#include "./ser_ctrl/ser_ctrl.h"
HardwareSerial ctrl(2);
CTRL ser_ctrl(&ctrl);  


#include "./oled/oled.h"
#include <U8g2lib.h>
#define SDA_PIN 21
#define SCL_PIN 22
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0,/* reset=*/ U8X8_PIN_NONE);
OLED oled(&u8g2);

#include "./speaker/speaker.h"
#define SPEAKER_PIN 4
SPEAKER speaker(SPEAKER_PIN);

#include "./power/power.h"
#define VOL_PIN 26 //これ、無線機能のピンとかぶってるから注意
#define VOL_MAX 12.6
#define VOL_MIN 11.1
POWER power(VOL_PIN, VOL_MAX, VOL_MIN);

#include "./dip/dip.h"
#define DIP_PIN0 25
#define DIP_PIN1 18
#define DIP_PIN2 19
#define DIP_PIN3 23
DIP dip(DIP_PIN0, DIP_PIN1, DIP_PIN2, DIP_PIN3);

#include "./button/button.h"
#define BUTTON_PIN0 34
#define BUTTON_PIN1 5
#define BUTTON_PIN2 15
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

void led_test(float piyo);
void led_rainbow(float piyo);

#define BCD(c) 5 * (5 * (5 * (5 * (5 * (5 * (5 * (c & 128) + (c & 64)) + (c & 32)) + (c & 16)) + (c & 8)) + (c & 4)) + (c & 2)) + (c & 1)


void setup() {
	Serial.begin(115200);
	ser_ctrl.init();
	
	power.init();
	dip.init();
	button.init();
	
	oled.init();
	speaker.init();
	speaker.ring(C4);

	for(int i=0; i<5; i++)rawpixel[i].begin();
	for(int i=0; i<6; i++)led[i].init();
	for(int i=0; i<4; i++)circuit_led[i].init();

	for(int n=0; n<30; n++){
		for(int i=0; i<6; i++)led[i].clear();
		for(int i=0; i<4; i++)circuit_led[i].clear();
		for(int j=0; j<6; j++){
			led[j].set_color_hsv_all(150, 250, n);
			// led[j].set_width_hsv(24.5, n, 150, 250, 30); //set_width_hsv(float center, float width, int h, int s, int v)
		}
		for(int j=0; j<4; j++){
			circuit_led[j].set_color_hsv_all(150, 250, n);
			// circuit_led[j].set_width_hsv(4.5, n/2.5, 150, 250, 30);
		}
		for(int i=0; i<6; i++)led[i].show();
		for(int i=0; i<4; i++)circuit_led[i].show();
		if(n > 25){
			speaker.mute();
		}
	}

	oled.display_version(VERSION);
	oled.show();

	speaker.boot_music();

	delay(1000);

	pinMode(36, INPUT); //SENSOR_VP
	pinMode(39, INPUT); //SENSOR_VN
}


uint32_t hoge = 0;
float piyo = -30.0;

uint8_t pre_phase = 0;
uint32_t pf_time = 0;

uint32_t loop_time = 0;

uint32_t led_timer = 0;

uint32_t A_tim = 0;
uint32_t B_tim = 0;
uint32_t C_tim = 0;


void loop(){
	// Serial.print("loop_time: ");
	// Serial.print(micros()-loop_time);
	// Serial.print("  ");
	loop_time = micros();
	hoge++;
	piyo += 0.01;
	if(digitalRead(36) == 1){
		led_test(piyo);
	}else{
		ser_ctrl.read();
		// Serial.println();

		bool btn_val[3] = {0, 0, 0};
		button.read(btn_val);
		

		// //ーーーーーーーーーー効果音と光ーーーーーーーーーー
		
		//送信相手の判定 自分が下から何番目か数える
		bool re_id[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0}; 
		for(int i=0; i<8; i++){
			re_id[i] = ser_ctrl.data[0] & (1 << i);
			re_id[i+8] = ser_ctrl.data[1] & (1 << i);
		}

		uint8_t re_LedOrMusic = ser_ctrl.data[7] & 0b00001111; //0:LED 1:Music

		uint8_t my_number_index = 0;
		int8_t hogege = -1;
		for(int i=0; i<16; i++){
			if(re_id[i]){
				hogege++;
				if(i == dip.read_ID()){
					my_number_index = hogege;
				}
			}
		}

		if(re_LedOrMusic == 1 || re_LedOrMusic == 3){
			//モードの確認
			uint8_t re_mode = ser_ctrl.data[2] & 0b00001111; //0-F
			uint8_t re_scale = ser_ctrl.data[2] >> 4; //0-B

			//音の確認
			uint8_t re_sound = ser_ctrl.data[3] & 0b00001111; //0-8音 Fミュート
			int8_t re_octave = (ser_ctrl.data[3] >> 4) - 8; //-8から7

			//出力します！
			if(re_id[dip.read_ID()]){ //自分に送られてきたら
				if(re_mode = 0x1){ //メロディ 下ハモリ 2個まで
					if(re_sound == 0xF){
						speaker.mute();
						// Serial.println(" mute ");
					}else{
						uint16_t freq;
						if(my_number_index == 0){
							freq = speaker.major[re_scale][re_sound] * pow(2, re_octave);
						}else{ //if(my_number_index == 1){ これで越えた分は全部これになる
							if(re_sound-2 < 0){
								freq = speaker.major[re_scale][re_sound+5] * pow(2, re_octave-1);
							}else{
								freq = speaker.major[re_scale][re_sound-2] * pow(2, re_octave);
							}
						}
						speaker.ring(freq);
						// Serial.print(" freq: ");
						// Serial.println(freq);
					}
				}
			}
		}else{

		}

		uint16_t re_time = (ser_ctrl.data[7]-5) + (ser_ctrl.data[8]-5)*256;
		uint8_t re_start = ser_ctrl.data[9];
		uint8_t re_phase = ser_ctrl.data[10]-5;
		
		// Serial.printf("d7 %d d8 %d d9 %d d10 %d", ser_ctrl.data[7], ser_ctrl.data[8], ser_ctrl.data[9], ser_ctrl.data[10]);
		// Serial.printf(" time %d start %d phase %d \n", re_time, re_start, re_phase);


		if(pre_phase == 2 && re_phase == 3){
			pf_time = millis();	
		}
		pre_phase = re_phase;
		
		// Serial.printf("raw %d jissoku ", re_time-890);
		// if(pf_time != 0){
		// 	Serial.println(millis()-pf_time);
		// }else{
		// 	Serial.println("0");
		// }

		int32_t mytime = re_time; // 100msec単位
		Serial.println(re_time);

		if(led_timer+40 < millis()){
			led_timer = millis();
			int ledV = digitalRead(39) + 1;
			
			for(int i=0; i<4; i++) circuit_led[i].clear();
			for(int i=0; i<6; i++) led[i].clear();
			
			if(mytime <= 0){
				uint8_t H,S,V;
				H = 150; S = 250;	V = 50*ledV;
				for(int i=0;i<4;i++) circuit_led[i].set_color_hsv_all(H, S, V);
				for(int i=0;i<6;i++) led[i].set_color_hsv_all(H, S, V);
			}else if(mytime < 600){
				uint8_t H,S,V;
				H = 150+15*sin(piyo); S = 200; V = 10*ledV;
				for(int i=0;i<4;i++) circuit_led[i].set_color_hsv_all(H, S, V);
				for(int i=0;i<6;i++) led[i].set_color_hsv_all(H, S, V);
			}else if(mytime < 890){
				uint8_t H,S,V;
				H = 30+15*sin(piyo); S = 200; V = 10*ledV;
				for(int i=0;i<4;i++) circuit_led[i].set_color_hsv_all(H, S, V);
				for(int i=0;i<6;i++) led[i].set_color_hsv_all(H, S, V);
			}else if(mytime < 925){
				uint8_t H,S,V;
				H = 50; S = 250; V = 25*ledV;
				for(int i=0;i<4;i++) circuit_led[i].set_color_hsv_all(H, S, V);
				for(int i=0;i<6;i++) led[i].set_color_hsv_all(H, S, V);
			}else if(mytime <1300){
				for(int i=0;i<6;i++) led[i].set_color_hsv_all(0,0,2);
				if(A_tim == 0) A_tim = millis();
				uint32_t ttttime = (millis()-A_tim)/100;
				for(int i=0;i<6;i++) led[i].set_height_hsv(59*ttttime/390.0, 160, 250, 50*ledV);
			}else if(mytime < 1630){
				for(int i=0;i<6;i++) led[i].set_raimbow(piyo*5.0,130,10*ledV);
			}else if(mytime < 1730){
				if(B_tim == 0) B_tim = millis();
				uint32_t ttttime = (millis()-B_tim)/100;
				float gain = ttttime/110;
				for(int i=0;i<6;i++) led[i].set_raimbow(piyo*5.0,130+120*gain,(10+45*gain)*ledV);
			}else if(mytime <2100){
				for(int i=0;i<6;i++) led[i].set_color_hsv_all(0,0,2);
				if(C_tim == 0) C_tim = millis();
				uint32_t ttttime = (millis()-C_tim)/100;
				for(int i=0;i<6;i++) led[i].set_height_hsv(59*ttttime/320.0, 64*dip.read_ID() , 250, 50*ledV);
			}else if(mytime < 8000){
				A_tim = 0; B_tim = 0; C_tim = 0;
				for(int i=0;i<6;i++) led[i].set_color_hsv_all(0,0,10*ledV);
				int v;
				if(mytime < 2250){
					v = 120*ledV;
				}else{
					v = 120*ledV;
				}
				led[0].set_width_hsv( 30 + piyo*1.5, 7, 0, 240, v);
				led[1].set_width_hsv( 10 + piyo*2.0, 7, 42, 240, v);
				led[2].set_width_hsv( 20 + piyo*2.5, 7, 84, 240, v);
				led[3].set_width_hsv( 40 + piyo*1.8, 7, 126, 240, v);
				led[4].set_width_hsv( 30 + piyo*1.5, 7, 168, 240, v);
				led[5].set_width_hsv( 26 + piyo*0.9, 7, 210, 240, v);

				led[4].set_width_hsv( 60 + piyo*1.5, 7, 0, 240, v);
				led[5].set_width_hsv( 40 + piyo*2.0, 7, 42, 240, v);
				led[0].set_width_hsv( 52 + piyo*2.5, 7, 84, 240, v);
				led[1].set_width_hsv( 8 + piyo*1.8, 7, 126, 240, v);
				led[2].set_width_hsv( 58 + piyo*1.5, 7, 168, 240, v);
				led[3].set_width_hsv( 52 + piyo*0.7, 7, 210, 240, v);

				
				led[2].set_width_hsv( 60 + piyo*1.4, 7, 0, 240, v);
				led[3].set_width_hsv( 20 + piyo*2.1, 7, 42, 240, v);
				led[4].set_width_hsv( 32 + piyo*2.5, 7, 84, 240, v);
				led[5].set_width_hsv( 28 + piyo*1.7, 7, 126, 240, v);
				led[0].set_width_hsv( 58 + piyo*1.4, 7, 168, 240, v);
				led[1].set_width_hsv( 52 + piyo*0.8, 7, 210, 240, v);
			}			
			for(int i=0; i<4; i++) circuit_led[i].show();
			for(int i=0; i<6; i++) led[i].show();
		}



		//ーーーーーーーーーー表示ーーーーーーーーーー

		// printf(" data0: %#X data1: %#X\n", ser_ctrl.data[0], ser_ctrl.data[1]);

		oled.clear();
		oled.display_title("Pole"+ String(dip.read_ID()) +" V" + String(VERSION));
		oled.display_battary(power.voltage(), power.percentage());

		char StrID[5];
		sprintf(StrID, "%02X%02X ", ser_ctrl.data[1], ser_ctrl.data[0]);

		oled.half_display_num(
			"ID = "+String(StrID) + "  D2 = "+ String(ser_ctrl.data[2]),
			"D3 = "+String(ser_ctrl.data[3]) + "  D4 = "+String(ser_ctrl.data[4])
		);
		oled.half_display_3button(btn_val);
		oled.show();
	}
}





//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーメモ帳ーーーーーーーーーーーーーーーー

void led_rainbow(float piyo){
	for(int i=0; i<4; i++){
		circuit_led[i].clear();
	}
	for(int i=0; i<4; i++){ //虹色ループ
		for(int j=0; j<10; j++){
			circuit_led[i].set_color_hsv(j, 60*i+6*j, 250, 30);
		}
	}
	for(int i=0; i<4; i++){
		circuit_led[i].show();
	}
	// ser_ctrl.read();
	// byte kidou = ser_ctrl.data[0]; //10 起動 2 停止
	// byte phase = ser_ctrl.data[1] - 5; //1-4
}

void led_test(float piyo){
	for(int i=0; i<4; i++){
		circuit_led[i].clear();
	}
	for(int i=0; i<6; i++){
		led[i].clear();
	}

	circuit_led[0].set_width_hsv(piyo, 5.5, 0, 240, 100);
	circuit_led[1].set_width_hsv(piyo, 5.5, 63, 240, 100);
	circuit_led[2].set_width_hsv(piyo, 5.5, 126, 240, 100);
	circuit_led[3].set_width_hsv(piyo, 5.5, 189, 240, 100);

	for(int i=0; i<6; i++){
		led[i].set_color_hsv_all(0, 10, 3);
	}
	int i = 0;
	led[(0+i)%6].set_width_hsv( 30 + piyo, 7, 0, 240, 150);
	led[(1+i)%6].set_width_hsv( 10 + piyo, 7, 42, 240, 150);
	led[(2+i)%6].set_width_hsv( 20 + piyo, 7, 84, 240, 150);
	led[(3+i)%6].set_width_hsv( 40 + piyo, 7, 126, 240, 150);
	led[(4+i)%6].set_width_hsv( 30 + piyo, 7, 168, 240, 150);
	led[(5+i)%6].set_width_hsv( 26 + piyo, 7, 210, 240, 150);
	for(int i=0; i<4; i++){
		circuit_led[i].show();
	}
	for(int i=0; i<6; i++){
		led[i].show();
	}	
}


