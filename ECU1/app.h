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
void MainTask(void);
void PCcommunication(void);
void KeepAlive(void);
void LEDTASK(void);
void GetButtons(void);
void ChangeState(void);
#endif /* APP_H_ */
