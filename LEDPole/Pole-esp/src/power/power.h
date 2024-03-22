#ifndef _H_POWER_
#define _H_POWER_

#include <Arduino.h>

class POWER{
	private:
		int PIN;

		//回路の定数
		float VOL_MAX = 3.3;
		int ADC_RES = 4096;
		int RES1 = 33;
		int RES2 = 10;
		//アナログ値に対する電圧の係数
		float _VolK; // = VOL_MAX/ADC_RES *(RES1+RES2)/RES2;
		//電圧の最大値と最小値の差分
		float _diff_voltage;
		//%を計算するときの引く値
		float _parcentage_subtract;
		//%は、X /_diff_voltage*100 - _parcentage_subtract


		float MAX_VOLTAGE;
		float MIN_VOLTAGE;

		void read(void);
		long timer = 0; 
		int interval = 1000; //ms

		float voltage_value = 0;
		int percentage_value = 0;

	public:
		POWER(int ptr_pin, float max_voltage, float min_voltage);
		void init();
		
		float voltage();
		int percentage();
};

#endif