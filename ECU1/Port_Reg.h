/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_Reg.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Marwan Mobarak
 ******************************************************************************/

#ifndef PORT_REG_H_
#define PORT_REG_H_

/*******************************************************************************
 *                              Module Definitions                             *
 *******************************************************************************/

/* GPIO Registers base addresses */
#define GPIO_PORTA_BASE_ADDRESS           0x40004000
#define GPIO_PORTB_BASE_ADDRESS           0x40005000
#define GPIO_PORTC_BASE_ADDRESS           0x40006000
#define GPIO_PORTD_BASE_ADDRESS           0x40007000
#define GPIO_PORTE_BASE_ADDRESS           0x40024000
#define GPIO_PORTF_BASE_ADDRESS           0x40025000

/* GPIO Registers offset addresses */
#define PORT_DATA_REG_OFFSET              0x3FC
#define PORT_DIR_REG_OFFSET               0x400
#define PORT_ALT_FUNC_REG_OFFSET          0x420
#define PORT_PULL_UP_REG_OFFSET           0x510
#define PORT_PULL_DOWN_REG_OFFSET         0x514
#define PORT_DIGITAL_ENABLE_REG_OFFSET    0x51C
#define PORT_LOCK_REG_OFFSET              0x520
#define PORT_COMMIT_REG_OFFSET            0x524
#define PORT_ANALOG_MODE_SEL_REG_OFFSET   0x528
#define PORT_CTL_REG_OFFSET               0x52C

typedef enum {
    PORTA = 0,
    PORTB,
    PORTC,
    PORTD,
    PORTE,
    PORTF
}Ports;


typedef enum {
    PIN_0 = 0,
    PIN_1,
    PIN_2,
    PIN_3,
    PIN_4,
    PIN_5,
    PIN_6,
    PIN_7
}Pins;


typedef enum
{
    PA0 = 0,PA1, PA2, PA3, PA4, PA5, PA6, PA7,
    PB0,PB1, PB2, PB3, PB4, PB5, PB6, PB7,
    PC0,PC1, PC2, PC3, PC4, PC5, PC6, PC7,
    PD0,PD1, PD2, PD3, PD4, PD5, PD6, PD7,
    PE0,PE1, PE2, PE3, PE4, PE5, PE6, PE7,
    PF0,PF1, PF2, PF3, PF4, PF5, PF6, PF7,
} Pin_Port;

#endif /* PORT_REG_H_ */
