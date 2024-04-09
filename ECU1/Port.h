/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Marwan Mobarak
 ******************************************************************************/

#ifndef PORT_H
#define PORT_H

//TEAM ID
#define PORT_VENDOR_ID    (7U)

/* Port Module Id */
#define PORT_MODULE_ID    (124U)

/* PORT Instance Id */
#define PORT_INSTANCE_ID  (0U)

/*
 * Module Version 1.0.0
 */
#define PORT_SW_MAJOR_VERSION           (1U)
#define PORT_SW_MINOR_VERSION           (0U)
#define PORT_SW_PATCH_VERSION           (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_AR_RELEASE_MAJOR_VERSION   (4U)
#define PORT_AR_RELEASE_MINOR_VERSION   (0U)
#define PORT_AR_RELEASE_PATCH_VERSION   (3U)

/*
 * Macros for Port Status
 */
#define PORT_INITIALIZED                (1U)
#define PORT_NOT_INITIALIZED            (0U)

/* Standard AUTOSAR types */
#include "Std_Types.h"

/* AUTOSAR checking between Std Types and Port Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version"
#endif

/* Port Pre-Compile Configuration Header file */
#include "Port_Cfg.h"

/* AUTOSAR Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
#error "The AR version of Port_Cfg.h does not match the expected version"
#endif

/* Software Version checking between Dio_Cfg.h and Dio.h files */
#if ((PORT_CFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_CFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_CFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
#error "The SW version of Port_Cfg.h does not match the expected version"
#endif

/* Non AUTOSAR files */
#include "Common_Macros.h"
#include "Port_Reg.h"
#include "tm4c123gh6pm.h"
/******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/
/* Service ID for Port Init */
#define Port_Init_SID                   (uint8)0x00

/* Service ID for Port Set Pin Direction */
#define Port_SetPinDirection_SID        (uint8)0x01

/* Service ID for Port Refresh Port Direction */
#define Port_RefreshPortDirection_SID   (uint8)0x02

/* Service ID for Port Get Version Info */
#define Port_GetVersionInfo_SID         (uint8)0x03

/* Service ID for Port Set Pin Mode */
#define Port_SetPinMode_SID             (uint8)0x04

/*******************************************************************************
 *                      DET Error Codes                                        *
 *******************************************************************************/
/* DET code to report Invalid Port Pin ID requested */
#define PORT_E_PARAM_PIN (uint8)0x0A

/* DET code to report Port Pin not configured as changeable */
#define PORT_E_DIRECTION_UNCHANGEABLE (uint8)0x0B

/* DET code to report API Port_Init service */
#define PORT_E_PARAM_CONFIG (uint8)0x0C

/* DET code to report API Port_SetPinMode service called when mode is unchangeable. */
#define PORT_E_PARAM_INVALID_MODE (uint8)0x0D
#define PORT_E_MODE_UNCHANGEABLE (uint8)0x0E

/* DET code to report API service called without module initialization */
#define PORT_E_UNINIT (uint8)0x0F

/* DET code to report APIs called with a Null Pointer */
#define PORT_E_PARAM_POINTER (uint8)0x10

/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/
/* Type definition for Port_PortType used by the Port APIs */
typedef uint8 Port_PortType;

/* Type definition for Port_PinType used by the Port APIs */
typedef uint8 Port_PinType;

/* Type definition for Port_LevelType used by the Port APIs */
typedef uint8 Port_LevelType;

/* Type definition for Port_PinModeChangeable used by the Port APIs */
typedef uint8 Port_PinModeChangeable;

/* Type definition for Port_PinDirChangeable used by the Port APIs */
typedef uint8 Port_PinDirChangeable;

/* Description: Enum to hold Mode Type */
typedef enum
{
    ADC, ANALOG_COMPARTOR, CAN, DIO, I2C, PWM, QEI, SSI, UART,
} Port_PinModeType;

/* Description: Enum to hold PIN direction */
typedef enum
{
    PORT_PIN_IN, PORT_PIN_OUT
} Port_PinDirectionType;

/* Description: Enum to hold internal resistor type for PIN */
typedef enum
{
    OFF, PULL_UP, PULL_DOWN
} Port_InternalResistor;

typedef enum
{
    DIGITAL, ANALOG
} Port_PinDigitalType;
/* Description: Structure to hold Configurations for each PIN */
typedef struct
{
    Port_PortType port_num;
    uint8 pin_num;
    Port_PinModeType mode;
    Port_PinDigitalType analog;
    Port_PinDirectionType direction;
    Port_InternalResistor resistor;
    Port_LevelType initial_value;
    Port_PinDirChangeable dir_changeable;
    Port_PinModeChangeable mode_changeable;
} Port_ConfigType;

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/************************************************************************************
 * Service Name: Port_Init
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): ConfigPtr - Pointer to configuration set.
 * Parameters (input): None
 * Parameters (out): None
 * Return value: None
 * Description: Service to Initializes the Port Driver module.
 ************************************************************************************/
void Port_Init(Port_ConfigType *ConfigPtr);

/************************************************************************************
 * Service Name: Port_SetPinDirection
 * Service ID[hex]: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): pin - Port Pin ID number
 *                  Direction - Port Pin Direction
 * Parameters (input): None
 * Parameters (out): None
 * Return value: None
 * Description: Service to Sets the port pin direction.
 ************************************************************************************/
#if (Port_Set_Pin_Direction_API == STD_ON)
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction);
#endif
/************************************************************************************
 * Service Name: Port_RefreshPortDirection
 * Service ID[hex]: 0x02
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): None
 * Parameters (input): None
 * Parameters (out): None
 * Return value: None
 * Description: Service to Refresh the port direction of all initialized ports.
 ************************************************************************************/
void Port_RefreshPortDirection(void);

/************************************************************************************
 * Service Name: Port_SetPinMode
 * Service ID[hex]: 0x04
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): pin - Port Pin ID number
 *                  Mode - New Port Pin mode to be set on port pin.
 * Parameters (input): None
 * Parameters (out): None
 * Return value: None
 * Description: Service to Set the port pin mode.
 ************************************************************************************/
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode);

/************************************************************************************
 * Service Name: Port_GetVersionInfo
 * Service ID[hex]: 0x03
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): None
 * Parameters (input): None
 * Parameters (out): versioninfo - Pointer to where to store the version information of this module.
 * Return value: None
 * Description: Service to Return the version information of this module.
 ************************************************************************************/
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo(Std_VersionInfoType *versioninfo);
#endif

STATIC boolean Port_ModeErrorCheck(const Port_ConfigType *ConfigPtr);
STATIC void Port_SetPortClock(const Port_ConfigType *ConfigPtr);

/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/

/* Extern PB structures to be used by Port and other modules */
extern const Port_ConfigType Port_Configuration[];

#endif /* PORT_H */
