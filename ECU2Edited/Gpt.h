 /******************************************************************************
 *
 * Module: Gpt
 *
 * File Name: Gpt.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - General Purpose Timer Driver.
 *
 * Author: Marwan Mobarak
 ******************************************************************************/


#ifndef GPT_H_
#define GPT_H_
#include "tm4c123gh6pm.h"

/********************************************/
/* Function Name:      delay_timerA          */
/* Sync/Async:         Synchronous           */
/* Reentrancy:         Non-reentrant         */
/* Parameters (in/out):                      */
/*   - mtime (int): Time delay in milliseconds. */
/* Return Value:       None                  */
/* Description: Delays the program execution for the specified time using TimerA.
 *              Assumes that the Timer0 module is used for this purpose.
 *              Configures TimerA as a 16-bit periodic timer in down-counter mode.
 *              Uses busy-waiting to achieve the delay.
 *              The function may not be precise for very short delays due to busy-waiting.
 *              Make sure the Timer0 module is available on the target microcontroller.
*/
void delay_timerA(int ttime);

#endif /* GPT_H_ */
