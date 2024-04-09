#ifndef __Scheduler__
#define __Scheduler__

#include <std_types.h>
#include "tm4c123gh6pm.h"
#include "Mcu.h"

#define OS_overflow 5001



typedef struct{
  void (*pointer)(void);
  uint16 period;
}task;

extern uint32 systick_counter;
extern uint8 systick_flag;

/**************************************************************/
/* Function Name:      OS_start                              */
/* Sync/Async:         Synchronous                            */
/* Reentrancy:         Non-reentrant                          */
/* Parameters (in):    None                                    */
/* Description:        Simple task scheduler using SysTick     */
/**************************************************************/
void OS_start(void);

/**************************************************************/
/* Function Name:      task_init                               */
/* Sync/Async:         Synchronous                            */
/* Reentrancy:         Non-reentrant                          */
/* Parameters (in):    - p: Function pointer to the task      */
/*                     - period: Period of the task (in ticks) */
/* Description:        Initializes a task by storing its      */
/*                     function pointer and period in an array */
/**************************************************************/
void task_init(void (*p)(void),int period);


/* Description: Function called by the Timer Driver in the MCAL layer using the call back pointer */
void Os_NewTimerTick(void);





#endif
