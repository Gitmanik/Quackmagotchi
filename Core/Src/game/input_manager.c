/*
 * input_manager.c
 *
 *  Created on: 15 mar 2022
 *      Author: Gitmanik
 */
#include "input_manager.h"
#include "../../Inc/main.h"

#include <stdint.h>

#define DEBOUNCE_DELAY 20

uint8_t button1_state = 0;
unsigned long button1_tmr;

uint8_t button2_state = 0;
unsigned long button2_tmr;

uint8_t button3_state = 0;
unsigned long button3_tmr;

uint8_t button1_clicked, button2_clicked, button3_clicked;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	unsigned long tick = HAL_GetTick();
	if (GPIO_Pin == BUTTON1_Pin) {
		uint8_t new_state = HAL_GPIO_ReadPin(BUTTON1_GPIO_Port, BUTTON1_Pin);
		if (new_state != button1_state) {
			if (new_state == 1) {
				if (tick - button1_tmr > DEBOUNCE_DELAY) {
					button1_clicked = true;
				}
			}
			button1_state = new_state;
			button1_tmr = tick;
		}

	}

	if (GPIO_Pin == BUTTON2_Pin) {
		uint8_t new_state = HAL_GPIO_ReadPin(BUTTON2_GPIO_Port, BUTTON2_Pin);
		if (new_state != button2_state) {
			if (new_state == 1) {
				if (tick - button2_tmr > DEBOUNCE_DELAY) {
					button2_clicked = true;
				}
			}

			button2_state = new_state;
			button2_tmr = tick;
		}
	}

	if (GPIO_Pin == BUTTON3_Pin) {
		uint8_t new_state = HAL_GPIO_ReadPin(BUTTON3_GPIO_Port, BUTTON3_Pin);
		if (new_state != button3_state) {
			if (new_state == 1) {
				if (tick - button3_tmr > DEBOUNCE_DELAY) {
					button3_clicked = true;
				}
			}
			button3_state = new_state;
			button3_tmr = tick;
		}
	}
}

uint8_t INPUT_Get_Button(buttons_t button) {

	uint8_t ret = 0;
	switch (button)
	{
	case Button1:
		ret = button1_state;
		button1_state = 0;
		break;
	case Button2:
		ret = button2_state;
		button2_state = 0;
		break;
	case Button3:
		ret = button3_state;
		button3_state = 0;
		break;

	}
	return ret;
}
void INPUT_ClearInputs()
{
	button1_state = button2_state = button3_state = 0;
}

uint8_t INPUT_TestButton1()
{
	return !INPUT_Get_Button(Button1);
}
