/*
 * game_render.h
 *
 *  Created on: Mar 4, 2022
 *      Author: Gitmanik
 */

#ifndef GAME_RENDER_H_
#define GAME_RENDER_H_

#include <math.h>
#include "../pcd8544/pcd8544.h"
void RENDER_DrawStat(pcd8544_config_t* lcd, uint8_t idx, unsigned char* image, float val);

#endif /* SRC_GAME_GAME_RENDER_H_ */
