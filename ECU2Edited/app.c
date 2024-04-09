/******************************************************************************
 *
 * Module: Application
 *
 * File Name: App.c
 *
 * Description: Source file for Application Tasks.
 *
 * Author: Marwan Mobarak
 ******************************************************************************/

#include "app.h"

tCANMsgObject TempMessage, KeepAliveReceiveMessage, voltageMessage,
        buttonMessage;
uint32 ui32MsgData;
uint8 *pui8MsgData;

uint32 ui32VoltData;
uint32 *pui8VoltData;

uint32 ui32buttonData;
uint32 *pui32buttonData;

uint8 pui8ReceivedKeepAliveData[8];

volatile uint32 g_ui32MsgCount = 0;
volatile bool g_bErrFlag = 0;

uint32 systemState = 0;
uint16 ledState = 0;
uint16 buttonState = 0;

uint16 keepaliveTimer = 0;
uint16 overheatTimer = 0;
uint16 systemfaultTimer = 0;

void CANIntHandler(void)
{
    uint32 ui32Status;
    ui32Status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);
    if (ui32Status == CAN_INT_INTID_STATUS)
    {
        ui32Status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
        g_bErrFlag = 1;
    }
    else if (ui32Status == 1)
    {
        CANIntClear(CAN0_BASE, 1);
        g_ui32MsgCount++;
        g_bErrFlag = 0;
    }
    else if (ui32Status == 2)
    {
        CANIntClear(CAN0_BASE, 2);
        KeepAliveReceiveMessage.pui8MsgData = pui8ReceivedKeepAliveData;
        CANMessageGet(CAN0_BASE, 2, &KeepAliveReceiveMessage, 0);
        systemState = *KeepAliveReceiveMessage.pui8MsgData;
        g_bErrFlag = 0;
    }
    else if (ui32Status == 3)
    {
        CANIntClear(CAN0_BASE, 3);
        g_ui32MsgCount++;
        g_bErrFlag = 0;
    }
    else if (ui32Status == 4)
    {
        CANIntClear(CAN0_BASE, 4);
        g_ui32MsgCount++;
        g_bErrFlag = 0;
    }
    else
    {

    }
}

void App_init()
{

    KeepAliveReceiveMessage.ui32MsgID = 0x1002;
    KeepAliveReceiveMessage.ui32MsgIDMask = 0;
    KeepAliveReceiveMessage.ui32Flags = MSG_OBJ_RX_INT_ENABLE;
    KeepAliveReceiveMessage.ui32MsgLen = 8;
    CANMessageSet(CAN0_BASE, 2, &KeepAliveReceiveMessage, MSG_OBJ_TYPE_RX);

    pui8MsgData = (uint8*) &ui32MsgData;
    pui8VoltData = (uint32*) &ui32VoltData;

    pui32buttonData = (uint32*) &ui32buttonData;

    TempMessage.ui32MsgID = 1;
    TempMessage.ui32MsgIDMask = 0;
    TempMessage.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
    TempMessage.ui32MsgLen = sizeof(pui8MsgData);
    TempMessage.pui8MsgData = pui8MsgData;

    voltageMessage.ui32MsgID = 3;
    voltageMessage.ui32MsgIDMask = 0;
    voltageMessage.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
    voltageMessage.ui32MsgLen = sizeof(pui8VoltData);
    voltageMessage.pui8MsgData = pui8VoltData;

    buttonMessage.ui32MsgID = 4;
    buttonMessage.ui32MsgIDMask = 0;
    buttonMessage.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
    buttonMessage.ui32MsgLen = sizeof(pui8VoltData);
    buttonMessage.pui8MsgData = pui32buttonData;

    task_init(test_task_1, 500);
    task_init(LEDTASK, 500);
    task_init(GetButtons, 10);
}
uint32 voltageRead(void)
{
    // Number of readings to take

    // Storage for the readings
    uint32 readings[16];

    // Take multiple readings
    for (int i = 0; i < 16; ++i)
    {
        readings[i] = ADC_Read(ADC1_INDEX); // Assuming channel 0, adjust as needed
    }

    // Calculate the average
    uint32 sum = 0;
    for (int i = 0; i < AVG_READINGS; ++i)
    {
        sum += readings[i];
    }

    uint32 average = sum / AVG_READINGS;

    return average;
}
void test_task_1(void)
{
    if ((systemState != SYSTEM_FAULT_ERROR_CODE))
    {
        uint32 temperature = (int) (temperatureSensorRead() * 330.0 / 4095.0);
        ui32MsgData = temperature;
        CANMessageSet(CAN0_BASE, 1, &TempMessage, MSG_OBJ_TYPE_TX);
        intToAsciiAndSendPC(temperature);
        sendPC("\n");
        uint32 const_volt = (voltageRead() * 255.0 / 4095.0);
        intToAsciiAndSendPC(const_volt);
        sendPC("\n");
        ui32VoltData = const_volt;
        CANMessageSet(CAN0_BASE, 3, &voltageMessage, MSG_OBJ_TYPE_TX);
    }
}
void GetButtons(void)
{
    uint8 buttonState = 5;
    uint8 buttonStatus = 5;
    if (systemState != NO_ERROR_CODE)
    {
        buttonState = get_input();
        switch (systemState)
        {
        case NO_ERROR_CODE:
            buttonState = 0;
            break;

        case OVERHEATING_ERROR_CODE:
            if (buttonState == BTN1)
            {
                buttonStatus = BTN1;
                sendPC("BTN1");
                sendPC("\n");
            }
            break;

        case SYSTEM_FAULT_ERROR_CODE:
            if (buttonState == BOTH_BTNS)
            {
                buttonStatus = BOTH_BTNS;
                sendPC("BOTH");
                sendPC("\n");
            }
            break;

        case COMMUNICATION_ERROR_CODE:
            if (buttonState == BTN2)
            {
                buttonStatus = BTN2;
                sendPC("BTN2");
                sendPC("\n");
            }
            break;
        }
        ui32buttonData = buttonStatus;
        CANMessageSet(CAN0_BASE, 4, &buttonMessage, MSG_OBJ_TYPE_TX);
    }

}
void LEDTASK(void)
{
    switch (systemState)
    {
    case NO_ERROR_CODE:
        green_led();
        ledState = Led_State_GREEN;
        break;

    case OVERHEATING_ERROR_CODE:
        overheatTimer += 1;
        if (ledState != Led_State_WHITE || ledState == Led_State_OFF)
        {
            white_led();
            ledState = Led_State_WHITE;
        }
        else
        {
            clear_leds();
            ledState = Led_State_OFF;
        }
        break;

    case SYSTEM_FAULT_ERROR_CODE:
        systemfaultTimer += 1;
        if (ledState != Led_State_RED || ledState == Led_State_OFF)
        {
            Red_led();
            ledState = Led_State_RED;
        }
        else
        {
            clear_leds();
            ledState = Led_State_OFF;
        }
        break;

    case COMMUNICATION_ERROR_CODE:
        if (ledState != Led_State_BLUE || ledState == Led_State_OFF)
        {
            blue_led();
            ledState = Led_State_BLUE;
        }
        else
        {
            clear_leds();
            ledState = Led_State_OFF;
        }
        break;
    }
}
