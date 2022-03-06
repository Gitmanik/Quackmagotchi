/*
 * game_state.c
 *
 *  Created on: 5 mar 2022
 *      Author: Gitmanik
 */

#include <string.h>
#include <stdlib.h>

#include "game_state.h"
#include "game_render.h"

#include "../pcd8544/pcd8544.h"

#include "../misc/queue.h"

#include "../../Inc/main.h"

static queue_t queue;
static game_state_t STATE_state;
static int8_t STATE_food; //min: 0, max: 100

static uint32_t last_quack = 0, last_food = 0;

void STATE_Init() {
	STATE_state = REDRAW_IDLE;
	STATE_food = 5;
	QUEUE_Init(&queue);
}

void STATE_Tick() {

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
		RENDER_DrawStat(&pcd8544_handle, 0, (unsigned char*) &BITMAP_food,
				STATE_food);
		RENDER_DrawDuck(&pcd8544_handle, (unsigned char*) &BITMAP_base);
		STATE_QueueState(IDLE);
	}

	if (STATE_state == IDLE) {

		uint32_t tick = HAL_GetTick();
		if (tick - last_quack > 5000) {
			STATE_QueueState(QUACK);
			last_quack = tick;
		}
		if (tick - last_food > 15000) {
			STATE_AddFood(-1);
			last_food = tick;
		}

		__WFI();
	}

	if (STATE_state == DEBUG_SCREEN) {
		PCD8544_ClearBuffer(&pcd8544_handle);

		char buf[16];
		memset(buf, 0, 16);
		utoa(STATE_food, buf, 10);
		PCD8544_WriteString(&pcd8544_handle, 0, 0, "food: ", 1);
		PCD8544_WriteString(&pcd8544_handle, 6 * 6, 0, buf, 1);

		memset(buf, 0, 16);
		utoa(HAL_GetTick(), buf, 10);

		PCD8544_WriteString(&pcd8544_handle, 0, 9, "tick: ", 1);
		PCD8544_WriteString(&pcd8544_handle, 6 * 6, 9, buf, 1);

		PCD8544_UpdateScreen(&pcd8544_handle);

		HAL_Delay(100);
	}

	if (button2_clicked) {
		button2_clicked = 0;

		STATE_AddFood(-5);
	}

	if (button1_clicked) {
		button1_clicked = 0;

		STATE_QueueState(STATE_state == DEBUG_SCREEN ? IDLE : DEBUG_SCREEN);
	}

	if (button3_clicked) {
		button3_clicked = 0;

		STATE_AddFood(5);
	}

}

void STATE_QueueState(game_state_t q_state) {
#ifdef HAL_UART_MODULE_ENABLED
	printf("Queueing %d\n", q_state);
#endif
	QUEUE_insert(&queue, q_state);
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
	}

	if (new_state == IDLE) {
		if (STATE_state == QUACK || STATE_state == DEBUG_SCREEN)
			new_state = REDRAW_IDLE;
		else if (STATE_state == REDRAW_IDLE) {
			STATE_state = IDLE;
			return;
		}
	}

	STATE_state = new_state;
	PCD8544_ClearBuffer(&pcd8544_handle);
}

game_state_t STATE_GetState() {
	return STATE_state;
}
/*			*/

void STATE_AddFood(int8_t val) {
	if (STATE_food + val < 0)
		STATE_food = 0;
	else if (STATE_food + val > 100)
		STATE_food = 100;
	else
		STATE_food += val;

	if (STATE_GetState() == IDLE) {
		STATE_QueueState(REDRAW_IDLE);
	}
}
