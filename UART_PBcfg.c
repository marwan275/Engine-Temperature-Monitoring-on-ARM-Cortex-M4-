/*
 * UART_PBcfg.c
 *
 *  Created on: Jan 6, 2024
 *      Author: Marwan Mobarak
 */

#ifndef UART_PBCFG_C_
#define UART_PBCFG_C_

#include "UART_Cfg.h"

UART_Config UART_PBConfiguration[UART_CONFIGURED_MODULES] = {{
        .BaudRate = RATE_9600, .UART_num = UART0, .clock_source = SYSTEM_CLOCK,
        .length = DATA_8BITS, .mode = UART_POOLING, .pins_mode = TX_RX} };

#endif /* UART_PBCFG_C_ */
