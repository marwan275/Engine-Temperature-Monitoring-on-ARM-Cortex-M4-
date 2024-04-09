#include <adc.h>
#include <stdio.h>
// Initialize the specified ADC module
volatile uint32 *ADC_ACTSS_R_ptr = NULL_PTR;
volatile uint32 *ADC_EMUX_R_ptr = NULL_PTR;
volatile uint32 *ADC_SSMUX3_R_ptr = NULL_PTR;
volatile uint32 *ADC_SSCTL3_R_ptr = NULL_PTR;
volatile uint32 *ADC_IM_R_ptr = NULL_PTR;
// Initialize the specified ADC Read
volatile uint32 *ADC_SSFIFO3_R_ptr = NULL_PTR;
volatile uint32 *ADC_PSSI_R_ptr = NULL_PTR;
volatile uint32 *ADC_RIS_R_ptr = NULL_PTR;
volatile uint32 *ADC_ISC_R_ptr = NULL_PTR;

void ADC_Init_Start(ADC_config *configPtr)
{
    uint8 i = 0;

    for (i = 0; i < NUMBER_ADC_MODULES_CONFIGURED; i++)
    {

        SYSCTL_RCGCADC_R |= (1 << configPtr[i].module_num); // Enable the specified ADC module clock

        while (!(SYSCTL_PRADC_R & (1 << configPtr[i].module_num)))
        {
            // Enable ADC clock for the specified module
            // Add a delay here if needed before rechecking the clock status
        }

        if (configPtr[i].mode == ADC_POOLING)
        {
            // Initialize the specified ADC module
            ADC_ACTSS_R_ptr =
                    (configPtr[i].module_num == ADC0) ?
                            &ADC0_ACTSS_R : &ADC1_ACTSS_R;
            ADC_EMUX_R_ptr =
                    (configPtr[i].module_num == ADC0) ?
                            &ADC0_EMUX_R : &ADC1_EMUX_R;
            ADC_SSMUX3_R_ptr =
                    (configPtr[i].module_num == ADC0) ?
                            &ADC0_SSMUX3_R : &ADC1_SSMUX3_R;
            ADC_SSCTL3_R_ptr =
                    (configPtr[i].module_num == ADC0) ?
                            &ADC0_SSCTL3_R : &ADC1_SSCTL3_R;

            *ADC_ACTSS_R_ptr &= ~(1 << configPtr[i].pin_num); // Disable sample sequencer for the specified pin
            *ADC_EMUX_R_ptr &= ~(0xF000); // Software trigger conversion for the pin
            *ADC_SSMUX3_R_ptr = 0;           // Get input from the specified pin
            *ADC_SSCTL3_R_ptr = 0x06; // Take one sample at a time, set flag at 1st sample
            *ADC_ACTSS_R_ptr |= (1 << configPtr[i].pin_num); // Enable sample sequencer for the specified pin
        }
        else if (configPtr[i].mode == ADC_INTERRUPT)
        {

            // Initialize the specified ADC module
            ADC_ACTSS_R_ptr =
                    (configPtr[i].module_num == ADC0) ?
                            &ADC0_ACTSS_R : &ADC1_ACTSS_R;
            ADC_EMUX_R_ptr =
                    (configPtr[i].module_num == ADC0) ?
                            &ADC0_EMUX_R : &ADC1_EMUX_R;
            ADC_SSMUX3_R_ptr =
                    (configPtr[i].module_num == ADC0) ?
                            &ADC0_SSMUX3_R : &ADC1_SSMUX3_R;
            ADC_SSCTL3_R_ptr =
                    (configPtr[i].module_num == ADC0) ?
                            &ADC0_SSCTL3_R : &ADC1_SSCTL3_R;
            ADC_IM_R_ptr =
                    (configPtr[i].module_num == ADC0) ? &ADC0_IM_R : &ADC1_IM_R;

            *ADC_ACTSS_R_ptr &= ~(1 << configPtr[i].pin_num); // Disable sample sequencer for the specified pin
            *ADC_EMUX_R_ptr &= ~(0xF000); // Software trigger conversion for the pin
            *ADC_SSMUX3_R_ptr = 0;           // Get input from the specified pin
            *ADC_SSCTL3_R_ptr = 0x06; // Take one sample at a time, set flag at 1st sample
            *ADC_ACTSS_R_ptr |= (1 << configPtr[i].pin_num);
            *ADC_IM_R_ptr |= (1 << configPtr[i].pin_num); // Enable interrupt for SS3
            NVIC_EN0_R |= (1
                    << (configPtr[i].module_num ? INT_ADC0SS3 : INT_ADC1SS3)); // Enable ADC interrupt in NVIC

        }
    }
}

/*void ADC_IRQHandler(ADC_config*configPtr)
 {


 }*/

// Function to read ADC value from a specific ADC module
uint32 ADC_Read(uint8 adc_index)
{

    uint8 module_num = ADC_Configuartions[adc_index].module_num;
    uint32 result;

    ADC_PSSI_R_ptr = (module_num == 0) ? &ADC0_PSSI_R : &ADC1_PSSI_R;
    ADC_RIS_R_ptr = (module_num == 0) ? &ADC0_RIS_R : &ADC1_RIS_R;
    ADC_SSFIFO3_R_ptr = (module_num == 0) ? &ADC0_SSFIFO3_R : &ADC1_SSFIFO3_R;
    ADC_ISC_R_ptr = (module_num == 0) ? &ADC0_ISC_R : &ADC1_ISC_R;

    *ADC_PSSI_R_ptr = (1 << 3);         // Initiate SS3 conversion
    while ((*ADC_RIS_R_ptr & (1 << 3)) == 0)
        ; // Wait for conversion to be completed
    result = *ADC_SSFIFO3_R_ptr;        // Read ADC result
    *ADC_ISC_R_ptr = (1 << 3);          // Acknowledge completion

    return result;
}

float32 ADC_Read_Volt(uint8 adc_index, float32 Source)
{
    float32 result = ADC_Read(adc_index) * Source / 4095;
    return result;
}

