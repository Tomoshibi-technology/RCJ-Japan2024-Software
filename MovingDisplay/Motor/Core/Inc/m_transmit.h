#pragma once

#include "main.h"
#include "stm32g0xx_hal_conf.h"
#include "stm32g0xx_it.h"
#include <stdint.h>

class M_TRANSMIT{
	private:
		uint8_t rcvBuf[3];
		uint8_t ID;
		UART_HandleTypeDef* UART;
		uint8_t send_ID;
	public:
		M_TRANSMIT(UART_HandleTypeDef* uart, uint8_t id);
		int get_speed();
};