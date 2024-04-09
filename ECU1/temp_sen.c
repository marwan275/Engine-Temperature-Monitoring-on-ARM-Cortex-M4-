 /******************************************************************************
 *
 * Module: Temperature Sensor
 *
 * File Name: temp_sen.c
 *
 * Description: Source file for Temperature Sensor driver.
 *
 * Author: Marwan Mobarak
 ******************************************************************************/

#include "temp_sen.h"

uint32 temperatureSensorRead(void) {
    // Number of readings to take


    // Storage for the readings
    uint32 readings[AVG_READINGS];

    // Take multiple readings
    for (int i = 0; i < AVG_READINGS; ++i) {
        readings[i] = ADC_Read(TEMP_ADC_MODULE);  // Assuming channel 0, adjust as needed
    }

    // Calculate the average
    uint32 sum = 0;
    for (int i = 0; i < AVG_READINGS; ++i) {
        sum += readings[i];
    }

    uint32 average = sum / AVG_READINGS;

    return average;
}
