/************************************************************/
/* Module Name:   Button                                    */
/* File Name:   Button.c                                    */
/* Author: Marwan Mobarak                                   */
/* Purpose:       Implements initialization and control     */
/*                functions for input buttons (SW1 and SW2) */
/************************************************************/

#include "button.h"

uint8 Button_getState(Dio_ChannelType SW_index)
{
    if (Dio_ReadChannel(SW_index))
    {
        delay_timerA(DEBOUNCE_DELAY_MS);
        if (Dio_ReadChannel(SW_index))
            return BUTTON_PRESSED;
    }
    return BUTTON_RELEASED;
}

PRESSED_BTN get_input(void)
{
    if (Button_getState(DioConf_SW1_CHANNEL_ID_INDEX))
    {
        while (Button_getState(DioConf_SW1_CHANNEL_ID_INDEX))
        {
            if (Button_getState(DioConf_SW2_CHANNEL_ID_INDEX))
            {
                return BOTH_BTNS;
            }
        }
        return BTN1;
    }
    if (Button_getState(DioConf_SW2_CHANNEL_ID_INDEX))
    {
        while (Button_getState(DioConf_SW2_CHANNEL_ID_INDEX))
        {
            if (Button_getState(DioConf_SW1_CHANNEL_ID_INDEX))
            {
                return BOTH_BTNS;
            }
        }
        return BTN2;
    }
    return NONE;
}
