#include "./power.h"

POWER::POWER(int ptr_pin, float max_voltage, float min_voltage){
	PIN = ptr_pin;
	MAX_VOLTAGE = max_voltage;
	MIN_VOLTAGE = min_voltage;

	_VolK = VOL_MAX/ADC_RES *(RES1+RES2)/RES2;
	_diff_voltage = MAX_VOLTAGE - MIN_VOLTAGE;
	_parcentage_subtract = MIN_VOLTAGE/_diff_voltage*100;
}

void POWER::init(){
	pinMode(PIN, INPUT);
}

void POWER::read(){
	if(timer + interval < millis()){
		timer = millis();

		voltage_value = analogRead(PIN)*_VolK;
		if(voltage_value > MAX_VOLTAGE){
			percentage_value = 100;
		}else if(voltage_value < MIN_VOLTAGE){
			percentage_value = 0;
		}else{
			// percentage_value = (voltage_value - MIN_VOLTAGE)/(MAX_VOLTAGE - MIN_VOLTAGE)*100;
			percentage_value = voltage_value/_diff_voltage*100 - _parcentage_subtract;
		}
	}
}

float POWER::voltage(){
	read();
	return voltage_value;
}

int POWER::percentage(){
	read();
	return percentage_value;
}

