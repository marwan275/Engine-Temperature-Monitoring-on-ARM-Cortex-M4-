/*************************************************************/
/* Module Name:   LED                                        */
/* File Name:   led.c                                        */
/* Author: Marwan Mobarak                                    */
/* Purpose:       Implements initialization and control      */
/*                functions for on-board 3 LEDS              */
/*************************************************************/

#include "led.h"


void white_led(void)
{
    clear_leds();
    Dio_WriteChannel(DioConf_RED_LED_CHANNEL_ID_INDEX, STD_HIGH);
    Dio_WriteChannel(DioConf_BLUE_LED_CHANNEL_ID_INDEX, STD_HIGH);
    Dio_WriteChannel(DioConf_GREEN_LED_CHANNEL_ID_INDEX, STD_HIGH);
}
void blue_led(void)
{
    clear_leds();
    Dio_WriteChannel(DioConf_BLUE_LED_CHANNEL_ID_INDEX, STD_HIGH);
}
void Red_led(void)
{
    clear_leds();
    Dio_WriteChannel(DioConf_RED_LED_CHANNEL_ID_INDEX, STD_HIGH);
}
void green_led(void)
{
    clear_leds();
    Dio_WriteChannel(DioConf_GREEN_LED_CHANNEL_ID_INDEX, STD_HIGH);
}
void clear_leds(void)
{
    Dio_WriteChannel(DioConf_RED_LED_CHANNEL_ID_INDEX, STD_LOW);
    Dio_WriteChannel(DioConf_BLUE_LED_CHANNEL_ID_INDEX, STD_LOW);
    Dio_WriteChannel(DioConf_GREEN_LED_CHANNEL_ID_INDEX, STD_LOW);
}
