/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_PBcfg.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author:  Marwan Mobarak
 ******************************************************************************/
#include "Port.h"
#include "Port_Cfg.h"
#include "tm4c123gh6pm.h"

const Port_ConfigType Port_Configuration[PORT_CONFIGURED_PINS] = {
        { .analog = DIGITAL, .dir_changeable = STD_OFF,
          .direction = PORT_PIN_IN, .initial_value = STD_OFF, .mode = DIO,
          .mode_changeable = STD_OFF, .pin_num = PortConf_SW1_PIN_NUM,
          .port_num =
          PortConf_SW1_PORT_NUM,
          .resistor = PULL_UP },
        { .analog = DIGITAL, .dir_changeable = STD_OFF,
          .direction = PORT_PIN_IN, .initial_value = STD_OFF, .mode = DIO,
          .mode_changeable = STD_OFF, .pin_num = PortConf_SW2_PIN_NUM,
          .port_num =
          PortConf_SW2_PORT_NUM,
          .resistor = PULL_UP },
        { .analog = DIGITAL, .dir_changeable = STD_OFF, .direction =
                  PORT_PIN_OUT,
          .initial_value = STD_OFF, .mode = DIO, .mode_changeable = STD_OFF,
          .pin_num = PortConf_RED_LED_PIN_NUM, .port_num =
          PortConf_RED_LED_PORT_NUM,
          .resistor = OFF },
        { .analog = DIGITAL, .dir_changeable = STD_OFF, .direction =
                  PORT_PIN_OUT,
          .initial_value = STD_OFF, .mode = DIO, .mode_changeable = STD_OFF,
          .pin_num = PortConf_BLUE_LED_PIN_NUM, .port_num =
          PortConf_RED_LED_PORT_NUM,
          .resistor = OFF },
        { .analog = DIGITAL, .dir_changeable = STD_OFF, .direction =
                  PORT_PIN_OUT,
          .initial_value = STD_OFF, .mode = DIO, .mode_changeable = STD_OFF,
          .pin_num = PortConf_GREEN_LED_PIN_NUM, .port_num =
          PortConf_RED_LED_PORT_NUM,
          .resistor = OFF },
        { .analog = DIGITAL, .dir_changeable = STD_OFF,
          .direction = PORT_PIN_IN, .initial_value = STD_OFF, .mode = UART,
          .mode_changeable = STD_OFF, .pin_num = PIN_0, .port_num = PORTA,
          .resistor = OFF },
        { .analog = DIGITAL, .dir_changeable = STD_OFF, .direction =
                  PORT_PIN_OUT,
          .initial_value = STD_OFF, .mode = UART, .mode_changeable = STD_OFF,
          .pin_num = PIN_1, .port_num = PORTA, .resistor = OFF },
        { .analog = ANALOG, .dir_changeable = STD_OFF, .direction = PORT_PIN_IN,
          .initial_value = STD_OFF, .mode = ADC, .mode_changeable = STD_OFF,
          .pin_num = PIN_3, .port_num = PORTE, .resistor = OFF }, };
