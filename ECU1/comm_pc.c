/******************************************************************************
 *
 * Module: Communication with PC
 *
 * File Name: comm_pc.c
 *
 * Description: Source file for the TM4C123GH6PM communication with PC driver
 *
 * Author: Marwan Mobarak
 *
 *******************************************************************************/
#include "comm_pc.h"

void reverse(char str[], int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end)
    {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void intToAsciiAndSendPC(uint32 number)
{
    char buffer[12]; // Adjust the size based on the expected maximum length of the integer representation

    // Handle negative numbers
    int isNegative = 0;
    if (number < 0)
    {
        isNegative = 1;
        number = -number;
    }

    int i = 0;

    // Convert the number to a string
    do
    {
        buffer[i++] = '0' + (number % 10);
        number /= 10;
    }
    while (number != 0 && i < sizeof(buffer) - 1);

    if (isNegative)
    {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    // Reverse the string
    reverse(buffer, i);

    // Send the string to UART0_SendString function
    sendPC((uint8*)buffer);
}

void floatToAsciiAndSendPC(float floatValue)
{
    // Adjust the buffer size based on the expected maximum length of the float representation
    char buffer[20];
    int isNegative = 0;

    // Handle negative floats
    if (floatValue < 0)
    {
        isNegative = 1;
        floatValue = -floatValue;
    }

    // Extract integer part
    uint32 integerPart = (uint32)floatValue;

    // Convert integer part to string
    int i = 0;
    do
    {
        buffer[i++] = '0' + (integerPart % 10);
        integerPart /= 10;
    } while (integerPart != 0);

    // Reverse the integer part string
    reverse(buffer, i);

    // Add decimal point
    buffer[i++] = '.';

    // Extract decimal part with a fixed number of digits (adjust as needed)
    uint32 decimalPart = (uint32)((floatValue - (float)integerPart) * 10000);

    // Convert decimal part to string
    do
    {
        buffer[i++] = '0' + (decimalPart % 10);
        decimalPart /= 10;
    } while (decimalPart != 0 && i < sizeof(buffer) - 1);

    if (isNegative)
    {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    // Send the string to UART0_SendString function
    sendPC((uint8 *)buffer);
}

void sendPC(uint8 *pData)
{
    UART0_SendString(pData);

}
