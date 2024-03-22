
#include "./ser_ctrl.h"

CTRL::CTRL(HardwareSerial* ptr_serial){
	SER = ptr_serial;
}

void CTRL::init(void){
	SER->begin(115200);
}

void CTRL::read(void){
	int n = SER->available();
	// Serial.print(n);
	// Serial.print(" ");
	while(SER->available() > 25){
		byte aaa = SER->read();
		// Serial.print("dsadsa");
	}
	if(SER->available()){
		// do{
		// 	byte n = SER->read();
		// }while(n != 250);
		byte mydata = SER->read();
		if(mydata == 250){
			byte raw_receive_data[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
			bool receive_bad_flg = 0; //受信失敗フラグ
			for(int i=0; i<12; i++){
				if(SER->available()){
					raw_receive_data[i] = SER->read();
					if(raw_receive_data[i] == 250){
					// Serial.print("250data ");
					receive_bad_flg = 1; break;} //受信失敗フラグを立てる(250が来たら
					if(i == 9){
						if(raw_receive_data[9] != 50 && raw_receive_data[9] != 200){
							receive_bad_flg = 1; break;
						}
					}
				}else{
					// Serial.print("not data ");
					receive_bad_flg = 1; break;
				}
			}
			if(!receive_bad_flg){
				for(int i=0; i<12; i++){
					data[i] = raw_receive_data[i];
				}
				for(int i=0; i<12; i++){
					// Serial.print(data[i]);
					// Serial.print(" ");
				}
				// Serial.println();
			}else{
				// Serial.print("receive_bad_flg");
			}
		}
	}
}