#ifndef _H_SPEAKER_
#define _H_SPEAKER_

#include <Arduino.h>

class SPEAKER{
		private:
			uint8_t PIN;

		public:
			SPEAKER(uint8_t ptr_pin);
			void init();
			void ring(int freq);
			void mute(void);

			void beat(int freq, float beat, int bpm);

			void boot_music(void);
			void boot2_music(void);

};

#define C4 262
#define C4s 277
#define D4 294
#define D4s 311
#define E4 330
#define F4 349
#define F4s 370
#define G4 392
#define G4s 415
#define A4 440
#define A4s 466
#define B4 494

#define C5 523
#define C5s 554
#define D5 587
#define D5s 622
#define E5 659
#define F5 698
#define F5s 740
#define G5 784
#define G5s 831
#define A5 880
#define A5s 932
#define B5 988

#define C6 1047
#define C6s 1109
#define D6 1175
#define D6s 1245
#define E6 1319
#define F6 1397
#define F6s 1480
#define G6 1568
#define G6s 1661
#define A6 1760
#define A6s 1865
#define B6 1976


#endif