/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author:  Marwan Mobarak
 ******************************************************************************/

#include "Port.h"
#if (PORT_DEV_ERROR_DETECT == STD_ON)

#include "Det.h"
/* AUTOSAR Version checking between Det and Port Modules */
#if ((DET_AR_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 || (DET_AR_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 || (DET_AR_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
#error "The AR version of Det.h does not match the expected version"
#endif

#endif

STATIC uint8 Port_Status = PORT_NOT_INITIALIZED;

void Port_Init(Port_ConfigType *ConfigPtr)
{
    uint8 i;
    for (i = 0; i < PORT_CONFIGURED_PINS; i++)
    {
        Port_ConfigType *selected = &ConfigPtr[i];

        boolean mode_error = FALSE;
        mode_error = Port_ModeErrorCheck(&ConfigPtr[i]);
        if (mode_error == TRUE)
        {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, Port_Init_SID,
            PORT_E_PARAM_CONFIG);
#endif
        }
        else if (mode_error == FALSE)
        {
            Port_SetPortClock(&ConfigPtr[i]);
            volatile uint32 *PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */

            switch (ConfigPtr[i].port_num)
            {
            case 0:
                PortGpio_Ptr = (volatile uint32*) GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                break;
            case 1:
                PortGpio_Ptr = (volatile uint32*) GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                break;
            case 2:
                PortGpio_Ptr = (volatile uint32*) GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                break;
            case 3:
                PortGpio_Ptr = (volatile uint32*) GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                break;
            case 4:
                PortGpio_Ptr = (volatile uint32*) GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                break;
            case 5:
                PortGpio_Ptr = (volatile uint32*) GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                break;
            }
            *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                    + PORT_LOCK_REG_OFFSET) = GPIO_LOCK_KEY; /* Unlock the GPIOCR register */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_COMMIT_REG_OFFSET),
                    ConfigPtr[i].pin_num); /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
            if (ConfigPtr[i].direction == PORT_PIN_OUT)
            {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET),
                        ConfigPtr[i].pin_num); /* Set the corresponding bit in the GPIODIR register to configure it as output pin */

                if (ConfigPtr[i].initial_value == STD_HIGH)
                {
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET),
                            ConfigPtr[i].pin_num); /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
                }
                else
                {
                    CLEAR_BIT(
                            *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET),
                            ConfigPtr[i].pin_num); /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
                }
            }
            else if (ConfigPtr[i].direction == PORT_PIN_IN)
            {
                CLEAR_BIT(
                        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET),
                        ConfigPtr[i].pin_num); /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */

                if (ConfigPtr[i].resistor == PULL_UP)
                {
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET),
                            ConfigPtr[i].pin_num); /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
                }
                else if (ConfigPtr[i].resistor == PULL_DOWN)
                {
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET),
                            ConfigPtr[i].pin_num); /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
                }
                else
                {
                    CLEAR_BIT(
                            *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET),
                            ConfigPtr[i].pin_num); /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                    CLEAR_BIT(
                            *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET),
                            ConfigPtr[i].pin_num); /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
                }
            }
            if (ConfigPtr[i].analog == ANALOG)
            {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),
                        ConfigPtr[i].pin_num); /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
                CLEAR_BIT(
                        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),
                        ConfigPtr[i].pin_num); /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
            }
            else if (ConfigPtr[i].analog == DIGITAL)
            {
                CLEAR_BIT(
                        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),
                        ConfigPtr[i].pin_num); /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),
                        ConfigPtr[i].pin_num); /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            }
            if (ConfigPtr[i].mode == DIO)
            {
                /* Setup the pin mode as GPIO */
                CLEAR_BIT(
                        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET),
                        ConfigPtr[i].pin_num); /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
                *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                        + PORT_CTL_REG_OFFSET) &= ~(0x0000000F
                        << (ConfigPtr[i].pin_num * 4)); /* Clear the PMCx bits for this pin */
            }
            else
            {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET),
                        ConfigPtr[i].pin_num); /* Set Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
                switch (ConfigPtr[i].mode)
                {
                case ADC:
                    *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                            + PORT_CTL_REG_OFFSET) &= ~(0x0000000F
                            << (ConfigPtr[i].pin_num * 4)); /* Clear the PMCx bits for this pin */
                    break;
                case ANALOG_COMPARTOR:
                    *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                            + PORT_CTL_REG_OFFSET) &= ~(0x0000000F
                            << (ConfigPtr[i].pin_num * 4)); /* Clear the PMCx bits for this pin */
                    break;
                case CAN:
                    if (ConfigPtr[i].port_num == PORTF)
                    {
                        *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                                + PORT_CTL_REG_OFFSET) |= (3
                                << (ConfigPtr[i].pin_num * 4));
                    }
                    else
                    {
                        *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                                + PORT_CTL_REG_OFFSET) |= (8
                                << (ConfigPtr[i].pin_num * 4));
                    }
                    break;
                case I2C:
                    *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                            + PORT_CTL_REG_OFFSET) |= (3
                            << (ConfigPtr[i].pin_num * 4));
                    break;
                case PWM:
                    if (ConfigPtr[i].port_num == PORTA
                            || ConfigPtr[i].port_num == PORTF)
                    {
                        *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                                + PORT_CTL_REG_OFFSET) |= (5
                                << (ConfigPtr[i].pin_num * 4));
                    }
                    else
                    {
                        *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                                + PORT_CTL_REG_OFFSET) |= (4
                                << (ConfigPtr[i].pin_num * 4));
                    }
                    break;
                case QEI:
                    *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                            + PORT_CTL_REG_OFFSET) |= (6
                            << (ConfigPtr[i].pin_num * 4));
                    break;
                case SSI:
                    *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                            + PORT_CTL_REG_OFFSET) |= (2
                            << (ConfigPtr[i].pin_num * 4));
                    break;
                case UART:
                    *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                            + PORT_CTL_REG_OFFSET) |= (1
                            << (ConfigPtr[i].pin_num * 4));
                    break;
                }
            }
            Port_Status = PORT_INITIALIZED;
        }
    }
}

#if (Port_Set_Pin_Direction_API == STD_ON)
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction)
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if (Port_Status == PORT_NOT_INITIALIZED)
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, Port_Init_SID,
        PORT_E_UNINIT);
#endif
    uint8 pin_num = 0;
    uint8 port_num = 0;
    volatile uint32 *PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
    switch (Pin)
    {
    case PA0:
        pin_num = PIN_0;
        port_num = PORTA;
        break;
    case PA1:
        pin_num = PIN_1;
        port_num = PORTA;
        break;
    case PA2:
        pin_num = PIN_2;
        port_num = PORTA;
        break;
    case PA3:
        pin_num = PIN_3;
        port_num = PORTA;
        break;
    case PA4:
        pin_num = PIN_4;
        port_num = PORTA;
        break;
    case PA5:
        pin_num = PIN_5;
        port_num = PORTA;
        break;
    case PA6:
        pin_num = PIN_6;
        port_num = PORTA;
        break;
    case PA7:
        pin_num = PIN_7;
        port_num = PORTA;
        break;
    case PB0:
        pin_num = PIN_0;
        port_num = PORTB;
        break;
    case PB1:
        pin_num = PIN_1;
        port_num = PORTB;
        break;
    case PB2:
        pin_num = PIN_2;
        port_num = PORTB;
        break;
    case PB3:
        pin_num = PIN_3;
        port_num = PORTB;
        break;
    case PB4:
        pin_num = PIN_4;
        port_num = PORTB;
        break;
    case PB5:
        pin_num = PIN_5;
        port_num = PORTB;
        break;
    case PB6:
        pin_num = PIN_6;
        port_num = PORTB;
        break;
    case PB7:
        pin_num = PIN_7;
        port_num = PORTB;
        break;
    case PC0:
        pin_num = PIN_0;
        port_num = PORTC;
        break;
    case PC1:
        pin_num = PIN_1;
        port_num = PORTC;
        break;
    case PC2:
        pin_num = PIN_2;
        port_num = PORTC;
        break;
    case PC3:
        pin_num = PIN_3;
        port_num = PORTC;
        break;
    case PC4:
        pin_num = PIN_4;
        port_num = PORTC;
        break;
    case PC5:
        pin_num = PIN_5;
        port_num = PORTC;
        break;
    case PC6:
        pin_num = PIN_6;
        port_num = PORTC;
        break;
    case PC7:
        pin_num = PIN_7;
        port_num = PORTC;
        break;
    case PD0:
        pin_num = PIN_0;
        port_num = PORTD;
        break;
    case PD1:
        pin_num = PIN_1;
        port_num = PORTD;
        break;
    case PD2:
        pin_num = PIN_2;
        port_num = PORTD;
        break;
    case PD3:
        pin_num = PIN_3;
        port_num = PORTD;
        break;
    case PD4:
        pin_num = PIN_4;
        port_num = PORTD;
        break;
    case PD5:
        pin_num = PIN_5;
        port_num = PORTD;
        break;
    case PD6:
        pin_num = PIN_6;
        port_num = PORTD;
        break;
    case PD7:
        pin_num = PIN_7;
        port_num = PORTD;
        break;
    case PE0:
        pin_num = PIN_0;
        port_num = PORTE;
        break;
    case PE1:
        pin_num = PIN_1;
        port_num = PORTE;
        break;
    case PE2:
        pin_num = PIN_2;
        port_num = PORTE;
        break;
    case PE3:
        pin_num = PIN_3;
        port_num = PORTE;
        break;
    case PE4:
        pin_num = PIN_4;
        port_num = PORTE;
        break;
    case PE5:
        pin_num = PIN_5;
        port_num = PORTE;
        break;
    case PE6:
        pin_num = PIN_6;
        port_num = PORTE;
        break;
    case PE7:
        pin_num = PIN_7;
        port_num = PORTE;
        break;
    case PF0:
        pin_num = PIN_0;
        port_num = PORTF;
        break;
    case PF1:
        pin_num = PIN_1;
        port_num = PORTF;
        break;
    case PF2:
        pin_num = PIN_2;
        port_num = PORTF;
        break;
    case PF3:
        pin_num = PIN_3;
        port_num = PORTF;
        break;
    case PF4:
        pin_num = PIN_4;
        port_num = PORTF;
        break;
    case PF5:
        pin_num = PIN_5;
        port_num = PORTF;
        break;
    case PF6:
        pin_num = PIN_6;
        port_num = PORTF;
        break;
    case PF7:
        pin_num = PIN_7;
        port_num = PORTF;
        break;
    default:
#if (PORT_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, Port_Init_SID,
        PORT_E_PARAM_PIN);
#endif
        break;
    }
    boolean changeable = TRUE;
    for (int i = 0; i < PORT_CONFIGURED_PINS; i++)
    {
        if (Port_Configuration[i].pin_num == pin_num
                && Port_Configuration[i].port_num == port_num)
        {
            changeable =
                    (Port_Configuration[i].dir_changeable == STD_ON) ?
                    TRUE :
                                                                       FALSE;
            break;  // Exit the loop once a matching pin is found
        }
    }
    if (changeable == FALSE)
    {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, Port_Init_SID,
        PORT_E_DIRECTION_UNCHANGEABLE);
#endif
    }
    switch (port_num)
    {
    case 0:
        PortGpio_Ptr = (volatile uint32*) GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
        break;
    case 1:
        PortGpio_Ptr = (volatile uint32*) GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
        break;
    case 2:
        PortGpio_Ptr = (volatile uint32*) GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
        break;
    case 3:
        PortGpio_Ptr = (volatile uint32*) GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
        break;
    case 4:
        PortGpio_Ptr = (volatile uint32*) GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
        break;
    case 5:
        PortGpio_Ptr = (volatile uint32*) GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
        break;
    }
    if (Direction == PORT_PIN_OUT)
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET),
                pin_num); /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
    else
        CLEAR_BIT(
                *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET),
                pin_num); /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */

}
#endif

void Port_RefreshPortDirection(void)
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if (Port_Status == PORT_NOT_INITIALIZED)
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, Port_Init_SID,
        PORT_E_UNINIT);
#endif
    uint8 i;
    for (i = 0; i < PORT_CONFIGURED_PINS; i++)
    {
        volatile uint32 *PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */

        switch (Port_Configuration[i].port_num)
        {
        case 0:
            PortGpio_Ptr = (volatile uint32*) GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
            break;
        case 1:
            PortGpio_Ptr = (volatile uint32*) GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
            break;
        case 2:
            PortGpio_Ptr = (volatile uint32*) GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
            break;
        case 3:
            PortGpio_Ptr = (volatile uint32*) GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
            break;
        case 4:
            PortGpio_Ptr = (volatile uint32*) GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
            break;
        case 5:
            PortGpio_Ptr = (volatile uint32*) GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
            break;
        }
        *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                + PORT_LOCK_REG_OFFSET) = GPIO_LOCK_KEY; /* Unlock the GPIOCR register */
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_COMMIT_REG_OFFSET),
                Port_Configuration[i].pin_num); /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
        if (Port_Configuration[i].direction == PORT_PIN_OUT)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET),
                    Port_Configuration[i].pin_num); /* Set the corresponding bit in the GPIODIR register to configure it as output pin */

        }
        else if (Port_Configuration[i].direction == PORT_PIN_IN)
        {
            CLEAR_BIT(
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET),
                    Port_Configuration[i].pin_num); /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */

            if (Port_Configuration[i].resistor == PULL_UP)
            {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET),
                        Port_Configuration[i].pin_num); /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
            }
            else if (Port_Configuration[i].resistor == PULL_DOWN)
            {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET),
                        Port_Configuration[i].pin_num); /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
            }
            else
            {
                CLEAR_BIT(
                        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET),
                        Port_Configuration[i].pin_num); /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                CLEAR_BIT(
                        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET),
                        Port_Configuration[i].pin_num); /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
            }
        }
    }
}

void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode)
{

#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if (Port_Status == PORT_NOT_INITIALIZED)
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, Port_Init_SID,
        PORT_E_UNINIT);
#endif
    uint8 pin_num = 0;
    uint8 port_num = 0;
    volatile uint32 *PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
    uint8 i = 0;
    switch (Pin)
    {
    case PA0:
        pin_num = PIN_0;
        port_num = PORTA;
        break;
    case PA1:
        pin_num = PIN_1;
        port_num = PORTA;
        break;
    case PA2:
        pin_num = PIN_2;
        port_num = PORTA;
        break;
    case PA3:
        pin_num = PIN_3;
        port_num = PORTA;
        break;
    case PA4:
        pin_num = PIN_4;
        port_num = PORTA;
        break;
    case PA5:
        pin_num = PIN_5;
        port_num = PORTA;
        break;
    case PA6:
        pin_num = PIN_6;
        port_num = PORTA;
        break;
    case PA7:
        pin_num = PIN_7;
        port_num = PORTA;
        break;
    case PB0:
        pin_num = PIN_0;
        port_num = PORTB;
        break;
    case PB1:
        pin_num = PIN_1;
        port_num = PORTB;
        break;
    case PB2:
        pin_num = PIN_2;
        port_num = PORTB;
        break;
    case PB3:
        pin_num = PIN_3;
        port_num = PORTB;
        break;
    case PB4:
        pin_num = PIN_4;
        port_num = PORTB;
        break;
    case PB5:
        pin_num = PIN_5;
        port_num = PORTB;
        break;
    case PB6:
        pin_num = PIN_6;
        port_num = PORTB;
        break;
    case PB7:
        pin_num = PIN_7;
        port_num = PORTB;
        break;
    case PC0:
        pin_num = PIN_0;
        port_num = PORTC;
        break;
    case PC1:
        pin_num = PIN_1;
        port_num = PORTC;
        break;
    case PC2:
        pin_num = PIN_2;
        port_num = PORTC;
        break;
    case PC3:
        pin_num = PIN_3;
        port_num = PORTC;
        break;
    case PC4:
        pin_num = PIN_4;
        port_num = PORTC;
        break;
    case PC5:
        pin_num = PIN_5;
        port_num = PORTC;
        break;
    case PC6:
        pin_num = PIN_6;
        port_num = PORTC;
        break;
    case PC7:
        pin_num = PIN_7;
        port_num = PORTC;
        break;
    case PD0:
        pin_num = PIN_0;
        port_num = PORTD;
        break;
    case PD1:
        pin_num = PIN_1;
        port_num = PORTD;
        break;
    case PD2:
        pin_num = PIN_2;
        port_num = PORTD;
        break;
    case PD3:
        pin_num = PIN_3;
        port_num = PORTD;
        break;
    case PD4:
        pin_num = PIN_4;
        port_num = PORTD;
        break;
    case PD5:
        pin_num = PIN_5;
        port_num = PORTD;
        break;
    case PD6:
        pin_num = PIN_6;
        port_num = PORTD;
        break;
    case PD7:
        pin_num = PIN_7;
        port_num = PORTD;
        break;
    case PE0:
        pin_num = PIN_0;
        port_num = PORTE;
        break;
    case PE1:
        pin_num = PIN_1;
        port_num = PORTE;
        break;
    case PE2:
        pin_num = PIN_2;
        port_num = PORTE;
        break;
    case PE3:
        pin_num = PIN_3;
        port_num = PORTE;
        break;
    case PE4:
        pin_num = PIN_4;
        port_num = PORTE;
        break;
    case PE5:
        pin_num = PIN_5;
        port_num = PORTE;
        break;
    case PE6:
        pin_num = PIN_6;
        port_num = PORTE;
        break;
    case PE7:
        pin_num = PIN_7;
        port_num = PORTE;
        break;
    case PF0:
        pin_num = PIN_0;
        port_num = PORTF;
        break;
    case PF1:
        pin_num = PIN_1;
        port_num = PORTF;
        break;
    case PF2:
        pin_num = PIN_2;
        port_num = PORTF;
        break;
    case PF3:
        pin_num = PIN_3;
        port_num = PORTF;
        break;
    case PF4:
        pin_num = PIN_4;
        port_num = PORTF;
        break;
    case PF5:
        pin_num = PIN_5;
        port_num = PORTF;
        break;
    case PF6:
        pin_num = PIN_6;
        port_num = PORTF;
        break;
    case PF7:
        pin_num = PIN_7;
        port_num = PORTF;
        break;
    default:
#if (PORT_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, Port_Init_SID,
        PORT_E_PARAM_PIN);
#endif
        break;
    }
    boolean changeable = TRUE;
    for (i = 0; i < PORT_CONFIGURED_PINS; i++)
    {
        if (Port_Configuration[i].pin_num == pin_num
                && Port_Configuration[i].port_num == port_num)
        {
            changeable =
                    (Port_Configuration[i].mode_changeable == STD_ON) ?
                    TRUE :
                                                                        FALSE;
            break;  // Exit the loop once a matching pin is found
        }
    }
    if (changeable == FALSE)
    {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, Port_Init_SID,
        PORT_E_MODE_UNCHANGEABLE);
#endif
    }
    else if (changeable == TRUE)
    {
        boolean mode_error = FALSE;
        Port_ConfigType Pin_Modified = {
                .analog = Port_Configuration[i].analog, .dir_changeable =
                        Port_Configuration[i].dir_changeable,
                .direction = Port_Configuration[i].direction, .initial_value =
                        Port_Configuration[i].initial_value,
                .mode = Mode, .mode_changeable =
                        Port_Configuration[i].mode_changeable,
                .pin_num = pin_num, .port_num = port_num, .resistor =
                        Port_Configuration[i].resistor };
        mode_error = Port_ModeErrorCheck(&Pin_Modified);
        if (mode_error == TRUE)
        {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, Port_Init_SID,
            PORT_E_PARAM_INVALID_MODE);
#endif
        }
        else if (mode_error == FALSE)
        {
            switch (port_num)
            {
            case 0:
                PortGpio_Ptr = (volatile uint32*) GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                break;
            case 1:
                PortGpio_Ptr = (volatile uint32*) GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                break;
            case 2:
                PortGpio_Ptr = (volatile uint32*) GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                break;
            case 3:
                PortGpio_Ptr = (volatile uint32*) GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                break;
            case 4:
                PortGpio_Ptr = (volatile uint32*) GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                break;
            case 5:
                PortGpio_Ptr = (volatile uint32*) GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                break;
            }
            ///////////////
            if (Mode == DIO)
            {
                /* Setup the pin mode as GPIO */
                CLEAR_BIT(
                        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET),
                        pin_num); /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
                *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                        + PORT_CTL_REG_OFFSET) &=
                        ~(0x0000000F << (pin_num * 4)); /* Clear the PMCx bits for this pin */
            }
            else
            {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET),
                        pin_num); /* Set Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
                switch (Mode)
                {
                case ADC:
                    *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                            + PORT_CTL_REG_OFFSET) &= ~(0x0000000F
                            << (pin_num * 4)); /* Clear the PMCx bits for this pin */
                    break;
                case ANALOG_COMPARTOR:
                    *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                            + PORT_CTL_REG_OFFSET) &= ~(0x0000000F
                            << (pin_num * 4)); /* Clear the PMCx bits for this pin */
                    break;
                case CAN:
                    if (port_num == PORTF)
                    {
                        *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                                + PORT_CTL_REG_OFFSET) |= (3 << (pin_num * 4));
                    }
                    else
                    {
                        *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                                + PORT_CTL_REG_OFFSET) |= (8 << (pin_num * 4));
                    }
                    break;
                case I2C:
                    *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                            + PORT_CTL_REG_OFFSET) |= (3 << (pin_num * 4));
                    break;
                case PWM:
                    if (port_num == PORTA || port_num == PORTF)
                    {
                        *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                                + PORT_CTL_REG_OFFSET) |= (5 << (pin_num * 4));
                    }
                    else
                    {
                        *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                                + PORT_CTL_REG_OFFSET) |= (4 << (pin_num * 4));
                    }
                    break;
                case QEI:
                    *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                            + PORT_CTL_REG_OFFSET) |= (6 << (pin_num * 4));
                    break;
                case SSI:
                    *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                            + PORT_CTL_REG_OFFSET) |= (2 << (pin_num * 4));
                    break;
                case UART:
                    *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                            + PORT_CTL_REG_OFFSET) |= (1 << (pin_num * 4));
                    break;
                }
                ///////////////
            }
        }
    }
}
void Port_SetPortClock(const Port_ConfigType *ConfigPtr)
{
    switch (ConfigPtr->port_num)
    {
    case PORTA:
        SYSCTL_RCGCGPIO_R |= (1 << PORTA);
        while (!(SYSCTL_PRGPIO_R & (1 << PORTA)))
            ;
        break;
    case PORTB:
        SYSCTL_RCGCGPIO_R |= (1 << PORTB);
        while (!(SYSCTL_PRGPIO_R & (1 << PORTB)))
            ;
        break;
    case PORTC:
        SYSCTL_RCGCGPIO_R |= (1 << PORTC);
        while (!(SYSCTL_PRGPIO_R & (1 << PORTC)))
            ;
        break;
    case PORTD:
        SYSCTL_RCGCGPIO_R |= (1 << PORTD);
        while (!(SYSCTL_PRGPIO_R & (1 << PORTD)))
            ;
        break;
    case PORTE:
        SYSCTL_RCGCGPIO_R |= (1 << PORTE);
        while (!(SYSCTL_PRGPIO_R & (1 << PORTE)))
            ;
        break;
    case PORTF:
        SYSCTL_RCGCGPIO_R |= (1 << PORTF);
        while (!(SYSCTL_PRGPIO_R & (1 << PORTF)))
            ;
        break;
    }
}

boolean Port_ModeErrorCheck(const Port_ConfigType *ConfigPtr)
{
    switch (ConfigPtr->mode)
    {
    case ADC: //PB4 PB5 PD0,1,2,3 PE0,1,2,3,4,5
        if (ConfigPtr->direction != PORT_PIN_IN || ConfigPtr->analog != ANALOG)
            return TRUE;
        if (ConfigPtr->port_num == PORTB)
        {
            if (ConfigPtr->pin_num != PIN_4 || ConfigPtr->pin_num != PIN_5)
                return TRUE;
        }
        else if (ConfigPtr->port_num == PORTD)
        {
            if (ConfigPtr->pin_num != PIN_0 || ConfigPtr->pin_num != PIN_1
                    || ConfigPtr->pin_num != PIN_2
                    || ConfigPtr->pin_num != PIN_3)
                return TRUE;
        }
        else if (ConfigPtr->port_num == PORTE)
        {
            if (ConfigPtr->pin_num == PIN_6 || ConfigPtr->pin_num == PIN_7)
                return TRUE;
        }
        else if (ConfigPtr->port_num == PORTA || ConfigPtr->port_num == PORTC
                || ConfigPtr->port_num == PORTF)
            return TRUE;
        break;
    case ANALOG_COMPARTOR:
        if (ConfigPtr->port_num == PORTC)
        {
            if (ConfigPtr->analog != ANALOG
                    || ConfigPtr->direction != PORT_PIN_IN)
                return TRUE;
            if (ConfigPtr->pin_num != PIN_4 || ConfigPtr->pin_num != PIN_5
                    || ConfigPtr->pin_num != PIN_6
                    || ConfigPtr->pin_num != PIN_7)
                return TRUE;
        }
        else if (ConfigPtr->port_num == PORTF)
        {
            if (ConfigPtr->analog != DIGITAL
                    || ConfigPtr->direction != PORT_PIN_OUT)
                return TRUE;
            if (ConfigPtr->pin_num != PIN_0 || ConfigPtr->pin_num != PIN_1)
                return TRUE;
        }
        else if (ConfigPtr->port_num == PORTA || ConfigPtr->port_num == PORTB
                || ConfigPtr->port_num == PORTD || ConfigPtr->port_num == PORTE)
            return TRUE;
        break;
    case CAN:
        if (ConfigPtr->analog != DIGITAL)
            return TRUE;
        if (ConfigPtr->port_num == PORTA)
        {
            if (ConfigPtr->pin_num == PIN_0)
            {
                if (ConfigPtr->direction != PORT_PIN_IN)
                    return TRUE;
            }
            else if (ConfigPtr->pin_num == PIN_1)
            {
                if (ConfigPtr->direction != PORT_PIN_OUT)
                    return TRUE;
            }
            else if (ConfigPtr->pin_num != PIN_0 || ConfigPtr->pin_num != PIN_1)
                return TRUE;
        }
        else if (ConfigPtr->port_num == PORTF || ConfigPtr->port_num == PORTB
                || ConfigPtr->port_num == PORTE)
        {
            if (ConfigPtr->pin_num == PIN_0 || ConfigPtr->pin_num == PIN_4)
            {
                if (ConfigPtr->direction != PORT_PIN_IN)
                    return TRUE;
            }
            else if (ConfigPtr->pin_num == PIN_3 || ConfigPtr->pin_num == PIN_5)
            {
                if (ConfigPtr->direction != PORT_PIN_OUT)
                    return TRUE;
            }
            else if (ConfigPtr->pin_num == PIN_1 || ConfigPtr->pin_num == PIN_2
                    || ConfigPtr->pin_num == PIN_6
                    || ConfigPtr->pin_num == PIN_7)
                return TRUE;
        }
        else if (ConfigPtr->port_num == PORTC || ConfigPtr->port_num == PORTD)
            return TRUE;
        break;
    case I2C:
        if (ConfigPtr->analog != DIGITAL)
            return TRUE;
        if (ConfigPtr->port_num == PORTB)
        {
            if (ConfigPtr->pin_num != PIN_2 || ConfigPtr->pin_num != PIN_3)
                return TRUE;
        }
        else if (ConfigPtr->port_num == PORTA)
        {
            if (ConfigPtr->pin_num != PIN_6 || ConfigPtr->pin_num != PIN_7)
                return TRUE;
        }
        else if (ConfigPtr->port_num == PORTE)
        {
            if (ConfigPtr->pin_num != PIN_4 || ConfigPtr->pin_num != PIN_5)
                return TRUE;
        }
        else if (ConfigPtr->port_num == PORTD)
        {
            if (ConfigPtr->pin_num != PIN_0 || ConfigPtr->pin_num != PIN_1)
                return TRUE;
        }
        else if (ConfigPtr->port_num == PORTC || ConfigPtr->port_num == PORTF)
            return TRUE;
        break;
    case PWM:
        if (ConfigPtr->direction != PORT_PIN_OUT
                || ConfigPtr->analog != DIGITAL)
            return TRUE;
        if (ConfigPtr->port_num == PORTA)
        {
            if (ConfigPtr->pin_num != PIN_6 || ConfigPtr->pin_num != PIN_7)
                return TRUE;
        }
        else if (ConfigPtr->port_num == PORTB)
        {
            if (ConfigPtr->pin_num != PIN_6 || ConfigPtr->pin_num != PIN_7
                    || ConfigPtr->pin_num != PIN_5
                    || ConfigPtr->pin_num != PIN_4)
                return TRUE;
        }
        else if (ConfigPtr->port_num == PORTC)
        {
            if (ConfigPtr->pin_num != PIN_5 || ConfigPtr->pin_num != PIN_4)
                return TRUE;
        }
        else if (ConfigPtr->port_num == PORTD)
        {
            if (ConfigPtr->pin_num != PIN_0 || ConfigPtr->pin_num != PIN_1)
                return TRUE;
        }
        else if (ConfigPtr->port_num == PORTE)
        {
            if (ConfigPtr->pin_num != PIN_5 || ConfigPtr->pin_num != PIN_4)
                return TRUE;
        }
        else if (ConfigPtr->port_num == PORTF)
        {
            if (ConfigPtr->pin_num != PIN_0 || ConfigPtr->pin_num != PIN_1
                    || ConfigPtr->pin_num != PIN_2
                    || ConfigPtr->pin_num != PIN_3
                    || ConfigPtr->pin_num != PIN_4)
                return TRUE;
        }
        else
            return TRUE;
        break;
    case QEI:
        if (ConfigPtr->direction != PORT_PIN_IN || ConfigPtr->analog != DIGITAL)
            return TRUE;
        if (ConfigPtr->port_num == PORTC)
        {
            if (ConfigPtr->pin_num != PIN_4 || ConfigPtr->pin_num != PIN_5
                    || ConfigPtr->pin_num != PIN_6)
                return TRUE;
        }
        else if (ConfigPtr->port_num == PORTD)
        {
            if (ConfigPtr->pin_num != PIN_3 || ConfigPtr->pin_num != PIN_6
                    || ConfigPtr->pin_num != PIN_7)
                return TRUE;
        }
        else if (ConfigPtr->port_num == PORTF)
        {
            if (ConfigPtr->pin_num != PIN_0 || ConfigPtr->pin_num != PIN_1
                    || ConfigPtr->pin_num != PIN_4)
                return TRUE;
        }
        else if (ConfigPtr->port_num == PORTA || ConfigPtr->port_num == PORTE
                || ConfigPtr->port_num == PORTE)
            return TRUE;
        break;
    case SSI:
        if (ConfigPtr->analog != DIGITAL)
            return TRUE;
        if (ConfigPtr->port_num == PORTA)
        {
            if (ConfigPtr->pin_num != PIN_2 || ConfigPtr->pin_num != PIN_3
                    || ConfigPtr->pin_num != PIN_4
                    || ConfigPtr->pin_num != PIN_5)
                return TRUE;
        }
        else if (ConfigPtr->port_num == PORTB)
        {
            if (ConfigPtr->pin_num != PIN_4 || ConfigPtr->pin_num != PIN_5
                    || ConfigPtr->pin_num != PIN_6
                    || ConfigPtr->pin_num != PIN_7)
                return TRUE;
        }
        else if (ConfigPtr->port_num == PORTD)
        {
            if (ConfigPtr->pin_num != PIN_0 || ConfigPtr->pin_num != PIN_1
                    || ConfigPtr->pin_num != PIN_2
                    || ConfigPtr->pin_num != PIN_3)
                return TRUE;
        }
        else if (ConfigPtr->port_num == PORTF)
        {
            if (ConfigPtr->pin_num != PIN_0 || ConfigPtr->pin_num != PIN_1
                    || ConfigPtr->pin_num != PIN_2
                    || ConfigPtr->pin_num != PIN_3)
                return TRUE;
        }
        else if (ConfigPtr->port_num == PORTC || ConfigPtr->port_num == PORTE)
            return TRUE;
        break;
    case UART:
        if (ConfigPtr->analog != DIGITAL)
            return TRUE;

    }
    return FALSE;
}
