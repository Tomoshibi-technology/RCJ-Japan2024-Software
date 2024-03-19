#include "m_transmit.h"

M_TRANSMIT::M_TRANSMIT(UART_HandleTypeDef* uart, uint8_t id){
    UART=uart;
    ID=id;
    send_ID = id + 190;
}

int M_TRANSMIT::get_travel(){
    HAL_UART_Transmit(UART, &send_ID, 1, 100);
    HAL_UART_Receive(UART, rcvBuf, 3, 100);

    int hyaku = 1;
    int travel = 0;

    for(int j = 1; j <3 ; j++){
        travel = travel + (rcvBuf[j]*hyaku);
        hyaku = hyaku*100;
    }
    travel-=5000;
    HAL_Delay(1);

    return travel;

}