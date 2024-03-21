#ifndef _H_OLED_
#define _H_OLED_

#include <Arduino.h>
#include <U8g2lib.h>

class OLED{
		private:
			U8G2_SSD1306_128X64_NONAME_F_SW_I2C* myU8G2;
			
			uint32_t timer = 0;  
			int interval = 1000; //ms こいつだけ特別に速くしてます

		public:
			OLED(U8G2_SSD1306_128X64_NONAME_F_SW_I2C* ptr_myu8g2);
			void init();
			void display_version(float version);
			void clear();
			void show();

			void half_display_num(String str0, String str1);
			void display_battary(float voltage, int percentage);
			void display_title(String str);
			void half_display_3button(bool* n);
			
};

#endif