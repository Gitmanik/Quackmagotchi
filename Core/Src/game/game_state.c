/*
 * game_state.c
 *
 *  Created on: 5 mar 2022
 *      Author: Gitmanik
 */

#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "game_state.h"
#include "game_render.h"
#include "input_manager.h"

#include "../pcd8544/pcd8544.h"

#include "../misc/queue.h"
#include "../misc/math.h"

#include "../../Inc/main.h"

static queue_t queue;
static game_state_t STATE_state;
static uint8_t menu_pos;
static uint32_t last_idle_anim = 0, last_food = 0, last_bat = 0, menu_entered = 0;

int8_t STATE_food, STATE_energy, STATE_happiness; //min: 0, max: 100
uint32_t battery_volt;

void _STATE_HandleStatistics();

uint8_t _STATE_MockTest()
{
	return 1;
}

#if 1 /* Menu glowne*/
#define DLUGOSC_MENU_GLOWNEGO 3
char menu_glowne_tekst[][MENU_ENTRY_LENGTH] = {
		"Nakarm\0         ",
		"Debugowanie\0    ",
		"Powrot\0"
};

void MENU_OnClick_Nakarm(pcd8544_config_t *lcd) {
	STATE_AddToStat(&STATE_food, 35);
	last_idle_anim = HAL_GetTick();
	STATE_QueueState(IDLE);
}

void MENU_OnClick_Debugowanie(pcd8544_config_t* lcd)
{
	STATE_QueueState(DEBUG_SCREEN);
}

void MENU_OnClick_Powrot(pcd8544_config_t* lcd)
{
	STATE_QueueState(IDLE);
}

MENU_OnClick menu_glowne_callbacki[DLUGOSC_MENU_GLOWNEGO] = { MENU_OnClick_Nakarm, MENU_OnClick_Debugowanie, MENU_OnClick_Powrot };
menu_t menu_glowne;
#endif

uint8_t STATE_Delay(uint32_t Delay, uint8_t (*func) ()) {

	uint32_t tickstart = HAL_GetTick();
	uint32_t wait = Delay;

	/* Add a period to guarantee minimum wait */
	if (wait < HAL_MAX_DELAY) {
		wait += (uint32_t) uwTickFreq;
	}

	while ((HAL_GetTick() - tickstart) < wait) {
		if (!func())
			return 0;

		__WFI();
	}
	return 1;
}

void STATE_Init() {
	STATE_state = REDRAW_IDLE;
	//todo wczytywanie z eeprom..
	STATE_food = 5;
	STATE_happiness = 50;

	RENDER_Init();
	QUEUE_Init(&queue);

	menu_glowne.menu_entries = (char*) &menu_glowne_tekst;
	menu_glowne.menu_callbacks = (MENU_OnClick*) &menu_glowne_callbacki;
	menu_glowne.menu_length = DLUGOSC_MENU_GLOWNEGO;
}

void STATE_Tick() {

	_STATE_HandleStatistics();

	if (!QUEUE_isEmpty(&queue)) {
		STATE_SetState(QUEUE_get(&queue));
	}

	if (STATE_state == QUACK) {
		if (!RENDER_Animate(&pcd8544_handle, ANIM_Quack, 3, 150, INPUT_TestButton1))
			STATE_QueueState(MENU);
		else
			STATE_QueueState(IDLE);
	}

	if (STATE_state == REDRAW_IDLE) {

		RENDER_DrawStat(&pcd8544_handle, 0, (unsigned char*) &BITMAP_energy,
				STATE_energy);
		RENDER_DrawStat(&pcd8544_handle, 1, (unsigned char*) &BITMAP_food,
				STATE_food);
		RENDER_DrawStat(&pcd8544_handle, 2, (unsigned char*) &BITMAP_happiness,
				STATE_happiness);
		RENDER_DrawStat(&pcd8544_handle, 3, (unsigned char*) &BITMAP_happiness,
				0);

		RENDER_DrawDuck(&pcd8544_handle, (unsigned char*) &BITMAP_base);

		PCD8544_UpdateScreen(&pcd8544_handle);
		STATE_QueueState(IDLE);
	}

	if (STATE_state == IDLE) {
		if (HAL_GetTick() - last_idle_anim > 10000) {

			if (rand() % 2 == 0)
				RENDER_Animate(&pcd8544_handle, ANIM_Stand, 2, 400, _STATE_MockTest);
			else
				RENDER_Animate(&pcd8544_handle, ANIM_Blink, 1, 100, _STATE_MockTest);

			RENDER_DrawDuck(&pcd8544_handle, (unsigned char*) &BITMAP_base);
			PCD8544_UpdateScreen(&pcd8544_handle);

			last_idle_anim = HAL_GetTick();
		}

		if (INPUT_Get_Button(Button1)) {
			menu_pos = 0;
			menu_entered = HAL_GetTick();
			STATE_QueueState(MENU);
		}
		else if (INPUT_Get_Button(Button2))
			STATE_QueueState(QUACK);

		INPUT_Clear_Buttons();

		__WFI();
	}

	if (STATE_state == DEBUG_SCREEN) {

		PCD8544_ClearBuffer(&pcd8544_handle);

		RENDER_RenderDebugScreen(&pcd8544_handle);

		PCD8544_UpdateScreen(&pcd8544_handle);

		if (!STATE_Delay(100, INPUT_TestButton1)) {
			STATE_QueueState(IDLE);
		}
		INPUT_Clear_Buttons();
	}

	if (STATE_state == MENU) {

		PCD8544_ClearBuffer(&pcd8544_handle);

		RENDER_RenderMenu(&pcd8544_handle, &menu_glowne, menu_pos);

		PCD8544_UpdateScreen(&pcd8544_handle);

		if (INPUT_Get_Button(Button1)) {
			menu_glowne.menu_callbacks[menu_pos](&pcd8544_handle);
		}
		else if (INPUT_Get_Button(Button2)) {
			if (menu_pos < menu_glowne.menu_length - 1)
				menu_pos++;
		}
		else if (INPUT_Get_Button(Button3)) {
			if (menu_pos > 0)
				menu_pos--;
		}
		else if (HAL_GetTick() - menu_entered > 15000)
			STATE_QueueState(IDLE);
	}
}

#if 1 /* Zarzadzanie statystykami */

void _STATE_HandleStatistics() {

	uint32_t tick = HAL_GetTick();
	if (tick - last_food > 15000) {
		STATE_AddToStat(&STATE_food, -1);
		last_food = tick;
	}

	if (tick - last_bat > 3000) {

		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
		battery_volt = (uint32_t) ((float) HAL_ADC_GetValue(&hadc1) / 4096 * 3.3
				* 1000) + 1000; //todo: usunac, to jest do testowania wskaznika baterii
		HAL_ADC_Stop(&hadc1);

		STATE_energy = map(battery_volt, 3700, 4200, 0, 100);

		last_bat = tick;
	}
}

void STATE_AddToStat(int8_t *stat, int8_t val) {
	if (*stat + val < 0)
		*stat = 0;
	else if (*stat + val > 100)
		*stat = 100;
	else
		*stat += val;

	if (STATE_GetState() == IDLE) {
		STATE_QueueState(REDRAW_IDLE);
	}
}
#endif

#if 1 /* Zarzadzanie stanem gry */

void STATE_QueueState(game_state_t q_state) {
#ifdef HAL_UART_MODULE_ENABLED
	printf("Queueing %d\n", q_state);
#endif
	QUEUE_insert(&queue, q_state);
}

void STATE_SetState(game_state_t new_state) {
	if (STATE_state == new_state)
		return;

	if (new_state == QUACK) {
		if (STATE_state == IDLE || STATE_state == REDRAW_IDLE) {
			STATE_state = QUACK;
			return;
		}
		else
		{
			new_state = IDLE; // will be changed into redraw_idle in next step
			STATE_QueueState(QUACK);
		}
	}

	if (new_state == IDLE) {
		if (STATE_state == REDRAW_IDLE) {
			STATE_state = IDLE;
			return;
		} else {
			new_state = REDRAW_IDLE;
		}
	}

	STATE_state = new_state;
	PCD8544_ClearBuffer(&pcd8544_handle);
}

game_state_t STATE_GetState() {
	return STATE_state;
}
#endif
