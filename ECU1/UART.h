/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the TM4C123GH6PM UART driver
 *
 * Author: Marwan Mobarak
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"
#include "tm4c123gh6pm.h"
#include "UART_Cfg.h"
#include "UART_reg.h"
/*******************************************************************************
 *                             PreProcessor Macros                             *
 *******************************************************************************/

typedef enum
{
    DATA_5BITS, DATA_6BITS, DATA_7BITS, DATA_8BITS
} UART_Datalength;
typedef enum
{
    RATE_9600, RATE_115200
} UART_BaudRate;
typedef enum
{
    UART_POOLING, UART_INTERRUPT
} UART_OperationMode;
typedef enum
{
    SYSTEM_CLOCK, PIOSC
} UART_ClockSource;
typedef enum
{
    UART0, UART1, UART2, UART3, UART4, UART5, UART6, UART7
} UART_ModuleNum;

typedef enum
{
    TX, RX, TX_RX,
} UART_PinsMode;
typedef struct
{
    UART_ModuleNum UART_num;
    UART_BaudRate BaudRate;
    UART_Datalength length;
    UART_OperationMode mode;
    UART_ClockSource clock_source;
    UART_PinsMode pins_mode;
} UART_Config;

extern UART_Config UART_PBConfiguration[];

#define UART_LCRH_WLEN_BITS_POS  5
#define UART_CTL_UARTEN_MASK     0x00000001
#define UART_CTL_TXE_MASK        0x00000100
#define UART_CTL_RXE_MASK        0x00000200
#define UART_FR_TXFE_MASK        0x00000080
#define UART_FR_RXFE_MASK        0x00000010

/*******************************************************************************
 *                            Functions Prototypes                             *
 *******************************************************************************/

void UART_Init(UART_Config *ConfigPtr);

void UART0_SendByte(uint8 data);

uint8 UART0_ReceiveByte(void);

void UART0_SendData(const uint8 *pData, uint32 uSize);

void UART0_ReceiveData(uint8 *pData, uint32 uSize);

void UART0_SendString(const uint8 *pData);

void UART0_ReceiveString(uint8 *pData);

void LCD_intgerToString(int data);


STATIC void UART_SetClock(UART_ModuleNum UART_Num);

#endif
