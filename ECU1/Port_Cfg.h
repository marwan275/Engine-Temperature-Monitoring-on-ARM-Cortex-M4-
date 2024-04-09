 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_Cfg.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author:  Marwan Mobarak
 ******************************************************************************/


#ifndef PORT_CFG_H_
#define PORT_CFG_H_


/*
 * Module Version 1.0.0
 */
#define PORT_CFG_SW_MAJOR_VERSION              (1U)
#define PORT_CFG_SW_MINOR_VERSION              (0U)
#define PORT_CFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_CFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_CFG_AR_RELEASE_PATCH_VERSION     (3U)

/* Pre-compile option for Development Error Detect */
#define PORT_DEV_ERROR_DETECT                (STD_ON)

/* Pre-compile option for Version Info API */
#define PORT_VERSION_INFO_API                (STD_OFF)

#define Port_Set_Pin_Direction_API           (STD_ON)
/* Number of the configured Dio Channels */
#define PORT_CONFIGURED_PINS              (8U)


/* PORT Configured Port ID's  */
#define PortConf_RED_LED_PORT_NUM                PORTF /* PORTF */
#define PortConf_BLUE_LED_PORT_NUM                PORTF /* PORTF */
#define PortConf_GREEN_LED_PORT_NUM                PORTF /* PORTF */

#define PortConf_SW1_PORT_NUM                 PORTF /* PORTF */
#define PortConf_SW2_PORT_NUM                 PORTF /* PORTF */

/* PORT Configured PIN ID's */
#define PortConf_RED_LED_PIN_NUM             PIN_1 /* Pin 1 in PORTF */
#define PortConf_BLUE_LED_PIN_NUM             PIN_2 /* Pin 2 in PORTF */
#define PortConf_GREEN_LED_PIN_NUM             PIN_3 /* Pin 3 in PORTF */

#define PortConf_SW1_PIN_NUM              PIN_0 /* Pin 0 in PORTF */
#define PortConf_SW2_PIN_NUM              PIN_4 /* Pin 4 in PORTF */

#endif /* PORT_CFG_H_ */
