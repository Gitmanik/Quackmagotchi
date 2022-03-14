/*
 * game_state.c
 *
 *  Created on: 5 mar 2022
 *      Author: Gitmanik
 */

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "game_state.h"
#include "game_render.h"

#include "../pcd8544/pcd8544.h"

#include "../misc/queue.h"
#include "../misc/math.h"

#include "../../Inc/main.h"

static queue_t queue;
uint8_t menu_pos;

static game_state_t STATE_state;

int8_t STATE_food, STATE_energy, STATE_happiness; //min: 0, max: 100
uint32_t battery_volt;
uint32_t last_quack = 0, last_food = 0, last_bat = 0;

void Handle_Statistics();

bool Get_Button(bool *button_state) {
	bool ret = *button_state;
	*button_state = 0;
	return ret;
}

#define DLUGOSC_GOWNA 4
char menu[][MENU_ENTRY_LENGTH] = {
		"Zrob kwa!\0      ", //17 with \0
		"Dodaj 10 food\0  ",
		"Odejmij 10 food\0",
		"Debug Screen\0   "
};

void kwa(pcd8544_config_t *lcd) {
	STATE_QueueState(QUACK);
}
void dodaj(pcd8544_config_t *lcd) {
	STATE_Add(&STATE_food, 10);
	STATE_QueueState(IDLE);
}
void zabierz(pcd8544_config_t *lcd) {
	STATE_Add(&STATE_food, -10);
	STATE_QueueState(IDLE);
}

void debug(pcd8544_config_t* lcd)
{
	STATE_QueueState(DEBUG_SCREEN);
}

MENU_OnClick menu_cbks[DLUGOSC_GOWNA] = { kwa, dodaj, zabierz, debug };

menu_t gowno;

void STATE_Init() {
	STATE_state = REDRAW_IDLE;
	//todo wczytywanie z eeprom..
	STATE_food = 5;
	STATE_happiness = 50;
	QUEUE_Init(&queue);

	gowno.menu_entries = &menu;
	gowno.menu_callbacks = &menu_cbks;
	gowno.menu_length = DLUGOSC_GOWNA;
}

void STATE_Tick() {

	Handle_Statistics();

	if (!QUEUE_isEmpty(&queue)) {
		game_state_t new_state = QUEUE_get(&queue);
#ifdef HAL_UART_MODULE_ENABLED
		printf("Dequeueing %d\n", new_state);
#endif

		STATE_SetState(new_state);
	}

	if (STATE_state == QUACK) {
		if (rand() % 2 == 0)
			RENDER_Animate(&pcd8544_handle, ANIM_Quack, 3, 150);
		else if (rand() % 2 == 0)
			RENDER_Animate(&pcd8544_handle, ANIM_Stand, 2, 400);
		else
			RENDER_Animate(&pcd8544_handle, ANIM_Blink, 1, 100);

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

		PCD8544_DrawLine(&pcd8544_handle, 83, 0, 83, 47, 1);

		PCD8544_UpdateScreen(&pcd8544_handle);
		STATE_QueueState(IDLE);
	}

	if (STATE_state == IDLE) {
		if (HAL_GetTick() - last_quack > 5000) {
			STATE_QueueState(QUACK);
			last_quack = HAL_GetTick();
		}

		if (Get_Button(&button1_clicked)) {
			menu_pos = 0;
			STATE_QueueState(MENU);
		}

		if (Get_Button(&button2_clicked))
			STATE_QueueState(QUACK);

		__WFI();
	}

	if (STATE_state == DEBUG_SCREEN) {

		PCD8544_ClearBuffer(&pcd8544_handle);

		RENDER_RenderDebugScreen(&pcd8544_handle);

		PCD8544_UpdateScreen(&pcd8544_handle);

		HAL_Delay(100);

		if (Get_Button(&button1_clicked)) {
			STATE_QueueState(IDLE);
		}
	}

	if (STATE_state == MENU) {
		PCD8544_ClearBuffer(&pcd8544_handle);

		for (int menu_idx = 0; menu_idx < gowno.menu_length; menu_idx++) {
			if (menu_idx == menu_pos) {
				PCD8544_WriteString(&pcd8544_handle, 0, menu_idx * 6, "> ", 1);
				PCD8544_WriteString(&pcd8544_handle, 4 * strlen( gowno.menu_entries + MENU_ENTRY_LENGTH * menu_idx), menu_idx * 6, " <", 1);
			}
			PCD8544_WriteString(&pcd8544_handle, menu_pos == menu_idx ? 4 : 0, menu_idx * 6, gowno.menu_entries + MENU_ENTRY_LENGTH * menu_idx, 1);
		}

		PCD8544_UpdateScreen(&pcd8544_handle);

		if (Get_Button(&button1_clicked)) {
			gowno.menu_callbacks[menu_pos](&pcd8544_handle);
		}

		if (Get_Button(&button2_clicked)) {
			if (menu_pos < gowno.menu_length - 1)
				menu_pos++;
		}
		if (Get_Button(&button3_clicked)) {
			if (menu_pos > 0)
				menu_pos--;
		}
	}
}

void Handle_Statistics() {

	uint32_t tick = HAL_GetTick();
	if (tick - last_food > 15000) {
		STATE_Add(&STATE_food, -1);
		last_food = tick;
	}

	if (tick - last_bat > 3000) {

		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
		battery_volt = (uint32_t) ((float) HAL_ADC_GetValue(&hadc1) / 4096 * 3.3
				* 1000) + 1000; //todo: usunac, to jest do tesowania
		HAL_ADC_Stop(&hadc1);

		STATE_energy = map(battery_volt, 3700, 4200, 0, 100);

		last_bat = tick;
	}
}

/* Game State */
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
			new_state = IDLE;
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
/*			*/

void STATE_Add(int8_t *stat, int8_t val) {
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

void STATE_QueueState(game_state_t q_state) {
#ifdef HAL_UART_MODULE_ENABLED
	printf("Queueing %d\n", q_state);
#endif
	QUEUE_insert(&queue, q_state);
}
