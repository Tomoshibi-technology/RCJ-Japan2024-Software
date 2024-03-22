#include "./twelite.h"

TWELITE::TWELITE(HardwareSerial* ptr_serial){
	TWE = ptr_serial;
}

void TWELITE::init(void){
	TWE->begin(115200);
}

bool TWELITE::read(void){ //受信成功したら1を返す
	if(TWE->available()>10){
		byte data = TWE->read();
		if(data == 250){
			byte raw_receive_data[4] = {0, 0, 0, 0};
			bool receive_bad_flg = 0; //受信失敗フラグ
			for(int i=0; i<4; i++){
				if(TWE->available()>0){
					raw_receive_data[i] = TWE->read();
					if(raw_receive_data[i] == 250){receive_bad_flg = 1; break;} //受信失敗フラグを立てる(250が来たら
				}else{
					receive_bad_flg = 1; break;
				}
			}
			if(!receive_bad_flg){
				for(int i=0; i<4; i++){
					receive_data[i] = raw_receive_data[i];
				}
				return 1;
			}
		}
	}
	return 0;
}

void TWELITE::send(byte data[4]){
	TWE->write(250);
	for(int i=0; i<4; i++){
		TWE->write(data[i]);
	}
}

