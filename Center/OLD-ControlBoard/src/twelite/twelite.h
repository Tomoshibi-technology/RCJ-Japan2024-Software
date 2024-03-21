#ifndef _H_TWELITE_
#define _H_TWELITE_

#include <Arduino.h>
#include <HardwareSerial.h>

class TWELITE{
	private:
		HardwareSerial* TWE;
	
	public:
		TWELITE(HardwareSerial* ptr_serial);
		void init(void);
		bool read(void); //受信成功したら1を返す
		void send(byte data[4]);

		byte receive_data[4] = {0, 0, 0, 0};
};

#endif