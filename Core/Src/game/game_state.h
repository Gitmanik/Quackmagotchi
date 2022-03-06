/*
 * game_state.h
 *
 *  Created on: 5 mar 2022
 *      Author: Gitmanik
 */

#pragma once

#include <stdint.h>

typedef enum {

	IDLE, // Base state
	REDRAW_IDLE, // Draws over existing buffer DO NOT SET - IT WILL BE SET AUTOMATICALLY BY SETSTATE
	QUACK, // Draws over existing buffer
	DEBUG_SCREEN // Clears buffer

} game_state_t;


void STATE_Init();
void STATE_Tick();
void STATE_TimerTick();


void STATE_SetState(game_state_t new_state);
game_state_t STATE_GetState();
void STATE_QueueState(game_state_t q_state);

void STATE_AddFood(int8_t val);
