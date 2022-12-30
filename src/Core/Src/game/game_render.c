/*
 * game_render.c
 *
 *  Created on: Mar 4, 2022
 *      Author: Gitmanik
 */

#include <string.h>
#include <stdlib.h>
#include "game_render.h"
#include "game_state.h"
#include "../pcd8544/pcd8544.h"

/* all my homies hate radzio dxp
Generated by Gitmanik's image2lcd 2022 */
const unsigned char BITMAP_base[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xFC, 0xFC, 0xC3, 0xC3, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xC0, 0xC0, 0x03, 0x03, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x0C, 0x0C, 0x0C, 0x0C, 0xFC, 0xFC, 0x0F, 0x0F, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x3C, 0x3C, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0, 0xCC, 0xCC, 0x0C, 0x0C, 0x03, 0x03, 0x03, 0x03, 0xF3, 0xF3, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xF0, 0xF0, 0xF0, 0xF0, 0xFC, 0xFC, 0xFF, 0xFF, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x0C, 0x0C, 0x3C, 0x3C, 0xFC, 0xFC, 0x3C, 0x3C, 0x3F, 0x3F, 0x3F, 0x3F, 0xFF, 0xFF, 0x3F, 0x3F, 0x0F, 0x0F, 0x0F, 0x0F, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x03, 0x03, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x03, 0x03, 0x0C, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // 44x44
const unsigned char BITMAP_blink[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xFC, 0xFC, 0xC3, 0xC3, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0x03, 0x03, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x0C, 0x0C, 0x0C, 0x0C, 0xFC, 0xFC, 0x0F, 0x0F, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x3C, 0x3C, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0, 0xCC, 0xCC, 0x0C, 0x0C, 0x03, 0x03, 0x03, 0x03, 0xF3, 0xF3, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xF0, 0xF0, 0xF0, 0xF0, 0xFC, 0xFC, 0xFF, 0xFF, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x0C, 0x0C, 0x3C, 0x3C, 0xFC, 0xFC, 0x3C, 0x3C, 0x3F, 0x3F, 0x3F, 0x3F, 0xFF, 0xFF, 0x3F, 0x3F, 0x0F, 0x0F, 0x0F, 0x0F, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x03, 0x03, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x03, 0x03, 0x0C, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // 44x44
const unsigned char BITMAP_lookingback[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFC, 0x03, 0x03, 0xC0, 0xC0, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0xC3, 0xC3, 0xFC, 0xFC, 0xC0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x3F, 0x3C, 0x3C, 0xFC, 0xFC, 0xFC, 0xFC, 0xFF, 0xFF, 0xCF, 0xCF, 0x0C, 0x0C, 0x03, 0x03, 0x03, 0x03, 0xF3, 0xF3, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xF0, 0xF0, 0xF0, 0xF0, 0xFC, 0xFC, 0xFF, 0xFF, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x0C, 0x0C, 0x3C, 0x3C, 0xFC, 0xFC, 0x3C, 0x3C, 0x3F, 0x3F, 0x3F, 0x3F, 0xFF, 0xFF, 0x3F, 0x3F, 0x0F, 0x0F, 0x0F, 0x0F, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x03, 0x03, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x03, 0x03, 0x0C, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // 44x44
const unsigned char BITMAP_stand1[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xFC, 0xFC, 0xC3, 0xC3, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xC0, 0xC0, 0x03, 0x03, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x0C, 0x0C, 0x0C, 0x0C, 0xFC, 0xFC, 0x0F, 0x0F, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x3C, 0x3C, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0, 0xCC, 0xCC, 0x0C, 0x0C, 0x03, 0x03, 0x03, 0x03, 0xF3, 0xF3, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xF0, 0xF0, 0xF0, 0xF0, 0xFC, 0xFC, 0xFF, 0xFF, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x0C, 0x0C, 0x3C, 0x3C, 0xFC, 0xFC, 0x3C, 0x3C, 0x3F, 0x3F, 0x3F, 0x3F, 0xFF, 0xFF, 0x3F, 0x3F, 0x0F, 0x0F, 0x0F, 0x0F, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x03, 0x03, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x03, 0x03, 0x0C, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // 44x44
const unsigned char BITMAP_stand2[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0x0C, 0x0C, 0x03, 0x03, 0x03, 0x03, 0xC3, 0xC3, 0x03, 0x03, 0x0C, 0x0C, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x33, 0x33, 0x33, 0x33, 0xF3, 0xF3, 0x3F, 0x3F, 0x0C, 0x0C, 0x00, 0x00, 0x03, 0x03, 0xC3, 0xC3, 0xF0, 0xF0, 0xFF, 0xFF, 0xC0, 0xC0, 0xC0, 0xC0, 0x30, 0x30, 0x30, 0x30, 0x0C, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x3F, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0xC3, 0xC3, 0xC3, 0xC3, 0xF3, 0xF3, 0xFC, 0xFC, 0xC0, 0xC0, 0xFC, 0xFC, 0xFF, 0xFF, 0x3F, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x0C, 0x0C, 0x30, 0x30, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x3F, 0x3F, 0x3F, 0x0F, 0x0F, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x03, 0x03, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x03, 0x03, 0x0C, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // 44x44
const unsigned char BITMAP_talk1[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xFC, 0xFC, 0xC3, 0xC3, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xC0, 0xC0, 0x03, 0x03, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x0C, 0x0C, 0x0C, 0x0C, 0xFC, 0xFC, 0x0F, 0x0F, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x3C, 0x3C, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0, 0xCC, 0xCC, 0x0C, 0x0C, 0x03, 0x03, 0x03, 0x03, 0xF3, 0xF3, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xF0, 0xF0, 0xF0, 0xF0, 0xFC, 0xFC, 0xFF, 0xFF, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x0C, 0x0C, 0x3C, 0x3C, 0xFC, 0xFC, 0x3C, 0x3C, 0x3F, 0x3F, 0x3F, 0x3F, 0xFF, 0xFF, 0x3F, 0x3F, 0x0F, 0x0F, 0x0F, 0x0F, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x03, 0x03, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x03, 0x03, 0x0C, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // 44x44
const unsigned char BITMAP_talk2[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0x0C, 0x0C, 0x03, 0x03, 0x03, 0x03, 0xC3, 0xC3, 0x03, 0x03, 0x0C, 0x0C, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x33, 0x33, 0x33, 0x33, 0xF3, 0xF3, 0x3F, 0x3F, 0x0C, 0x0C, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03, 0x30, 0x30, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0, 0xCC, 0xCC, 0x0C, 0x0C, 0x03, 0x03, 0x03, 0x03, 0xF3, 0xF3, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xF0, 0xF0, 0xF0, 0xF0, 0xFC, 0xFC, 0xFF, 0xFF, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x0C, 0x0C, 0x3C, 0x3C, 0xFC, 0xFC, 0x3C, 0x3C, 0x3F, 0x3F, 0x3F, 0x3F, 0xFF, 0xFF, 0x3F, 0x3F, 0x0F, 0x0F, 0x0F, 0x0F, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x03, 0x03, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x03, 0x03, 0x0C, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // 44x44
const unsigned char BITMAP_talk3[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xF0, 0xF0, 0x0C, 0x0C, 0x03, 0x03, 0x03, 0x03, 0xC3, 0xC3, 0x03, 0x03, 0x0C, 0x0C, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF3, 0xF3, 0xCC, 0xCC, 0xF3, 0xF3, 0x3F, 0x3F, 0x0C, 0x0C, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03, 0x30, 0x30, 0xFF, 0xFF, 0xF0, 0xF0, 0x30, 0x30, 0x3C, 0x3C, 0xCC, 0xCC, 0x03, 0x03, 0x03, 0x03, 0xF3, 0xF3, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xF0, 0xF0, 0xF0, 0xF0, 0xFC, 0xFC, 0xFF, 0xFF, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x0C, 0x0C, 0x3C, 0x3C, 0xFC, 0xFC, 0x3C, 0x3C, 0x3F, 0x3F, 0x3F, 0x3F, 0xFF, 0xFF, 0x3F, 0x3F, 0x0F, 0x0F, 0x0F, 0x0F, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x03, 0x03, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x03, 0x03, 0x0C, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // 44x44
const unsigned char BITMAP_sleep[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0x30, 0x30, 0x30, 0x30, 0x3F, 0x3F, 0xF0, 0xF0, 0xC0, 0xC0, 0x00, 0x00, 0x30, 0x20, 0x20, 0x30, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03, 0xFF, 0xFF, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x0F, 0x0F, 0x3F, 0x3F, 0x3C, 0x3C, 0x3C, 0x3C, 0x33, 0x33, 0xC3, 0xC3, 0x00, 0x00, 0xC0, 0xC0, 0xFC, 0xFC, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x3C, 0x3C, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xFC, 0xFC, 0xFC, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xFF, 0xFF, 0x3F, 0x3F, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // 44x44
const unsigned char BITMAP_food[] = {0x0E, 0x11, 0xF1, 0x11, 0x0E, 0x00, 0x1F, 0x10, 0xFF, 0x10, 0x1F, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00}; // 11x10
const unsigned char BITMAP_energy[] = {0x00, 0x00, 0xFE, 0xAA, 0xAB, 0xAB, 0xAB, 0xAA, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 0x00}; // 11x10
const unsigned char BITMAP_happiness[] = {0x00, 0x40, 0x9C, 0x9C, 0x0C, 0x00, 0x0C, 0x9C, 0x9C, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00}; // 11x10

const unsigned char BITMAP_title[] = {0x00, 0x00, 0x00, 0xC0, 0xF0, 0xA8, 0x58, 0xC0, 0x00, 0x00, 0x80, 0xC0, 0x60, 0x00, 0xC0, 0xE0, 0x00, 0xC0, 0x80, 0x80, 0x00, 0x00, 0x80, 0x40, 0x40, 0xC0, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0x40, 0xA0, 0xE0, 0x40, 0x20, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0xC0, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0xC0, 0x00, 0x00, 0x80, 0x80, 0x40, 0x80, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0xC0, 0x00, 0x00, 0x80, 0x40, 0x40, 0xC0, 0x40, 0x00, 0x00, 0x80, 0xC0, 0xD8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x05, 0x07, 0x01, 0x06, 0x05, 0x04, 0x02, 0x01, 0x00, 0x07, 0x04, 0x02, 0x07, 0x04, 0x04, 0x03, 0x00, 0x00, 0x02, 0x07, 0x04, 0x02, 0x07, 0x04, 0x02, 0x01, 0x00, 0x07, 0x01, 0x00, 0x07, 0x01, 0x00, 0x07, 0x07, 0x02, 0x00, 0x02, 0x07, 0x04, 0x02, 0x07, 0x04, 0x02, 0x30, 0x37, 0x2C, 0x3A, 0x0F, 0x05, 0x06, 0x00, 0x07, 0x04, 0x04, 0x03, 0x01, 0x01, 0x02, 0x07, 0x04, 0x04, 0x02, 0x00, 0x04, 0x06, 0x07, 0x09, 0x08, 0x06, 0x00, 0x01, 0x00, 0x07, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // 84x16
int ANIM_Quack[3];
int ANIM_Blink[1];
int ANIM_Stand[2];
void RENDER_Init()
{
	  ANIM_Quack[0] = (int) &BITMAP_talk1;
	  ANIM_Quack[1] = (int) &BITMAP_talk2;
	  ANIM_Quack[2] = (int) &BITMAP_talk3;

	  ANIM_Blink[0] = (int) &BITMAP_blink;

	  ANIM_Stand[0] = (int) &BITMAP_stand1;
	  ANIM_Stand[1] = (int) &BITMAP_stand2;
}

void RENDER_DrawStat(pcd8544_config_t* lcd, uint8_t idx, unsigned char* image, uint8_t val)
{
	PCD8544_DrawLine(lcd, (PCD8544_LCD_WIDTH - 1) - STAT_BITMAP_WIDTH - 1, (STAT_BITMAP_HEIGHT+1) * (idx), (PCD8544_LCD_WIDTH - 1), (STAT_BITMAP_HEIGHT+1) * (idx), 1);
	PCD8544_DrawLine(lcd, (PCD8544_LCD_WIDTH - 1) - STAT_BITMAP_WIDTH - 1, (STAT_BITMAP_HEIGHT+1) * (idx+1), (PCD8544_LCD_WIDTH - 1), (STAT_BITMAP_HEIGHT+1) * (idx+1), 1);

	PCD8544_DrawLine(lcd, (PCD8544_LCD_WIDTH - 1) - STAT_BITMAP_WIDTH - 1, (STAT_BITMAP_HEIGHT + 1) * idx, (PCD8544_LCD_WIDTH - 1) - STAT_BITMAP_WIDTH - 1, (STAT_BITMAP_HEIGHT + 1) * (idx + 1), 1);
	PCD8544_DrawLine(lcd, (PCD8544_LCD_WIDTH - 1), (STAT_BITMAP_HEIGHT + 1) * idx, (PCD8544_LCD_WIDTH - 1), (STAT_BITMAP_HEIGHT + 1) * (idx + 1), 1);

	for (uint8_t x_idx = 0; x_idx < STAT_BITMAP_WIDTH; x_idx++)
	{
		for (uint8_t y_idx = 0; y_idx < STAT_BITMAP_HEIGHT; y_idx++)
		{
			uint8_t bit = (image[x_idx + (y_idx / 8) * STAT_BITMAP_WIDTH] >> y_idx % 8) & 1;

			float war = round((float) (1+y_idx) / (float) (1+STAT_BITMAP_HEIGHT) * 100.0) / 100.0;
			PCD8544_SetPixel(lcd, 83-STAT_BITMAP_WIDTH + x_idx, 1 * (idx+1) + STAT_BITMAP_HEIGHT*idx + y_idx, 1 - war > (float) val / 100.0 ? bit : !bit);
		}
	}
}

uint8_t RENDER_Animate(pcd8544_config_t* lcd, int* anim, size_t len, uint32_t delay, uint8_t (*func) ())
{
	for (uint8_t idx = 0; idx < len; idx++) {
		RENDER_DrawDuck(lcd, (unsigned char*) anim[idx]);
		PCD8544_UpdateScreen(lcd);
		if (!STATE_Delay(delay, func))
			return 0;
	}
	return 1;
}

void RENDER_DrawDuck(pcd8544_config_t* lcd, unsigned char* image)
{
	PCD8544_DrawImage(lcd, 10, 1, DUCK_BITMAP_WIDTH, DUCK_BITMAP_HEIGHT, image);
}

void _RENDER_DebugValue(pcd8544_config_t* lcd, char *str, unsigned int val, uint8_t idx) {
	char buf[16];
	memset(buf, 0, 16);
	utoa(val, buf, 10);

	PCD8544_WriteString(lcd, 0, idx * (PCD8544_FONT_HEIGHT + 1), str, 1);
	PCD8544_WriteString(lcd, strlen(str) * (PCD8544_FONT_WIDTH + 1), idx * (PCD8544_FONT_HEIGHT + 1), buf, 1);
}

void RENDER_RenderDebugScreen(pcd8544_config_t* lcd)
{
	_RENDER_DebugValue(lcd, "food:", STATE_food, 0);
	_RENDER_DebugValue(lcd, "happ:", STATE_happiness, 1);
	_RENDER_DebugValue(lcd, "bat mV:", battery_volt, 2);
	_RENDER_DebugValue(lcd, "energy:", STATE_energy, 3);
}

void RENDER_RenderMenu(pcd8544_config_t* lcd, menu_t *menu, uint8_t pos)
{
	PCD8544_DrawImage(lcd, 0, 0, 83, 15, &BITMAP_title);

	for (int menu_idx = 0; menu_idx < menu->menu_length; menu_idx++) {

		char* menu_str = menu->menu_entries + MENU_ENTRY_LENGTH * menu_idx;
		uint8_t yPos = PCD8544_LCD_HEIGHT/2 - (menu->menu_length) * (PCD8544_FONT_HEIGHT + 1) / 2 + (menu_idx) * (PCD8544_FONT_HEIGHT + 1);
		uint8_t xPos = PCD8544_LCD_WIDTH/2 - (strlen(menu_str) + (menu_idx == pos ? 4 : 0)) * (PCD8544_FONT_WIDTH + 1) /2;

		if (menu_idx == pos) {
			PCD8544_WriteString(lcd, xPos, yPos, "> ", 1);
			PCD8544_WriteString(lcd, xPos + (PCD8544_FONT_WIDTH + 1) * (strlen(menu_str) + 2), yPos, " <", 1);
		}
		PCD8544_WriteString(lcd, xPos + (pos == menu_idx ? (PCD8544_FONT_WIDTH + 1) * 2 : 0), yPos, menu->menu_entries + MENU_ENTRY_LENGTH * menu_idx, 1);
	}
}

void RENDER_RenderIdle(pcd8544_config_t* lcd)
{
	RENDER_DrawStat(lcd, 0, (unsigned char*) &BITMAP_energy,
			STATE_energy);
	RENDER_DrawStat(lcd, 1, (unsigned char*) &BITMAP_food,
			STATE_food);
	RENDER_DrawStat(lcd, 2, (unsigned char*) &BITMAP_happiness,
			STATE_happiness);

	RENDER_DrawDuck(lcd, (unsigned char*) &BITMAP_sleep);
}