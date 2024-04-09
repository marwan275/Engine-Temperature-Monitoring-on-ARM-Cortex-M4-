/******************************************************************************
 *
 * Module: Gpt
 *
 * File Name: Gpt.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - General Purpose Timer Driver.
 *
 * Author: Marwan Mobarak
 ******************************************************************************/

#include "Gpt.h"
void delay_timerA(int mtime)
{
    volatile int i;
    SYSCTL_RCGCTIMER_R |= 1; /*enable clock for TimerA */
    TIMER0_CTL_R = 0; /*disable timer before initialization */
    TIMER0_CFG_R = 0x04; /*16-bit option */
    TIMER0_TAMR_R = 0x02; /*periodic mode and down counter */
    TIMER0_TAILR_R = 16000 - 1; /*TimerA interval load value register */
    TIMER0_ICR_R = 0x01; /*clear the TimerA timeout flag */
    TIMER0_CTL_R |= 0x01; /*enable TimerA after initialization */
    for (i = 0; i < mtime*10; i++)
    {
        while ((TIMER0_RIS_R & 0x01) == 1)
            ; /*wait for TimerA timeout flag */
        TIMER0_ICR_R = 0x01; /*clear the TimerA timeout flag */
    }
}

