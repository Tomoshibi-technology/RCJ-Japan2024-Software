#include "./button.h"

BUTTON::BUTTON(uint8_t ptr_pin0, uint8_t ptr_pin1, uint8_t ptr_pin2){
	PIN[0] = ptr_pin0;
	PIN[1] = ptr_pin1;
	PIN[2] = ptr_pin2;
}

void BUTTON::init(){
	for(int i = 0; i < 3; i++){
		pinMode(PIN[i], INPUT);
	}
}

void BUTTON::read(bool* n){
	for(int i = 0; i < 3; i++){
		n[i] = digitalRead(PIN[i]);
		// n[i] = !n[i]; //プルアップなので反転
	}
}