#ifndef _H_CTRL_
#define _H_CTRL_

#include <Arduino.h>
#include <HardwareSerial.h>

class CTRL{
	private:
		HardwareSerial* SER;

	public:
		CTRL(HardwareSerial* ptr_serial);

		void init(void);
		void read(void);

		byte data[12] = {0,0,0,0,0,0,0,0,0,0,0,0};

};

#endif