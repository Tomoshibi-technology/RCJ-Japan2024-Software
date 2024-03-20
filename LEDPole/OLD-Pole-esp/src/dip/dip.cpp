#include "./dip.h"

DIP::DIP(uint8_t ptr_pin0, uint8_t ptr_pin1, uint8_t ptr_pin2, uint8_t ptr_pin3){
	PIN[0] = ptr_pin0;
	PIN[1] = ptr_pin1;
	PIN[2] = ptr_pin2;
	PIN[3] = ptr_pin3;
}

void DIP::init(){
	for(int i = 0; i < 4; i++){
		pinMode(PIN[i], INPUT);
	}
}

int DIP::read_ID(){
	int id = 0;
	for(int i = 0; i < 4; i++){
		id += digitalRead(PIN[i]) * pow(2, i);
	}
	return id;
}