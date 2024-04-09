/*
 * UART_reg.h
 *
 *  Created on: Jan 6, 2024
 *      Author: Marwan Mobarak
 */

#ifndef UART_REG_H_
#define UART_REG_H_

#define UART0_BASE_ADDRESS 0x4000C000
#define UART1_BASE_ADDRESS 0x4000D000
#define UART2_BASE_ADDRESS 0x4000E000
#define UART3_BASE_ADDRESS 0x4000F000
#define UART4_BASE_ADDRESS 0x40010000
#define UART5_BASE_ADDRESS 0x40011000
#define UART6_BASE_ADDRESS 0x40012000
#define UART7_BASE_ADDRESS 0x40013000


#define UART_DATA_OFFSET       0x000
#define UART_RSR_ECR_OFFSET    0x004
#define UART_FR_OFFSET         0x018
#define UART_ILPR_OFFSET       0x020
#define UART_IBRD_OFFSET       0x024
#define UART_FBRD_OFFSET       0x028
#define UART_LCRH_OFFSET       0x02C
#define UART_CTL_OFFSET        0x030
#define UART_IFLS_OFFSET       0x034
#define UART_IM_OFFSET         0x038
#define UART_RIS_OFFSET        0x03C
#define UART_MIS_OFFSET        0x040
#define UART_ICR_OFFSET        0x044
#define UART_DMACTL_OFFSET     0x048
#define UART_9BIT_ADDR_OFFSET  0x0A4
#define UART_9BIT_MASK_OFFSET  0x0A8
#define UART_PP_OFFSET         0xFC0
#define UART_CC_OFFSET         0xFC8




#endif /* UART_REG_H_ */
