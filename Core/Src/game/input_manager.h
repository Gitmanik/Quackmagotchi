/*
 * input_manager.h
 *
 *  Created on: 15 mar 2022
 *      Author: Gitmanik
 */

#pragma once
#include <stdint.h>

#if defined (STM32F411xE)
#include "stm32f4xx_hal.h"
#else
#error HAL include missing
#endif

typedef enum {
	Button1,
	Button2,
	Button3
} buttons_t;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

uint8_t INPUT_Get_Button(buttons_t button);
void INPUT_ClearInputs();
uint8_t INPUT_TestButton1();
