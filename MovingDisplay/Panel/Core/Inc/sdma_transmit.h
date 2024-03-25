
#pragma once
#include "main.h"
#include "stm32g0xx_hal_conf.h"
#include "stm32g0xx_it.h"

class SDMA_TRANSMIT{
    private:
        uint8_t rxBuf[128];
        int indexRead = 0;
        uint8_t readData = 0;
        uint8_t ID;
        uint8_t rcvBuf[9];
        //uint8_t send_array[3];
        UART_HandleTypeDef* UART;

    public:
        SDMA_TRANSMIT(UART_HandleTypeDef* uart, uint8_t id);
        void begin_dma();
        void check_buf();
//        void send_data(long data);
        int index;
        int travel_x; int circle_x; int circle_z; uint8_t radius; int in_hue1; int in_hue2;
};


