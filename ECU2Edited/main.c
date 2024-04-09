/******************************************************************************
 *
 * Module: Main
 *
 * File Name: Main.c
 *
 * Description: Main file for the project application
 *
 * Author: Marwan Mobarak
 ******************************************************************************/
#include "Mcu.h"
#include "scheduler.h"
#include "app.h"

#include "adc.h"
#include "adc_cfg.h"

int main(void)
{
    Mcu_Init();
    App_init();
    OS_start();
    return 0;
}

