
#include "sdma_transmit.h"

SDMA_TRANSMIT::SDMA_TRANSMIT(UART_HandleTypeDef* uart, uint8_t id){
    UART = uart;
    ID = id;
}

void SDMA_TRANSMIT::begin_dma(){
    HAL_UART_Receive_DMA(UART, rxBuf, sizeof(rxBuf));
    HAL_Delay(1);
}

void SDMA_TRANSMIT::check_buf(){
	index = UART->hdmarx->Instance->CNDTR;
 	index = sizeof(rxBuf) - index;

 	int check_buf_point = index - 21;
 	if(check_buf_point < 0){check_buf_point = check_buf_point + sizeof(rxBuf);}

	while(1){
		readData = rxBuf[check_buf_point];
		if(readData == 220+ID){
			for(int i=1; i<10; i++){
				int read_buf_point = check_buf_point + i;
				if(read_buf_point>sizeof(rxBuf)-1){read_buf_point = read_buf_point - sizeof(rxBuf);}
				rcvBuf[i-1] = rxBuf[read_buf_point];
			}
			break;
		}
		check_buf_point++;
		if(check_buf_point>sizeof(rxBuf)-1){check_buf_point = check_buf_point - sizeof(rxBuf);}
		if(check_buf_point==index){break;}
	}

 	travel_x = 0; int hyaku = 1;
	for(int i = 0; i <2 ; i++){
		travel_x = travel_x + (rcvBuf[i]*hyaku);
		hyaku = hyaku*100;
	}
	travel_x -= 5000;

	circle_x = 0; hyaku = 1;
	for(int i = 2; i <4 ; i++){
		circle_x = circle_x + (rcvBuf[i]*hyaku);
		hyaku = hyaku*100;
	}
	circle_x -= 5000;

	circle_z = 0; hyaku = 1;
	for(int i = 4; i <6 ; i++){
		circle_z = circle_z + (rcvBuf[i]*hyaku);
		hyaku = hyaku*100;
	}
	circle_z -= 5000;

	radius = rcvBuf[6];
	in_hue1 = rcvBuf[7];
	in_hue2 = rcvBuf[8];

}

/*
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
*/

