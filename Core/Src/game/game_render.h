/*
 * game_render.h
 *
 *  Created on: Mar 4, 2022
 *      Author: Gitmanik
 */

#pragma once

#include <math.h>
#include "../pcd8544/pcd8544.h"

extern int ANIM_Quack[3];
extern int ANIM_Blink[1];
extern int ANIM_Stand[2];
extern const unsigned char BITMAP_base[];
extern const unsigned char BITMAP_blink[];

extern const unsigned char BITMAP_food[];

void RENDER_DrawStat(pcd8544_config_t* lcd, uint8_t idx, unsigned char* image, uint8_t val);
void RENDER_DrawDuck(pcd8544_config_t* lcd, unsigned char* image);
void RENDER_Animate(pcd8544_config_t* lcd, int* anim, size_t len, uint32_t delay);

void RENDER_Init();
