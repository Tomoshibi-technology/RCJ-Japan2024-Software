#include "./fled.h"

//---------public---------

FLED::FLED(Adafruit_NeoPixel* ptr_neopixel, int led_from, int led_to){
	NEOPIXEL = ptr_neopixel;
	LED_FROM = led_from;
	LED_TO = led_to;
	if(LED_FROM > LED_TO){
		TOTAL = LED_FROM - LED_TO + 1;
	}else{
		TOTAL = LED_TO - LED_FROM + 1;
	}

	pixel_vector = new Vector[TOTAL];

	num2rad_gain = 2.0*PI/TOTAL;

	for(int i=0; i<TOTAL; i++){
		pixel_vector[i].X = cos(i*num2rad_gain);
		pixel_vector[i].Y = sin(i*num2rad_gain);
	}
}

void FLED::init(void){
	NEOPIXEL->begin();
	NEOPIXEL->setBrightness(BRIGHTNESS);
	NEOPIXEL->clear();
	NEOPIXEL->show();
}

void FLED::show(void){
	// if(timer + interval < millis()){
		NEOPIXEL->show();
		// timer = millis();
	// }
}

void FLED::clear(void){
	NEOPIXEL->clear();
}

void FLED::set_color_rgb(int num, int r, int g, int b){
	NEOPIXEL->setPixelColor(get_num(num), r, g, b);
}

void FLED::set_color_rgb_all(int r, int g, int b){
	for(int i=0; i<TOTAL; i++){
		NEOPIXEL->setPixelColor(get_num(i), r, g, b);
	}
}

void FLED::set_color_hsv(int num, int h, int s, int v){
	int r, g, b; get_hsv2rgb(h, s, v, &r, &g, &b);
	NEOPIXEL->setPixelColor(get_num(num), r, g, b);
	
}


void FLED::set_color_hsv_all(int h, int s, int v){
	int r, g, b; get_hsv2rgb(h, s, v, &r, &g, &b);
	for(int i=0; i<TOTAL; i++){
		NEOPIXEL->setPixelColor(get_num(i), r, g, b);
	}
}


void FLED::set_width_rgb(float center, float width, int r, int g, int b){
	float diff = width / 2;
	while(center <= diff){
		center += TOTAL;
	}
	float from = center - diff;
	float to = center + diff;

	//目標のベクトル
	Vector center_vector(cos(center*num2rad_gain), sin(center*num2rad_gain));
	//端のベクトル
	Vector edge_vector(cos(from*num2rad_gain), sin(from*num2rad_gain));

	float dot_edge = (edge_vector.X * center_vector.X + edge_vector.Y * center_vector.Y +1) / 2;//内積 0-1
	
	
	if(dot_edge == 1.0){ //fromがcenterと同じとき
		this->set_color_rgb(from+1, r, g, b);
	}else{
		float r_gain, g_gain, b_gain;
		r_gain = r/(1.0-dot_edge);
		g_gain = g/(1.0-dot_edge);
		b_gain = b/(1.0-dot_edge);
		for(int i=from+1; from<=i&&i<=to; i++){	//fromに+1すれば、切り上げられるはず
																					//5.0とかのとき、6からになっちゃうけど、どうせ0なので問題なし
			//内積 0.0-1.0
			float dot_now = (pixel_vector[i%TOTAL].X * center_vector.X + pixel_vector[i%TOTAL].Y * center_vector.Y + 1) / 2;
			float dot_diff = abs(dot_now - dot_edge); //内積の差分 0.0-1.0

			int R = r_gain * dot_diff;
			int G = g_gain * dot_diff;
			int B = b_gain * dot_diff;
			this->set_color_rgb(i, R, G, B);
		}
	}
}

void FLED::set_width_hsv(float center, float width, int h, int s, int v){
	float diff = width / 2;
	while(center <= diff){
		center += TOTAL;
	}
	float from = center - diff;
	float to = center + diff;

	Vector center_vector(cos(center*num2rad_gain), sin(center*num2rad_gain));
	Vector edge_vector(cos(from*num2rad_gain), sin(from*num2rad_gain));
	float dot_edge = (edge_vector.X * center_vector.X + edge_vector.Y * center_vector.Y +1) / 2;//内積 0-1
	
	
	if(dot_edge == 1.0){
		this->set_color_hsv(from+1, h, s, v);
	}else{
		float v_gain = v/(1.0-dot_edge);
		for(int i=from+1; from<=i&&i<=to; i++){
			float dot_now = (pixel_vector[i%TOTAL].X * center_vector.X + pixel_vector[i%TOTAL].Y * center_vector.Y + 1) / 2;
			float dot_diff = abs(dot_now - dot_edge);
			int V = v_gain * dot_diff;
			this->set_color_hsv(i, h, s, V);
		}
	}
}


void FLED::set_height_rgb(float height, int r, int g, int b){
	//今度滑らかにする
	for(int i=0; i<height; i++){
		NEOPIXEL->setPixelColor(get_num(i), r, g, b);
	}
}

void FLED::set_height_hsv(float height, int h, int s, int v){
	for(int i=0; i<height; i++){
		uint16_t hogeH = (h+224 + int(32.0*i/height))%255;
		if(s < 100)s=100;
		uint16_t hogeS = 100 + (s-100)*i/height;

		Serial.print(hogeH);
		Serial.print(" ");
		this->set_color_hsv(i, hogeH, hogeS, v);
	}
	Serial.println();
}

void FLED::set_raimbow(float piyo,int s, int v){
	while(piyo>100) piyo -= 100;
	while(piyo<0) piyo += 100;
	for(int i=0; i<TOTAL; i++){
		this->set_color_hsv(i,int((255*i/TOTAL)+piyo)%255,s,v);
	}
}


//---------private---------
void FLED::get_hsv2rgb(int h, int s, int v, int* r, int* g, int* b){
	if (s == 0) {
		*r = *g = *b = v;
	}else{
		h = h * 6; // sector 0 to 5
		int i = h / 256;
		int f = h % 256;
		int p = (v * (256 - s)) / 256;
		int q = (v * (256 - (s * f) / 256)) / 256;
		int t = (v * (256 - (s * (256 - f)) / 256)) / 256;

		switch (i) {
			case 0: *r = v; *g = t; *b = p; break;
			case 1: *r = q; *g = v; *b = p; break;
			case 2: *r = p; *g = v; *b = t; break;
			case 3: *r = p; *g = q; *b = v; break;
			case 4: *r = t; *g = p; *b = v; break;
			default: *r = v; *g = p; *b = q; break;
		}
	}
}

int FLED::get_num(int n){
	int result;

	while(n < 0){
		n += TOTAL;
	}
	if(n >= TOTAL){
		result = n % (TOTAL);
	}else{
		result = n;
	}

	if(LED_FROM > LED_TO){
		result = LED_FROM - result;
	}else{
		result = LED_FROM + result;
	}
	return result;
}



