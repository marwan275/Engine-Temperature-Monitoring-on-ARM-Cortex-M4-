/*
 * Systick_cfg.c
 *
 *  Created on: Dec 16, 2023
 *      Author: Marwan Mobarak
 */
#include "Systick_cfg.h"

Systick_config config_systick_timer = { .mode = SYSTICK_MODE, .reload_value =
OS_TICK };

