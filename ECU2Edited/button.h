/*************************************************************/
/* Module Name:   Button                                     */
/* File Name:   Button.h                                     */
/* Author: Marwan Mobarak                                    */
/* Purpose:       Definitions and prototypes                 */
/*                for input buttons (SW1 and SW2)            */
/*************************************************************/

#ifndef BUTTON_H_
#define BUTTON_H_

#include "std_types.h"
#include "tm4c123gh6pm.h"
#include "Dio.h"
#include "Dio_Cfg.h"
#include "Gpt.h"

#define DEBOUNCE_DELAY_MS 15
#define BUTTON_PRESSED  STD_LOW
#define BUTTON_RELEASED STD_HIGH

typedef enum{
    NONE,
    BTN1,
    BTN2,
    BOTH_BTNS,
}PRESSED_BTN;

uint8 Button_getState(uint8 SW_index);
PRESSED_BTN get_input(void);

#endif /* BUTTON_H_ */
