/*
 * comm_pc.h
 *
 *  Created on: Jan 8, 2024
 *      Author: Marwan Mobarak
 */

#ifndef COMM_PC_H_
#define COMM_PC_H_
#include "UART.h"

void intToAsciiAndSendPC(uint32 number);
void floatToAsciiAndSendPC(float floatValue);
void reverse(char str[], int length);
void sendPC(uint8 *pData);

#endif /* COMM_PC_H_ */
