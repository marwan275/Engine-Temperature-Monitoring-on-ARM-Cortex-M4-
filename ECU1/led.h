/*************************************************************/
/* Module Name:   LED                                        */
/* File Name:   led.c                                        */
/* Author: Marwan Mobarak                                    */
/* Purpose:       Implements initialization and control      */
/*                functions for on-board 3 LEDS              */
/*************************************************************/

#ifndef LED_H_
#define LED_H_

#include "Dio.h"
#include "Dio_Cfg.h"
#include "Std_Types.h"

/********************************************/
/* Function Name:      green_led            */
/* Sync/Async:         Synchronous           */
/* Reentrancy:         Non-reentrant         */
/* Parameters (in/out): None                 */
/* Return Value:       None                  */
/* Description: Turns on the green LED by setting the corresponding GPIO pin.
 *              Assumes the existence of functions:
 *                - clear_leds(): Clears the state of all LEDs.
 *                - GPIO_write_pin(index, state): Writes the specified state (HIGH/LOW) to the GPIO pin at the given index.
 *                  GREEN_LED_INDEX is assumed to be a defined constant representing the index of the green LED.
********************************************/
void green_led(void);

/********************************************/
/* Function Name:      Red_led              */
/* Sync/Async:         Synchronous           */
/* Reentrancy:         Non-reentrant         */
/* Parameters (in/out): None                 */
/* Return Value:       None                  */
/* Description: Turns on the red LED by setting the corresponding GPIO pin.
 *              Assumes the existence of functions:
 *                - clear_leds(): Clears the state of all LEDs.
 *                - GPIO_write_pin(index, state): Writes the specified state (HIGH/LOW) to the GPIO pin at the given index.
 *                  RED_LED_INDEX is assumed to be a defined constant representing the index of the red LED.
********************************************/
void Red_led(void);

/********************************************/
/* Function Name:      blue_led             */
/* Sync/Async:         Synchronous           */
/* Reentrancy:         Non-reentrant         */
/* Parameters (in/out): None                 */
/* Return Value:       None                  */
/* Description: Turns on the blue LED by setting the corresponding GPIO pin.
 *              Assumes the existence of functions:
 *                - clear_leds(): Clears the state of all LEDs.
 *                - GPIO_write_pin(index, state): Writes the specified state (HIGH/LOW) to the GPIO pin at the given index.
 *                  BLUE_LED_INDEX is assumed to be a defined constant representing the index of the blue LED.
********************************************/
void blue_led(void);

/********************************************/
/* Function Name:      white_led            */
/* Sync/Async:         Synchronous           */
/* Reentrancy:         Non-reentrant         */
/* Parameters (in/out): None                 */
/* Return Value:       None                  */
/* Description: Turns on the white LED by setting the corresponding GPIO pins.
 *              Assumes the existence of functions:
 *                - clear_leds(): Clears the state of all LEDs.
 *                - GPIO_write_pin(index, state): Writes the specified state (HIGH/LOW) to the GPIO pin at the given index.
 *                  RED_LED_INDEX, BLUE_LED_INDEX, GREEN_LED_INDEX are assumed to be defined constants representing LED indices.
********************************************/
void white_led(void);

/********************************************/
/* Function Name:      clear_leds           */
/* Sync/Async:         Synchronous           */
/* Reentrancy:         Non-reentrant         */
/* Parameters (in/out): None                 */
/* Return Value:       None                  */
/* Description: Turns off all LEDs by setting the corresponding GPIO pins to a LOW state.
 *              Assumes the existence of a function:
 *                - GPIO_write_pin(index, state): Writes the specified state (HIGH/LOW) to the GPIO pin at the given index.
 *              Assumes constants:
 *                - RED_LED_INDEX: Index of the red LED.
 *                - BLUE_LED_INDEX: Index of the blue LED.
 *                - GREEN_LED_INDEX: Index of the green LED.
********************************************/
void clear_leds(void);

#endif /* LED_H_ */
