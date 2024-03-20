#ifndef _H_FLED_
#define _H_FLED_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
//#include "../cossim/cossim.h" 

struct Vector {//あとでライブラリ化して、ifndefで囲って使いやすくする
  float X;
  float Y;

	Vector() : X(0), Y(0) {} //指定しなければ0で初期化
	Vector(float x, float y) : X(x), Y(y) {} //初期化子リストってやつを使ってみたの巻

	~Vector(){
	}
};

class FLED{
	private:
		Adafruit_NeoPixel* NEOPIXEL;
		int BRIGHTNESS = 255;

		int TOTAL;
		int LED_FROM;
		int LED_TO;

		long timer = 0; 
		int interval = 20; //ms

		int get_num(int n);
		void get_hsv2rgb(int h, int s, int v, int* r, int* g, int* b);

		Vector* pixel_vector;
		float num2rad_gain;

	public:
		FLED(Adafruit_NeoPixel* ptr_neopixel, int led_from, int led_to);

		void init(void);
		void show(void);
		void clear(void);

		void set_color_rgb(int num, int r, int g, int b);
		void set_color_hsv(int num, int h, int s, int v);
		void set_color_rgb_all(int r, int g, int b);
		void set_color_hsv_all(int h, int s, int v);

		void set_width_rgb(float center, float width, int r, int g, int b);
		void set_width_hsv(float center, float width, int h, int s, int v);

		void set_height_rgb(float height, int r, int g, int b);
		void set_height_hsv(float height, int h, int s, int v);

		void set_raimbow(float piyo,int s, int v);
		

		~FLED(){ //デストラクタ
			delete[] pixel_vector; //一応メモリ解放しておく
		}
};

#endif