/*
 * game_state.h
 *
 *  Created on: 5 mar 2022
 *      Author: Gitmanik
 */

#pragma once

#include <stdint.h>

typedef enum {

	IDLE,
	REDRAW_IDLE,
	QUACK,
	DEBUG_SCREEN,
	MENU

} game_state_t;


extern int8_t STATE_food, STATE_happiness, STATE_energy;
extern uint32_t battery_volt;

void STATE_Init();
void STATE_Tick();
void STATE_TimerTick();

uint8_t STATE_Delay(uint32_t Delay, uint8_t *func());

void STATE_SetState(game_state_t new_state);
game_state_t STATE_GetState();
void STATE_QueueState(game_state_t q_state);

void STATE_AddToStat(int8_t *stat, int8_t val);
