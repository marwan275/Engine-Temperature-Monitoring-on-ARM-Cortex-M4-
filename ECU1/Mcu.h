 /******************************************************************************
 *
 * Module: Mcu
 *
 * File Name: Mcu.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Mcu Driver
 *
 * Author: Marwan Mobarak
 ******************************************************************************/


#ifndef MCU_H_
#define MCU_H_

#include <adc.h>
#include <UART.h>
#include "Std_Types.h"
#include "tm4c123gh6pm.h"
#include "SysTick.h"
#include "scheduler.h"
#include "Dio.h"
#include "Dio_Cfg.h"
#include "Port.h"
#include "Port_cfg.h"
#include "UART_Cfg.h"
#include "Can.h"
#include "eeprom.h"


#define BUS_FAULT_PRIORITY_MASK        0xFFFF1FFF
#define BUS_FAULT_PRIORITY                 1
#define BUS_FAULT_PRIORITY_BITS_POS        13
#define BUS_FAULT_ENABLE_BIT_POS           17

#define Enable_Exceptions()    __asm(" CPSIE I ")

#define Disable_Exceptions()   __asm(" CPSID I ")

#define Enable_Faults()        __asm(" CPSIE F ")

#define Disable_Faults()       __asm(" CPSID F ")


void Mcu_Init(void);

/**************************************************************/
/* Function Name:      BusFault_Init                           */
/* Sync/Async:         Synchronous                            */
/* Reentrancy:         Non-reentrant                          */
/* Parameters (in):    None                                    */
/* Description:        Initializes the Bus Fault Exception    */
/*                     by configuring its priority level and  */
/*                     enabling it.                            */
/**************************************************************/
void BusFault_Init(void);

#endif /* MCU_H_ */
