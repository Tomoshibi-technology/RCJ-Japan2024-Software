#ifndef _H_DIP_
#define _H_DIP_

#include <Arduino.h>

class DIP{
		private:
			uint8_t PIN[4] = {0, 0, 0, 0};

		public:
			DIP(uint8_t ptr_pin0, uint8_t ptr_pin1, uint8_t ptr_pin2, uint8_t ptr_pin3);
			void init();
			int read_ID();
};

#endif