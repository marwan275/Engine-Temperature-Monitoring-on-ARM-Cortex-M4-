/******************************************************************************
 *
 * Module: Mcu
 *
 * File Name: Mcu.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Mcu Driver
 *
 * Author: Marwan Mobarak
 ******************************************************************************/

#include "Mcu.h"

void Mcu_Init(void)
{
    Enable_Exceptions();
    Enable_Faults();
    BusFault_Init();
    Systick_INIT(&config_systick_timer);
    SysTick_SetCallBack(Os_NewTimerTick);
    UART_Init(UART_PBConfiguration);
    ADC_Init_Start(ADC_Configuartions);
    Port_Init(Port_Configuration);
    Dio_Init(&Dio_Configuration);
    eeprom_init();
    caninit();
}
void BusFault_Init(void)
{
    /* Assign priority level 1 to the Bus Fault Exception */
    NVIC_SYS_PRI1_R = (NVIC_SYS_PRI1_R & BUS_FAULT_PRIORITY_MASK)
            | (BUS_FAULT_PRIORITY << BUS_FAULT_PRIORITY_BITS_POS);

    /* Enable the Bus Fault Exception */
    NVIC_SYS_HND_CTRL_R |= (1 << BUS_FAULT_ENABLE_BIT_POS);

}
void Bus_Fault_Handler(void)
{
    while (1)
    {
    }
}
