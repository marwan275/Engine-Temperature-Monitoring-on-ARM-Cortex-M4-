#include <adc_cfg.h>

ADC_config ADC_Configuartions[NUMBER_ADC_MODULES_CONFIGURED] = {
        { .module_num = ADC0, .mode = ADC_POOLING, .pin_num = PIN_3,.ADC_channel = AN0, }, {
                .module_num = ADC1, .mode = ADC_POOLING, .pin_num = PIN_3,.ADC_channel = AN4, }, };
