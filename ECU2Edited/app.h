 /******************************************************************************
 *
 * Module: application
 *
 * File Name: App.h
 *
 * Description: Header file for Application Tasks.
 *
 * Author: Marwan Mobarak
 ******************************************************************************/

#ifndef APP_H_
#define APP_H_
#include "Can.h"
#include "std_types.h"
#include "scheduler.h"
#include "eeprom.h"
#include "Gpt.h"
#include "led.h"
#include "comm_pc.h"
#include "temp_sen.h"
#include "button.h"
#include "appErrors.h"


void App_init(void);
void test_task_1(void);
void test_task_2(void);
void test_task_3(void);
void LEDTASK(void);
void GetButtons(void);

#endif /* APP_H_ */
