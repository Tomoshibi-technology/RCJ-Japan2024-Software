#include "./speaker.h"

SPEAKER::SPEAKER(uint8_t ptr_pin){
	PIN = ptr_pin;
}

void SPEAKER::init(){
	// pinMode(PIN, OUTPUT);

  ledcSetup(CHANNEL,12000, 8);
  ledcAttachPin(PIN,CHANNEL);
}

void SPEAKER::ring(int freq){
	if(pre_freq != freq){
		// analogWriteFrequency(freq);
		// analogWrite(PIN, 128);
	  ledcWriteTone(CHANNEL,freq);
		pre_freq = freq;
		// Serial.println(freq);
	}
}

void SPEAKER::mute(void){
	// analogWrite(PIN, 0);
	  ledcWriteTone(CHANNEL,0);
	pre_freq = 0;
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



