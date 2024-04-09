/******************************************************************************
 *
 * Module: Temperature Sensor
 *
 * File Name: temp_sen.h
 *
 * Description: Header file for Temperature Sensor driver.
 *
 * Author: Marwan Mobarak
 ******************************************************************************/

#ifndef TEMP_SEN_H_
#define TEMP_SEN_H_

#include "adc.h"
#include "adc_cfg.h"
#include "Std_Types.h"

#define AVG_READINGS 3
#define TEMP_ADC_MODULE ADC0_INDEX
uint32 temperatureSensorRead(void);
#endif /* TEMP_SEN_H_ */
