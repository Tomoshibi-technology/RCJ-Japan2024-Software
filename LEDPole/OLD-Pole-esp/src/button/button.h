#ifndef _H_BUTTON_
#define _H_BUTTON_

#include <Arduino.h>

class BUTTON{
		private:
			uint8_t PIN[3] = {0, 0, 0};

		public:
			BUTTON(uint8_t ptr_pin0, uint8_t ptr_pin1, uint8_t ptr_pin2);
			void init();
			void read(bool* n);
};

#endif