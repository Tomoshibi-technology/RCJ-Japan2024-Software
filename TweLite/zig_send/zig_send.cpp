#include <TWELITE>
#include <NWK_SIMPLE>

#define TIME_UP 0
#define TIME_OFF -1
#define ZIG_WAIT_MAX 100
#define ADC_FLT_MAX 4

//通信
const uint32_t APP_ID = 0x1234abcd; // application ID
const uint8_t CHANNEL = 13; // channel 11-26
const char APP_FOURCHAR[] = "TECH";

//送るときのやつ
MWX_APIRET txreq_stat;
MWX_APIRET transmit();
uint8_t se_to_adrs; //送信先のアドレス
uint8_t se_from_adrs; //送信元のアドレス 自分のアドレス

uint8_t my_adrs = 0x00; //自分のアドレス


const uint8_t LED0= mwx::PIN_DIGITAL::DIO9; 
const uint8_t LED1= mwx::PIN_DIGITAL::DIO8; 


void setup() {
	pinMode(LED0,OUTPUT);
	pinMode(LED1,OUTPUT);

	//ーーーーー通信設定ーーーーー
	txreq_stat = MWX_APIRET(false,0);

	the_twelite
		<< TWENET::appid(APP_ID)    
		<< TWENET::channel(CHANNEL) 
		<< TWENET::rx_when_idle();      
	auto&&	nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
					nwksmpl << NWK_SIMPLE::logical_id(my_adrs);
	the_twelite.begin();

	// //ーーーーーー有線通信設定ーーーーーー
	Serial1.setup(64, 192); // 64byte TX, 192byte RX
	Serial1.begin(115200);
	
	//ーーーーー起動ーーーーー
	Serial << "--- F446->TweLite ---" << mwx::crlf;
}


bool b_transmit = false; //送信中かどうか
int16_t timzigwait = TIME_OFF; //ZigBeeの待ち時間
uint8_t se_data[4] = {0,0,0,0}; //送信データ
uint16_t send_i = 0;



uint8_t receive_data[4] = {0,0,0,0};
void loop() {
	//ここからシリアル受信

	while(Serial1.available()) {
		uint8_t data = Serial1.read();
		Serial << format("[%d]", data) << mwx::crlf << mwx::flush;;
		if(data == (uint8_t)250){
			uint8_t raw_receive_data[4] = {0,0,0,0};
			bool receive_bad_flg = false;
			for(int i=0; i<4; i++){
				if(Serial1.available()){
					raw_receive_data[i] = Serial1.read();
					if(raw_receive_data[i] == (uint8_t)250||raw_receive_data[i] == (uint8_t)0){
						receive_bad_flg = true;
						break;
					}
				}else{
					receive_bad_flg = true;
					break;
				}
			}
			if(!receive_bad_flg){
				for(int i=0; i<4; i++){
					receive_data[i] = raw_receive_data[i];
					Serial << format("  %d  ", receive_data[i]);
				}
				Serial << mwx::crlf << mwx::flush;
			}
			break;
		}
	}

	//Serial << format("[%d]", data) << crlf;

	//ここまで
	se_data[0] = receive_data[0]; //10 + (send_i%200);
	se_data[1] = receive_data[1];//20 + (send_i%200);
	se_data[2] = receive_data[2];//30 + (send_i%200);
	se_data[3] = receive_data[3];//40 + (send_i%200);

	if(TickTimer.available()){ //1msごとに実行
		if(timzigwait > TIME_UP ){ //ZigBeeの待ち時間が終わったか
			timzigwait--;
		}
	}

	if (b_transmit) {
		digitalWrite(LED0,HIGH);
		digitalWrite(LED1,LOW);
		//送信受付は完了
		if (the_twelite.tx_status.is_complete(txreq_stat.get_value())) {
			//送信完了
			timzigwait = TIME_OFF; //-1
			b_transmit = false; 
			the_twelite.sleep(1);
		}else{
			//送信未完了
			if(timzigwait == TIME_UP){
				//送信完了待ち時間が終わった
				timzigwait = TIME_OFF; //-1
				b_transmit = false;
				the_twelite.sleep(1);
			}else{
				//送信中	
			}
		}
	}else{
		digitalWrite(LED0,LOW);
		digitalWrite(LED1,HIGH);
		//送信受付はしていない
		txreq_stat = transmit();
		if(txreq_stat){
			//Serial << "..sleep2." << mwx::crlf << mwx::flush;
			b_transmit = true; //送信しよう！！
			timzigwait = ZIG_WAIT_MAX; //100msなら待ってあげる
			send_i++; //送信データをいじっているだけ
		}else{
			//送信受け付け失敗
			//Serial << "..chk2." << mwx::crlf << mwx::flush;
			b_transmit = false; //送信できないよ
			the_twelite.sleep(1);
		}
	}	
}


/*** transmit a packet */
MWX_APIRET transmit() {
	se_from_adrs = my_adrs; //0x00
	se_to_adrs = 0xFE; //送信先のアドレス 0xFEはブロードキャスト

	if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
		// Serial 	<< "se_fr:" << format("0x%X", se_from_adrs) 
		// 				<< "  se_to:"	<< format("0x%X", se_to_adrs)
		// 				<< "  data: "	<< (int)se_data[0] 
		// 				<< " : "	<< (int)se_data[1]
		// 				<< " : "	<< (int)se_data[2]
		// 				<< " : "	<< (int)se_data[3]
		// 				<< mwx::crlf << mwx::flush;

		pkt << tx_addr(se_to_adrs) 
				<< tx_retry(0x2) //送信トライ回数
				<< tx_packet_delay(0,0,2);	//最低待ち時間、最長待ち時間、再送間隔
																		//直ちに再送、2ms待って再送

		pack_bytes(
			pkt.get_payload(),
			make_pair(APP_FOURCHAR, 4), //4バイトの文字列
			se_from_adrs, //自分のアドレス uint8_t
			se_data[0],   //uint8_t
			se_data[1],   //uint8_t
			se_data[2],   //uint8_t
			se_data[3]    //uint8_t
		);
		return pkt.transmit();
	}
	return MWX_APIRET(false, 0);
}
