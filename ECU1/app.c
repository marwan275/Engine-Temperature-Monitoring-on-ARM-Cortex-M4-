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

tCANMsgObject sCANMessage, KeepAliveMessage, VoltageMsg, ButtonMsg;
uint8 pui8MsgData[8];
uint8 pui8VoltageData[8];
uint8 pui8ButtonData[8];

uint32 ui32KeepAliveData;
uint8 *pui8KeepAliveData;

volatile uint32 g_ui32MsgCount = 0;
volatile bool g_bErrFlag = 0;
uint32 TempRxFlag = 0;

uint32 temperature = 101;
uint32 voltagevalue = 0;

uint8 memoryerror = 0;
uint16 KeepAliveError = 0;

uint16 TempErrorCounter = 0;
uint16 SystemfaultErrorCounter = 0;
uint16 KeepAliveErrorCounter = 0;
uint16 OverheatErrorCounter = 0;

uint32 systemState = 0;
uint16 ledState = 0;
uint16 buttonState = 0;

uint16 keepaliveTimer = 0;
uint16 overheatTimer = 0;
uint16 systemfaultTimer = 0;

uint32 KeepAliveSent = 0;

uint8 Ecu2Buttons = 0;

void App_init()
{

//    //*********************************************************************************
    sCANMessage.ui32MsgID = 1;
    sCANMessage.ui32MsgIDMask = 0;
    sCANMessage.ui32Flags = MSG_OBJ_RX_INT_ENABLE;
    sCANMessage.ui32MsgLen = 8;
    CANMessageSet(CAN0_BASE, 1, &sCANMessage, MSG_OBJ_TYPE_RX);

//    //*********************************************************************************
    pui8KeepAliveData = (uint8*) &ui32KeepAliveData;

    KeepAliveMessage.ui32MsgID = 0x1002;
    KeepAliveMessage.ui32MsgIDMask = 0;
    KeepAliveMessage.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
    KeepAliveMessage.ui32MsgLen = sizeof(pui8KeepAliveData);
    KeepAliveMessage.pui8MsgData = pui8KeepAliveData;

//    //*********************************************************************************

    VoltageMsg.ui32MsgID = 3;
    VoltageMsg.ui32MsgIDMask = 0;
    VoltageMsg.ui32Flags = MSG_OBJ_RX_INT_ENABLE;
    VoltageMsg.ui32MsgLen = 8;
    CANMessageSet(CAN0_BASE, 3, &VoltageMsg, MSG_OBJ_TYPE_RX);
//    //*********************************************************************************

    ButtonMsg.ui32MsgID = 4;
    ButtonMsg.ui32MsgIDMask = 0;
    ButtonMsg.ui32Flags = MSG_OBJ_RX_INT_ENABLE;
    ButtonMsg.ui32MsgLen = 8;
    CANMessageSet(CAN0_BASE, 4, &ButtonMsg, MSG_OBJ_TYPE_RX);
//    //*********************************************************************************
//eeprom_write(0,ERROR_LOCATION_EEPROM, ERROR_BLOCK_EEPROM);

    systemState = eeprom_read(ERROR_LOCATION_EEPROM, ERROR_BLOCK_EEPROM);

    task_init(MainTask, 500);
    task_init(PCcommunication, 1000);
    task_init(KeepAlive, 100);
    task_init(LEDTASK, 500);
    task_init(GetButtons, 10);
    task_init(ChangeState, 50);

}
void MainTask(void)
{
    if (systemState == NO_ERROR_CODE)
    {
        if (temperature != 101)
        {
            if ((temperature > 25 || temperature < 10))
                TempErrorCounter++;
            else
            {
                TempErrorCounter = 0;
            }

            if (TempErrorCounter != 0 && TempErrorCounter % 6 == 0)
            {
                if (voltagevalue >= 2.5 && voltagevalue <= 3.5)
                {
//                    OverheatErrorCounter += 1;
                    SystemfaultErrorCounter = 0;
                    systemState = OVERHEATING_ERROR_CODE;
                }
                else
                {
                    OverheatErrorCounter = 0;
//                    SystemfaultErrorCounter += 1;
                    systemState = SYSTEM_FAULT_ERROR_CODE;
                }
            }
        }
        else
        {
            if (TempRxFlag == 1)
            {
                TempRxFlag = 0;
                KeepAliveError = 0;
                KeepAliveErrorCounter = 0;
            }
            else
                KeepAliveError += 1;
        }
    }
}

void KeepAlive(void)
{
    ui32KeepAliveData = systemState;
    CANMessageSet(CAN0_BASE, 2, &KeepAliveMessage, MSG_OBJ_TYPE_TX);

    if (KeepAliveError >= 10)
    {
        systemState = COMMUNICATION_ERROR_CODE;
    }

}
void ChangeState(void)
{
    switch (systemState)
    {
    case NO_ERROR_CODE:
        if (eeprom_read(ERROR_LOCATION_EEPROM,
        ERROR_BLOCK_EEPROM) != NO_ERROR_CODE)
        {
            eeprom_write(NO_ERROR_CODE, ERROR_LOCATION_EEPROM,
            ERROR_BLOCK_EEPROM);
        }
        break;

    case SYSTEM_FAULT_ERROR_CODE:
        if (eeprom_read(ERROR_LOCATION_EEPROM,
        ERROR_BLOCK_EEPROM) != SYSTEM_FAULT_ERROR_CODE)
        {
            eeprom_write(SYSTEM_FAULT_ERROR_CODE, ERROR_LOCATION_EEPROM,
            ERROR_BLOCK_EEPROM);
        }
        if (buttonState == BOTH_BTNS && systemfaultTimer <= 60
                && Ecu2Buttons == BOTH_BTNS)
        {
            systemfaultTimer = 0;
            systemState = NO_ERROR_CODE;
            SystemfaultErrorCounter += 1;
            Ecu2Buttons = NONE;
            buttonState = NONE;
        }
        else if (systemfaultTimer > 60) //30 seconds
        {
            if (eeprom_read(ERROR_LOCATION_EEPROM,
            ERROR_BLOCK_EEPROM) != SYSTEM_FAULT_ERROR_CODE)
            {
                eeprom_write(SYSTEM_FAULT_ERROR_CODE, ERROR_LOCATION_EEPROM,
                ERROR_BLOCK_EEPROM);
            }
            systemfaultTimer = 0;

        }
        else if (buttonState == BOTH_BTNS && Ecu2Buttons == BOTH_BTNS
                && systemfaultTimer >= 60)
        {
            Ecu2Buttons = NONE;
            buttonState = NONE;
            if (eeprom_read(ERROR_LOCATION_EEPROM,
            ERROR_BLOCK_EEPROM) != NO_ERROR_CODE)
            {
                eeprom_write(NO_ERROR_CODE, ERROR_LOCATION_EEPROM,
                ERROR_BLOCK_EEPROM);
            }
            systemState = NO_ERROR_CODE;
            SystemfaultErrorCounter = 0;

        }
        if (SystemfaultErrorCounter >= 3)
        {
            if (eeprom_read(ERROR_LOCATION_EEPROM,
            ERROR_BLOCK_EEPROM) != SYSTEM_FAULT_ERROR_CODE)
            {
                eeprom_write(SYSTEM_FAULT_ERROR_CODE, ERROR_LOCATION_EEPROM,
                ERROR_BLOCK_EEPROM);
            }
            SystemfaultErrorCounter = 0;
        }
        break;

    case OVERHEATING_ERROR_CODE:
        if (overheatTimer >= 20 && buttonState == BTN1 && overheatTimer < 60
                && Ecu2Buttons == BTN1)
        {
            OverheatErrorCounter +=1;
            overheatTimer = 0;
            systemState = NO_ERROR_CODE;
            Ecu2Buttons = NONE;
            buttonState = NONE;
        }
        else if (overheatTimer >= 60)
        {
            if (eeprom_read(ERROR_LOCATION_EEPROM,
            ERROR_BLOCK_EEPROM) != OVERHEATING_ERROR_CODE)
            {
                eeprom_write(OVERHEATING_ERROR_CODE, ERROR_LOCATION_EEPROM,
                ERROR_BLOCK_EEPROM);
            }
            overheatTimer = 0;
        }
        else if (buttonState == BTN1 && Ecu2Buttons == BTN1
                && overheatTimer > 60)
        {
            Ecu2Buttons = NONE;
            buttonState = NONE;
            OverheatErrorCounter = 0;

            if (eeprom_read(ERROR_LOCATION_EEPROM,
            ERROR_BLOCK_EEPROM) != NO_ERROR_CODE)
            {
                eeprom_write(NO_ERROR_CODE, ERROR_LOCATION_EEPROM,
                ERROR_BLOCK_EEPROM);
            }
            systemState = NO_ERROR_CODE;

        }
        if (OverheatErrorCounter > 2)
        {
            if (eeprom_read(ERROR_LOCATION_EEPROM,
            ERROR_BLOCK_EEPROM) != OVERHEATING_ERROR_CODE)
            {
                eeprom_write(OVERHEATING_ERROR_CODE, ERROR_LOCATION_EEPROM,
                ERROR_BLOCK_EEPROM);
            }
//            OverheatErrorCounter = 0;
        }
        break;

    case COMMUNICATION_ERROR_CODE:
        if (buttonState == BTN2 && keepaliveTimer >= 20 && keepaliveTimer <= 60)

        {
            buttonState = NONE;
            systemState = NO_ERROR_CODE;
            KeepAliveErrorCounter += 1;
            keepaliveTimer = 0;
            KeepAliveError = 0;
        }
        else if (buttonState == BTN2 && keepaliveTimer > 60)
        {
            buttonState = NONE;
            systemState = NO_ERROR_CODE;
            KeepAliveErrorCounter += 1;
            keepaliveTimer = 0;
            KeepAliveError = 0;

        }
       if(keepaliveTimer > 60)
       {
           if (eeprom_read(ERROR_LOCATION_EEPROM,
                    ERROR_BLOCK_EEPROM) != COMMUNICATION_ERROR_CODE)
                    {
                        eeprom_write(COMMUNICATION_ERROR_CODE, ERROR_LOCATION_EEPROM,
                        ERROR_BLOCK_EEPROM);
                    }

       }
        if (KeepAliveErrorCounter > 2)
        {
//            KeepAliveErrorCounter = 0;
            if (eeprom_read(ERROR_LOCATION_EEPROM,
            ERROR_BLOCK_EEPROM) != COMMUNICATION_ERROR_CODE)
            {
                eeprom_write(COMMUNICATION_ERROR_CODE, ERROR_LOCATION_EEPROM,
                ERROR_BLOCK_EEPROM);
            }
        }
        break;

    }
}

void GetButtons(void)
{
    switch (systemState)
    {
    case NO_ERROR_CODE:
        buttonState = 0;
        break;

    case OVERHEATING_ERROR_CODE:
        if (buttonState == BTN1)
            break;
        else
        {
            buttonState = get_input();
            break;
        }

    case SYSTEM_FAULT_ERROR_CODE:
        if (buttonState == BOTH_BTNS)
            break;
        else
        {
            buttonState = get_input();
            break;
        }

    case COMMUNICATION_ERROR_CODE:
        if (buttonState == BTN2)
            break;
        else
        {
            buttonState = get_input();
            break;
        }
    }

}

void PCcommunication(void)
{
    switch (systemState)
    {
    case NO_ERROR_CODE:
        if (temperature == 101)
            sendPC("Communication Problem Attempting to Connect in 5 Seconds\n");
        else
        {
            sendPC("Last Temperature = ");
            intToAsciiAndSendPC(temperature);
            temperature = 101;
            sendPC(" C and System State is ");
            sendPC("Connected!!\n");
        }
        break;
    case OVERHEATING_ERROR_CODE:
        if (temperature == 101)
            sendPC("Communication Problem Attempting to Connect in 5 Seconds\n");
        else
        {
            sendPC("Last Temperature = ");
            intToAsciiAndSendPC(temperature);
            sendPC(" C and System State is ");
            sendPC("OverHeated!!\n");
        }
        break;
    case SYSTEM_FAULT_ERROR_CODE:
        sendPC("SystemFault!!\n");
        break;
    case COMMUNICATION_ERROR_CODE:
        sendPC("Communication Error!!\n");
        break;
    }

}

void LEDTASK(void)
{
    switch (systemState)
    {
    case NO_ERROR_CODE:
        clear_leds();
        ledState = Led_State_OFF;
        Ecu2Buttons = NONE;
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
        keepaliveTimer += 1;
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

void CANIntHandler(void)
{
    uint32_t ui32Status;
    ui32Status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);

    if (ui32Status == CAN_INT_INTID_STATUS)
    {
        ui32Status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
        g_bErrFlag = 1;
    }

    else if (ui32Status == 1)   //msg object 1
    {
        CANIntClear(CAN0_BASE, 1);
        g_ui32MsgCount++;
        sCANMessage.pui8MsgData = pui8MsgData;
        CANMessageGet(CAN0_BASE, 1, &sCANMessage, 0);
        temperature = *sCANMessage.pui8MsgData;
        TempRxFlag = 1;
        g_bErrFlag = 0;
    }
    else if (ui32Status == 2)
    {
        CANIntClear(CAN0_BASE, 2);
        KeepAliveSent += 1;
        g_bErrFlag = 0;
    }
    else if (ui32Status == 3)   //msg object 3
    {
        CANIntClear(CAN0_BASE, 3);
        g_ui32MsgCount++;
        VoltageMsg.pui8MsgData = pui8VoltageData;
        CANMessageGet(CAN0_BASE, 3, &VoltageMsg, 0);
        voltagevalue = *VoltageMsg.pui8MsgData * 3.3 / 255.0;
        g_bErrFlag = 0;
    }
    else if (ui32Status == 4)   //msg object 4
    {
        CANIntClear(CAN0_BASE, 4);
        g_ui32MsgCount++;
        ButtonMsg.pui8MsgData = pui8ButtonData;
        CANMessageGet(CAN0_BASE, 4, &ButtonMsg, 0);
        Ecu2Buttons = *ButtonMsg.pui8MsgData;
        g_bErrFlag = 0;
    }
    else
    {

    }
}

