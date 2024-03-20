#include "./fled.h"

FLED::FLED(Adafruit_NeoPixel* ptr_neopixel, int num){
	NEOPIXEL = ptr_neopixel;
	NUM = num;
}

void FLED::init(void){
	NEOPIXEL->begin();
	NEOPIXEL->setBrightness(BRIGHTNESS);
	NEOPIXEL->clear();
	NEOPIXEL->show();
}

void FLED::show(void){
	if(timer + interval < millis()){
		NEOPIXEL->show();
	}
}

void FLED::clear(void){
	NEOPIXEL->clear();
}

void FLED::set_color_rgb(int n, int r, int g, int b){
	NEOPIXEL->setPixelColor(n, r, g, b);
}

void FLED::set_color_rgb_all(int r, int g, int b){
	for(int i=0; i<NUM; i++){
		NEOPIXEL->setPixelColor(i, r, g, b);
	}
}
void FLED::set_color_hsv(int n, int h, int s, int v){
	int r, g, b;
	if (s == 0) {
		r = g = b = v;
	}else{
		h = h * 6; // sector 0 to 5
		int i = h / 256;
		int f = h % 256;
		int p = (v * (256 - s)) / 256;
		int q = (v * (256 - (s * f) / 256)) / 256;
		int t = (v * (256 - (s * (256 - f)) / 256)) / 256;

		switch (i) {
			case 0: r = v; g = t; b = p; break;
			case 1: r = q; g = v; b = p; break;
			case 2: r = p; g = v; b = t; break;
			case 3: r = p; g = q; b = v; break;
			case 4: r = t; g = p; b = v; break;
			default: r = v; g = p; b = q; break;
		}
	}

	NEOPIXEL->setPixelColor(n, r, g, b);
	
}


void FLED::set_color_hsv_all(int h, int s, int v){
	int r, g, b;
	if (s == 0) {
		r = g = b = v;
	}else{
		h = h * 6; // sector 0 to 5
		int i = h / 256;
		int f = h % 256;
		int p = (v * (256 - s)) / 256;
		int q = (v * (256 - (s * f) / 256)) / 256;
		int t = (v * (256 - (s * (256 - f)) / 256)) / 256;

		switch (i) {
			case 0: r = v; g = t; b = p; break;
			case 1: r = q; g = v; b = p; break;
			case 2: r = p; g = v; b = t; break;
			case 3: r = p; g = q; b = v; break;
			case 4: r = t; g = p; b = v; break;
			default: r = v; g = p; b = q; break;
		}
	}

	for(int i=0; i<NUM; i++){
		NEOPIXEL->setPixelColor(i, r, g, b);
	}
}
