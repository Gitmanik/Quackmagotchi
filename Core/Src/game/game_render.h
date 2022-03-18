/*
 * game_render.h
 *
 *  Created on: Mar 4, 2022
 *      Author: Gitmanik
 */

#pragma once
#define MENU_ENTRY_LENGTH 16

#define STAT_BITMAP_WIDTH 11
#define STAT_BITMAP_HEIGHT 10

#define DUCK_BITMAP_WIDTH 44
#define DUCK_BITMAP_HEIGHT 44

#include <math.h>
#include "game_state.h"
#include "../pcd8544/pcd8544.h"

extern int ANIM_Quack[3];
extern int ANIM_Blink[1];
extern int ANIM_Stand[2];
extern const unsigned char BITMAP_base[];
extern const unsigned char BITMAP_blink[];

extern const unsigned char BITMAP_food[];
extern const unsigned char BITMAP_energy[];
extern const unsigned char BITMAP_happiness[];

typedef void (*MENU_OnClick) (pcd8544_config_t*);
typedef struct {
	char* menu_entries;
	MENU_OnClick* menu_callbacks;
	uint8_t menu_length;
} menu_t;

void RENDER_DrawStat(pcd8544_config_t* lcd, uint8_t idx, unsigned char* image, uint8_t val);
void RENDER_DrawDuck(pcd8544_config_t* lcd, unsigned char* image);
uint8_t RENDER_Animate(pcd8544_config_t* lcd, int* anim, size_t len, uint32_t delay, uint8_t (*func) ());

void RENDER_RenderDebugScreen(pcd8544_config_t* lcd);
void RENDER_RenderMenu(pcd8544_config_t* lcd, menu_t *menu, uint8_t pos);

void RENDER_Init();
