/******************************************************************************
 *
 * Module: UART0
 *
 * File Name: uart0.c
 *
 * Description: Source file for the TM4C123GH6PM UART0 driver
 *
 * Author: Marwan Mobarak
 *
 *******************************************************************************/

#include <UART.h>
/*******************************************************************************
 *                         Private Functions Definitions                       *
 *******************************************************************************/
STATIC void UART_SetClock(UART_ModuleNum UART_Num)
{
    switch (UART_Num)
    {
    case UART0:
        SYSCTL_RCGCUART_R |= 0x01; /* Enable clock for UART0 */
        while (!(SYSCTL_PRUART_R & 0x01))
            ; /* Wait until UART0 clock is activated and it is ready for access*/
        UART0_CTL_R = 0; /* Disable UART0 at the beginning */
        break;
    case UART1:
        SYSCTL_RCGCUART_R |= (1 << 1); /* Enable clock for UART0 */
        while (!(SYSCTL_PRUART_R & (1 << 1)))
            ; /* Wait until UART0 clock is activated and it is ready for access*/
        UART1_CTL_R = 0; /* Disable UART1 at the beginning */
        break;
    case UART2:
        SYSCTL_RCGCUART_R |= (1 << 2); /* Enable clock for UART0 */
        while (!(SYSCTL_PRUART_R & (1 << 2)))
            ; /* Wait until UART0 clock is activated and it is ready for access*/
        UART2_CTL_R = 0; /* Disable UART1 at the beginning */
        break;
    case UART3:
        SYSCTL_RCGCUART_R |= (1 << 3); /* Enable clock for UART0 */
        while (!(SYSCTL_PRUART_R & (1 << 3)))
            ; /* Wait until UART0 clock is activated and it is ready for access*/
        UART3_CTL_R = 0; /* Disable UART1 at the beginning */
        break;
    case UART4:
        SYSCTL_RCGCUART_R |= (1 << 4); /* Enable clock for UART0 */
        while (!(SYSCTL_PRUART_R & (1 << 4)))
            ; /* Wait until UART0 clock is activated and it is ready for access*/
        UART4_CTL_R = 0; /* Disable UART1 at the beginning */
        break;
    case UART5:
        SYSCTL_RCGCUART_R |= (1 << 5); /* Enable clock for UART0 */
        while (!(SYSCTL_PRUART_R & (1 << 5)))
            ; /* Wait until UART0 clock is activated and it is ready for access*/
        UART5_CTL_R = 0; /* Disable UART1 at the beginning */
        break;
    case UART6:
        SYSCTL_RCGCUART_R |= (1 << 6); /* Enable clock for UART0 */
        while (!(SYSCTL_PRUART_R & (1 << 6)))
            ; /* Wait until UART0 clock is activated and it is ready for access*/
        UART6_CTL_R = 0; /* Disable UART1 at the beginning */
        break;
    case UART7:
        SYSCTL_RCGCUART_R |= (1 << 7); /* Enable clock for UART0 */
        while (!(SYSCTL_PRUART_R & (1 << 7)))
            ; /* Wait until UART0 clock is activated and it is ready for access*/
        UART7_CTL_R = 0; /* Disable UART1 at the beginning */
        break;
    }

}

/*******************************************************************************
 *                         Public Functions Definitions                        *
 *******************************************************************************/

void UART_Init(UART_Config *ConfigPtr)
{

    uint8 i = 0;
    for (i = 0; i < UART_CONFIGURED_MODULES; i++)
    {
        UART_SetClock(ConfigPtr[i].UART_num);
        volatile uint32 *UART_ClockReg = NULL_PTR;
        volatile uint32 *UART_IBRDReg = NULL_PTR;
        volatile uint32 *UART_FBRDReg = NULL_PTR;
        volatile uint32 *UART_LCRH_R = NULL_PTR;
        volatile uint32 *UART_CTL_R = NULL_PTR;

        switch (ConfigPtr[i].UART_num)
        {
        case UART0:
            UART_ClockReg = (volatile uint32*) (UART0_BASE_ADDRESS
                    + UART_CC_OFFSET);
            UART_IBRDReg = (volatile uint32*) (UART0_BASE_ADDRESS
                    + UART_IBRD_OFFSET);
            UART_FBRDReg = (volatile uint32*) (UART0_BASE_ADDRESS
                    + UART_FBRD_OFFSET);
            UART_LCRH_R = (volatile uint32*) (UART0_BASE_ADDRESS
                    + UART_LCRH_OFFSET);
            UART_CTL_R = (volatile uint32*) (UART0_BASE_ADDRESS
                    + UART_CTL_OFFSET);
            break;
        case UART1:
            UART_ClockReg = (volatile uint32*) (UART1_BASE_ADDRESS
                    + UART_CC_OFFSET);
            UART_IBRDReg = (volatile uint32*) (UART1_BASE_ADDRESS
                    + UART_IBRD_OFFSET);
            UART_FBRDReg = (volatile uint32*) (UART1_BASE_ADDRESS
                    + UART_FBRD_OFFSET);
            UART_LCRH_R = (volatile uint32*) (UART1_BASE_ADDRESS
                    + UART_LCRH_OFFSET);
            UART_CTL_R = (volatile uint32*) (UART1_BASE_ADDRESS
                    + UART_CTL_OFFSET);
            break;
        case UART2:
            UART_ClockReg = (volatile uint32*) (UART2_BASE_ADDRESS
                    + UART_CC_OFFSET);
            UART_IBRDReg = (volatile uint32*) (UART2_BASE_ADDRESS
                    + UART_IBRD_OFFSET);
            UART_FBRDReg = (volatile uint32*) (UART2_BASE_ADDRESS
                    + UART_FBRD_OFFSET);
            UART_LCRH_R = (volatile uint32*) (UART2_BASE_ADDRESS
                    + UART_LCRH_OFFSET);
            UART_CTL_R = (volatile uint32*) (UART2_BASE_ADDRESS
                    + UART_CTL_OFFSET);
            break;
        case UART3:
            UART_ClockReg = (volatile uint32*) (UART3_BASE_ADDRESS
                    + UART_CC_OFFSET);
            UART_IBRDReg = (volatile uint32*) (UART3_BASE_ADDRESS
                    + UART_IBRD_OFFSET);
            UART_FBRDReg = (volatile uint32*) (UART3_BASE_ADDRESS
                    + UART_FBRD_OFFSET);
            UART_LCRH_R = (volatile uint32*) (UART3_BASE_ADDRESS
                    + UART_LCRH_OFFSET);
            UART_CTL_R = (volatile uint32*) (UART3_BASE_ADDRESS
                    + UART_CTL_OFFSET);
            break;
        case UART4:
            UART_ClockReg = (volatile uint32*) (UART4_BASE_ADDRESS
                    + UART_CC_OFFSET);
            UART_IBRDReg = (volatile uint32*) (UART4_BASE_ADDRESS
                    + UART_IBRD_OFFSET);
            UART_FBRDReg = (volatile uint32*) (UART4_BASE_ADDRESS
                    + UART_FBRD_OFFSET);
            UART_LCRH_R = (volatile uint32*) (UART4_BASE_ADDRESS
                    + UART_LCRH_OFFSET);
            UART_CTL_R = (volatile uint32*) (UART4_BASE_ADDRESS
                    + UART_CTL_OFFSET);
            break;
        case UART5:
            UART_ClockReg = (volatile uint32*) (UART5_BASE_ADDRESS
                    + UART_CC_OFFSET);
            UART_IBRDReg = (volatile uint32*) (UART5_BASE_ADDRESS
                    + UART_IBRD_OFFSET);
            UART_FBRDReg = (volatile uint32*) (UART5_BASE_ADDRESS
                    + UART_FBRD_OFFSET);
            UART_LCRH_R = (volatile uint32*) (UART5_BASE_ADDRESS
                    + UART_LCRH_OFFSET);
            UART_CTL_R = (volatile uint32*) (UART5_BASE_ADDRESS
                    + UART_CTL_OFFSET);
            break;
        case UART6:
            UART_ClockReg = (volatile uint32*) (UART6_BASE_ADDRESS
                    + UART_CC_OFFSET);
            UART_IBRDReg = (volatile uint32*) (UART6_BASE_ADDRESS
                    + UART_IBRD_OFFSET);
            UART_FBRDReg = (volatile uint32*) (UART6_BASE_ADDRESS
                    + UART_FBRD_OFFSET);
            UART_LCRH_R = (volatile uint32*) (UART6_BASE_ADDRESS
                    + UART_LCRH_OFFSET);
            UART_CTL_R = (volatile uint32*) (UART6_BASE_ADDRESS
                    + UART_CTL_OFFSET);
            break;
        case UART7:
            UART_ClockReg = (volatile uint32*) (UART7_BASE_ADDRESS
                    + UART_CC_OFFSET);
            UART_IBRDReg = (volatile uint32*) (UART7_BASE_ADDRESS
                    + UART_IBRD_OFFSET);
            UART_FBRDReg = (volatile uint32*) (UART7_BASE_ADDRESS
                    + UART_FBRD_OFFSET);
            UART_LCRH_R = (volatile uint32*) (UART7_BASE_ADDRESS
                    + UART_LCRH_OFFSET);
            UART_CTL_R = (volatile uint32*) (UART7_BASE_ADDRESS
                    + UART_CTL_OFFSET);
            break;
        }
        if (ConfigPtr[i].UART_num == SYSTEM_CLOCK)
        {
            *UART_ClockReg = 0; /* Use System Clock*/
        }
        else if (ConfigPtr[i].UART_num == PIOSC)
        {
            *UART_ClockReg = 0x05; /* Use System Clock*/
        }
        switch (ConfigPtr[i].BaudRate)
        {
        case RATE_9600:
            *UART_IBRDReg = 104;
            *UART_FBRDReg = 11;
            break;
        default:
            break;
        }

        /* UART Line Control Register Settings
         * BRK = 0 Normal Use
         * PEN = 0 Disable Parity
         * EPS = 0 No affect as the parity is disabled
         * STP2 = 0 1-stop bit at end of the frame
         * FEN = 0 FIFOs are disabled
         * WLEN = 0x3 8-bits data frame
         * SPS = 0 no stick parity
         */
        switch (ConfigPtr[i].length)
        {
        case DATA_8BITS:
            *UART_LCRH_R = (DATA_8BITS << UART_LCRH_WLEN_BITS_POS);
            break;
        case DATA_7BITS:
            *UART_LCRH_R = (DATA_7BITS << UART_LCRH_WLEN_BITS_POS);
            break;
        case DATA_6BITS:
            *UART_LCRH_R = (DATA_6BITS << UART_LCRH_WLEN_BITS_POS);
            break;
        case DATA_5BITS:
            *UART_LCRH_R = (DATA_5BITS << UART_LCRH_WLEN_BITS_POS);
            break;
        }
        /* UART Control Register Settings
         * RXE = 1 Enable UART Receive
         * TXE = 1 Enable UART Transmit
         * HSE = 0 The UART is clocked using the system clock divided by 16
         * UARTEN = 1 Enable UART
         */
        switch (ConfigPtr[i].pins_mode)
        {
        case TX:
            *UART_CTL_R = UART_CTL_UARTEN_MASK | UART_CTL_TXE_MASK;
            break;
        case RX:
            *UART_CTL_R = UART_CTL_UARTEN_MASK | UART_CTL_RXE_MASK;
            break;
        case TX_RX:
            *UART_CTL_R = UART_CTL_UARTEN_MASK | UART_CTL_TXE_MASK
                    | UART_CTL_RXE_MASK;
            break;
        }
    }
}

void UART0_SendByte(uint8 data)
{
    while (!(UART0_FR_R & UART_FR_TXFE_MASK))
        ; /* Wait until the transmit FIFO is empty */
    UART0_DR_R = data; /* Send the byte */
}

uint8 UART0_ReceiveByte(void)
{
    while (UART0_FR_R & UART_FR_RXFE_MASK)
        ; /* Wait until the receive FIFO is not empty */
    return UART0_DR_R; /* Read the byte */
}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART0_SendString(const uint8 *pData)
{
    uint32 uCounter = 0;
    /* Transmit the whole string */
    while (pData[uCounter] != '\0')
    {
        UART0_SendByte(pData[uCounter]); /* Send the byte */
        uCounter++; /* increment the counter to the next byte */
    }
}



/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART0_ReceiveString(uint8 *pData)
{
    uint32 uCounter = 0;

    pData[uCounter] = UART0_ReceiveByte(); /* Receive the first byte */

    /* Receive the whole string until the '#' */
    while (pData[uCounter] != '#')
    {
        uCounter++;
        pData[uCounter] = UART0_ReceiveByte();
    }
    /* After receiving the whole string plus the '#', replace the '#' with '\0' */
    pData[uCounter] = '\0';
}

/*
 * Description :
 * Send the required number of bytes through UART to the other UART device.
 */
void UART0_SendData(const uint8 *pData, uint32 uSize)
{
    /* Transmit the number of bytes requested on the UART port */
    while (uSize--)
    {

        UART0_SendByte(*pData); /* Send the byte */
        pData++; /* increment pointer to the next byte */
    }
}

/*
 * Description :
 * receive the required number of bytes through UART from the other UART device.
 */
void UART0_ReceiveData(uint8 *pData, uint32 uSize)
{
    /* Receive the number of bytes requested on the UART port */
    while (uSize--)
    {
        *pData = UART0_ReceiveByte(); /* receive a byte from the UART */
        pData++; /* increment pointer to receive the next byte in the next location */
    }
}
