#include "SysTick.h"

uint32 systick_counter = 0;
uint8 systick_flag = 0;

/* Global pointer to function used to point upper layer functions
 * to be used in Call Back */
static void (*g_SysTick_Call_Back_Ptr)(void) = NULL_PTR;

/**************************************************************/
/* Function Name:      Systick_INIT                            */
/* Sync/Async:         Synchronous                            */
/* Reentrancy:         Non-reentrant                          */
/* Parameters (in):    Pointer to a Systick_config structure   */
/* Description:        Initializes the SysTick timer based on  */
/*                     the provided configuration.            */
/**************************************************************/
void Systick_INIT(Systick_config *config)
{
    SYSTICK_CTRL_REG = 0;
    SYSTICK_RELOAD_REG = config->reload_value - 1;
    SYSTICK_CURRENT_REG = 0;
    if (config->mode == SYSTICK_INTERRUPT_ENABLED)
    {
        SYSTICK_CTRL_REG = 0x7;
        /* Assign priority level 3 to the SysTick Interrupt */
        NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & SYSTICK_PRIORITY_MASK)
                | (SYSTICK_INTERRUPT_PRIORITY << SYSTICK_PRIORITY_BITS_POS);

    }
    else if (config->mode == SYSTICK_POOLING)
    {
        SYSTICK_CTRL_REG = 0x5;
    }
}

/**************************************************************/
/* Function Name:      Systick_delay                          */
/* Sync/Async:         Synchronous                            */
/* Reentrancy:         Non-reentrant                          */
/* Parameters (in):    delay_ms - Delay time in milliseconds  */
/* Description:        Delays the program execution for the   */
/*                     specified time using the SysTick timer. */
/**************************************************************/
void Systick_delay(uint16 delay_ms)
{
    SYSTICK_CTRL_REG = 0;
    SYSTICK_RELOAD_REG = (delay_ms * 16000) - 1;
    SYSTICK_CURRENT_REG = 0;
    SYSTICK_CTRL_REG = 0x5;
    while ((SYSTICK_CTRL_REG & (1 << 16)) == 0)
        ;
    SYSTICK_CTRL_REG = 0;
}

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
void SysTick_Stop(void)
{
    SYSTICK_CTRL_REG = 0; /* Disable the SysTick Timer by Clear the ENABLE Bit */
}

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
void SysTick_SetCallBack(void (*Ptr2Func)(void))
{
    g_SysTick_Call_Back_Ptr = Ptr2Func;
}



/************************************************************************************
 * Service Name: SysTick_Handler
 * Description: SysTick Timer ISR
 ************************************************************************************/
void SysTick_Handler(void)
{
    /* Check if the SysTick_SetCallBack is already called */
    if (g_SysTick_Call_Back_Ptr != NULL_PTR)
    {
        (*g_SysTick_Call_Back_Ptr)(); /* call the function in the scheduler using call-back concept */
    }
}
