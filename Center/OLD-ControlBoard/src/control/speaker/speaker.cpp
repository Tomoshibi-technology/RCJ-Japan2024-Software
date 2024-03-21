#include "./speaker.h"

SPEAKER::SPEAKER(uint8_t ptr_pin){
	PIN = ptr_pin;
}

void SPEAKER::init(){
	pinMode(PIN, OUTPUT);
}

void SPEAKER::ring(int freq){
	analogWriteFrequency(freq);
	analogWrite(PIN, 128);
}

void SPEAKER::mute(void){
	analogWrite(PIN, 0);
}

void SPEAKER::beat(int freq, float beat, int bpm){
	uint8_t rest = 2;
	if(freq == 0){
		mute();
	}else{
		ring(freq);
	}
	delay((60000/bpm*beat)-rest);

	mute();
	delay(rest);
}

void SPEAKER::boot_music(void){
	int bpm = 230;
	beat(C5, 0.5, bpm);
	beat(G5, 0.5, bpm);
	beat(D5, 0.5, bpm);
	beat(A5, 0.5, bpm);
}

void SPEAKER::boot2_music(void){
	ring(F4);
	delay(100);
	ring(F5);
	delay(100);
	mute();
}



