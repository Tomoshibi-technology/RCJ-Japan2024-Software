/*
Zigbeeを受信して
F446に送信する
*/

#include <TWELITE>
#include <NWK_SIMPLE>

//通信
const uint32_t APP_ID = 0x1234abcd;
const uint8_t CHANNEL = 13;
const char APP_FOURCHAR[] = "TECH";

//関数
void receive();

//読み取るときのやつ
uint8_t re_to_adrs; //送信先のアドレス 自分のアドレス
uint8_t re_from_adrs; //送信元のアドレス


//ピン
const uint8_t DIP0= mwx::PIN_DIGITAL::DIO13; //adrs1
const uint8_t DIP1= mwx::PIN_DIGITAL::DIO16; //adrs2
const uint8_t LED0= mwx::PIN_DIGITAL::DIO9; 
const uint8_t LED1= mwx::PIN_DIGITAL::DIO8; 

uint8_t my_adrs; //自分のアドレス

void setup() {	
	//ーーーーーID読み取りーーーーー
	pinMode(DIP0,INPUT);
	pinMode(DIP1,INPUT);
	pinMode(LED0,OUTPUT);
	pinMode(LED1,OUTPUT);
	delay(10);
	my_adrs += 0xA0;//子機のアドレスを計算
	my_adrs += digitalRead(DIP1) << 1;//子機のアドレスを計算
	my_adrs += digitalRead(DIP0); //子機のアドレスを計算
	digitalWrite(LED0,digitalRead(DIP0));
	digitalWrite(LED1,digitalRead(DIP1));
	
	//ーーーーーm無線通信設定ーーーーー
	the_twelite
		<< TWENET::appid(APP_ID) 
		<< TWENET::channel(CHANNEL) 
		<< TWENET::rx_when_idle(); 
	auto&& 	nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
					nwksmpl << NWK_SIMPLE::logical_id(my_adrs); //0x00→マスター 0xFE→スレーブ
	the_twelite.begin(); // start twelite!

	// //ーーーーーー有線通信設定ーーーーーー
	Serial1.setup(192, 192); // 64byte TX, 192byte RX
	Serial1.begin(115200);

	//ーーーーー起動ーーーーー
	Serial << "--- TweLite->F446 ---" << mwx::crlf;
}

int16_t mytimer = -1;

uint8_t re_data[4] = {0,0,0,0}; //送信データ

void loop() {
	//ーーーーー受信ーーーーー
	while (the_twelite.receiver.available()) { //受信待ち 受信したデータがなくなるまで繰り返す
		//Serial << "kita";
		receive();
		Serial 	<< "re_fr:" << format("0x%X", re_from_adrs) //送信元のアドレス 16進数で表示してます
						<< "  data: " << (int)re_data[0]
						<< " : " << (int)re_data[1]
						<< " : " << (int)re_data[2]
						<< " : " << (int)re_data[3]
						<< mwx::crlf << mwx::flush;
	}
	if(TickTimer.available()){ //1msごとに実行
		mytimer++;
	}
	if(mytimer >= 10){//10msごとに実行
		Serial1 << (byte)250; //スタートビット
		for(int dataId=0;dataId<4;dataId++){
			Serial1 << (byte)re_data[dataId];
		}
		mytimer = 0;
	}

	//ーーーーーF446送信ーーーーー
	// Serial1 << (byte)250; //スタートビット
	// for(int i=0;i<4;i++){
	// 	Serial1 << (byte)re_data[i];
	// }
	// Serial1 << mwx::flush;
}

/*add function recive()*/
void receive() {
	auto&& rx = the_twelite.receiver.read(); //受信値をRXに読み込む
	char fourchars[5]{}; // 判定用
	auto&& np = expand_bytes(
								rx.get_payload().begin(), 
								rx.get_payload().end(), 
								make_pair((uint8_t*)fourchars, 4) //fourcharsに4バイトの文字列を格納
							);

	if (strncmp(APP_FOURCHAR, fourchars, 4)) {
		return;
	}
	expand_bytes(
		np,
		rx.get_payload().end(),
		re_from_adrs, //uint8_t
		re_data[0], //uint8_t
		re_data[1], //uint8_t
		re_data[2], //uint8_t
		re_data[3]  //uint8_t
	);
}


