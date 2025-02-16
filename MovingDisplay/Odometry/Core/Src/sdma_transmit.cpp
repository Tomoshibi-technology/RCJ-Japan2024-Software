#include "sdma_transmit.h"

SDMA_TRANSMIT::SDMA_TRANSMIT(UART_HandleTypeDef* uart, uint8_t id){
    UART = uart;
    ID = id;
}

void SDMA_TRANSMIT::begin_dma(){
    HAL_UART_Receive_DMA(UART, rxBuf, sizeof(rxBuf));
    HAL_Delay(1);
}

void SDMA_TRANSMIT::check_buf(long data){
	index = UART->hdmarx->Instance->CNDTR;//バッファー残容量
 	index = sizeof(rxBuf) - index;//最新の受信データ位置
 
 	int remainData = index - indexRead;//まだ読み込んでいないデータ数
 	if(remainData < 0){remainData = remainData + sizeof(rxBuf);}
 	//読み込み済みデータ位置より最新の受信データ位置が前にある時(バッファー内で受信データが一周してた場合)値を補正
 
 	if(remainData > 0){
 		readData = rxBuf[indexRead];//データ読み込み
        if(readData==190+ID){send_data(data);};
 		indexRead++;
 		if(indexRead == sizeof(rxBuf)){indexRead = 0;}
    }

}



void SDMA_TRANSMIT::send_data(long data){
    send_array[0]=190+ID;
    data += 5000;

    if(data<0){data=9999;}
    else if(data>9999){data=9999;}

	for(int i = 1; i <3 ;i++){
		send_array[i] = data%100;
		data = (int)data/100;
	}
    HAL_UART_Transmit(UART, send_array, 3, 100);
}



