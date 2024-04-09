#ifndef __SysTick__
#define __SysTick__
#include <std_types.h>
#include "tm4c123gh6pm.h"

#define SYSTICK_CTRL_REG          (*((volatile uint32 *)0xE000E010))
#define SYSTICK_RELOAD_REG        (*((volatile uint32 *)0xE000E014))
#define SYSTICK_CURRENT_REG       (*((volatile uint32 *)0xE000E018))
#define SYSTICK_PRIORITY_MASK  0x1FFFFFFF
#define SYSTICK_INTERRUPT_PRIORITY  3
#define SYSTICK_PRIORITY_BITS_POS   29

typedef enum
{
    SYSTICK_POOLING, SYSTICK_INTERRUPT_ENABLED,
} Systic_Mode;

typedef struct
{
    Systic_Mode mode;
    uint16 reload_value;
} Systick_config;

extern Systick_config config_systick_timer;

/**************************************************************/
/* Function Name:      Systick_INIT                            */
/* Sync/Async:         Synchronous                            */
/* Reentrancy:         Non-reentrant                          */
/* Parameters (in):    Pointer to a Systick_config structure   */
/* Description:        Initializes the SysTick timer based on  */
/*                     the provided configuration.            */
/**************************************************************/
void Systick_INIT(Systick_config *config);

/**************************************************************/
/* Function Name:      Systick_delay                          */
/* Sync/Async:         Synchronous                            */
/* Reentrancy:         Non-reentrant                          */
/* Parameters (in):    delay_ms - Delay time in milliseconds  */
/* Description:        Delays the program execution for the   */
/*                     specified time using the SysTick timer. */
/**************************************************************/
void Systick_delay(uint16 delay_ms);

/************************************************************************************
* Service Name: SysTick_Stop
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Stop the SysTick Timer.
************************************************************************************/
void SysTick_Stop(void);

/************************************************************************************
* Service Name: SysTick_SetCallBack
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): Ptr2Func - Call Back function address
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Setup the SysTick Timer call back
************************************************************************************/
void SysTick_SetCallBack(void (*Ptr2Func)(void));

#endif
