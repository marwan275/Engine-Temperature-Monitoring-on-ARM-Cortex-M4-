#ifndef ADC_H_
#define ADC_H_

#include "tm4c123gh6pm.h"  // Include the TM4C123GH6PM header file
#include "Port_Reg.h"      // Include the Port_Reg header file
#include "STD_types.h"
#include "ADC_cfg.h"

typedef enum{ADC0=0,ADC1}ADC_module;
typedef enum {ADC_POOLING=0,ADC_INTERRUPT}ADC_mode;
typedef struct{
Pins pin_num;
ADC_module module_num;
ADC_mode mode;
}ADC_config;
void ADC_Init_Start(ADC_config*configPtr);
uint32 ADC_Read(uint8 adc_index);
float32 ADC_Read_Volt(uint8 adc_index, float32 Source);
extern ADC_config ADC_Configuartions[];

#endif
