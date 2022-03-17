#include "../pcd8544/pcd8544.h"
//#include "fonts/font_default_5x8.h"
#include "fonts/font_tiny_3x8.h"


#include <assert.h>
#include <string.h>

#define PCD8544_SET_BIT(BYTE, BIT)      ((BYTE) |= (uint8_t)(1u << (uint8_t)(BIT)))
#define PCD8544_CLEAR_BIT(BYTE, BIT)    ((BYTE) &= (uint8_t)(~(uint8_t)(1u << (uint8_t)(BIT))))
#define PCD8544_MIN(VAL1, VAL2)         (((VAL1)<(VAL2))?(VAL1):(VAL2))
#define PCD8544_MAX(VAL1, VAL2)         (((VAL1)>(VAL2))?(VAL1):(VAL2))

#define PCD8544_EXTENDED_INSTRUCTION    ((uint8_t)0x01)

#define PCD8544_DISPLAYNORMAL   ((uint8_t)0x4)
#define PCD8544_DISPLAYINVERTED ((uint8_t)0x5)

// Standard instruction set
#define PCD8544_FUNCTIONSET     ((uint8_t)0x20)
#define PCD8544_DISPLAYCONTROL  ((uint8_t)0x08)
#define PCD8544_SETYADDR        ((uint8_t)0x40)
#define PCD8544_SETXADDR        ((uint8_t)0x80)

// Extended instruction set
#define PCD8544_SETBIAS         ((uint8_t)0x10)
#define PCD8544_SETVOP          ((uint8_t)0x80)

static bool PCD8544_WriteSPI(pcd8544_config_t *handle, uint8_t *values, uint16_t size)
{
	HAL_GPIO_WritePin(handle->ce_port_handle, handle->ce_pin, GPIO_PIN_RESET);
	HAL_StatusTypeDef status = HAL_SPI_Transmit(handle->spi_handle, values, size, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(handle->ce_port_handle, handle->ce_pin, GPIO_PIN_SET);
	return status == HAL_OK;
}

static bool PCD8544_WriteData(pcd8544_config_t *handle, uint8_t *data, uint16_t size)
{
	HAL_GPIO_WritePin(handle->dc_port_handle, handle->dc_pin, GPIO_PIN_SET);
	return PCD8544_WriteSPI(handle, data, size);
}

static bool PCD8544_WriteCommand(pcd8544_config_t *handle, uint8_t command)
{
	HAL_GPIO_WritePin(handle->dc_port_handle, handle->dc_pin, GPIO_PIN_RESET);
	return PCD8544_WriteSPI(handle, &command, 1);
}

bool PCD8544_Init(pcd8544_config_t *handle)
{
	assert(handle->spi_handle != NULL);
	assert(handle->dc_port_handle != NULL);
	assert(handle->rst_port_handle != NULL);
	assert(handle->ce_port_handle != NULL);

	HAL_GPIO_WritePin(handle->ce_port_handle, handle->ce_pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(handle->rst_port_handle, handle->rst_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(handle->rst_port_handle, handle->rst_pin, GPIO_PIN_SET);

	bool success = true;


	success &= PCD8544_SetBias(handle, 4);
	success &= PCD8544_SetContrast(handle, 55);

	success &= PCD8544_SetInverted(handle, true);

	PCD8544_ClearBuffer(handle);

	success &= PCD8544_UpdateScreen(handle);

	return success;
}

bool PCD8544_SetContrast(pcd8544_config_t *handle, uint8_t contrast)
{
	if (contrast > 0x7f) {
		return false;
	}

	bool success = true;

	success &= PCD8544_WriteCommand(handle, PCD8544_FUNCTIONSET | PCD8544_EXTENDED_INSTRUCTION);
	success &= PCD8544_WriteCommand(handle, PCD8544_SETVOP | contrast);
	success &= PCD8544_WriteCommand(handle, PCD8544_FUNCTIONSET);

	return success;
}

bool PCD8544_SetBias(pcd8544_config_t *handle, uint8_t bias)
{
	if (bias > 0x07) {
		return false;
	}

	bool success = true;

	success &= PCD8544_WriteCommand(handle, PCD8544_FUNCTIONSET | PCD8544_EXTENDED_INSTRUCTION);
	success &= PCD8544_WriteCommand(handle, PCD8544_SETBIAS | bias);
	success &= PCD8544_WriteCommand(handle, PCD8544_FUNCTIONSET);

	return success;
}

bool PCD8544_SetInverted(pcd8544_config_t *handle, bool inverted)
{
	if (inverted) {
		return PCD8544_WriteCommand(handle, PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);
	} else {
		return PCD8544_WriteCommand(handle, PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYINVERTED);
	}
}

void PCD8544_SetPixel(pcd8544_config_t *handle, uint8_t x, uint8_t y, pcd8544_color_t color)
{
	assert(x < PCD8544_LCD_WIDTH);
	assert(y < PCD8544_LCD_HEIGHT);

	if (color == PCD8544_COLOR_BLACK) {
		PCD8544_SET_BIT(handle->buffer[x + (y / 8) * PCD8544_LCD_WIDTH], y % 8);
	} else {
		PCD8544_CLEAR_BIT(handle->buffer[x + (y / 8) * PCD8544_LCD_WIDTH], y % 8);
	}

	if (!handle->update_required) {
		handle->update_min_x = handle->update_max_x = x;
		handle->update_min_y = handle->update_max_y = y;
		handle->update_required = true;

	} else {
		handle->update_min_x = PCD8544_MIN(x, handle->update_min_x);
		handle->update_min_y = PCD8544_MIN(y, handle->update_min_y);
		handle->update_max_x = PCD8544_MAX(x, handle->update_max_x);
		handle->update_max_y = PCD8544_MAX(y, handle->update_max_y);
	}
}

bool PCD8544_UpdateScreen(pcd8544_config_t *handle)
{
	if (!handle->update_required) {
		return true;
	}

	bool success = true;

	uint8_t min_page = handle->update_min_y / 8;
	uint8_t max_page = handle->update_max_y / 8;

	for (uint8_t page = min_page; page <= max_page; page++) {

		uint8_t min_column = handle->update_min_x;
		uint8_t max_column = handle->update_max_x;

		success &= PCD8544_WriteCommand(handle, PCD8544_SETYADDR | page);
		success &= PCD8544_WriteCommand(handle, PCD8544_SETXADDR | min_column);

		success &= PCD8544_WriteData(handle, &handle->buffer[PCD8544_LCD_WIDTH * page + min_column], (max_column - min_column) + 1);
		success &= PCD8544_WriteCommand(handle, PCD8544_SETYADDR);
	}

	if (success) {
		handle->update_required = false;
	}

	return success;
}

void PCD8544_DrawImage(pcd8544_config_t *handle, uint8_t x, uint8_t y, uint8_t width, uint8_t height, unsigned char* image)
{
	assert(x + width < PCD8544_LCD_WIDTH);
	assert(y + height < PCD8544_LCD_WIDTH);
	for (uint8_t x_idx = 0; x_idx < width; x_idx++)
	{
		for (uint8_t y_idx = 0; y_idx < height; y_idx++)
		{
			PCD8544_SetPixel(handle, x + x_idx, y + y_idx, (image[x_idx + (y_idx / 8) * width] >> (y_idx % 8)) & 1);
		}
	}

}

void PCD8544_DrawLine(pcd8544_config_t *handle, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, pcd8544_color_t color)
{
	//https://github.com/infusion/PCD8544/blob/master/PCD8544.cpp
	int8_t dx = x2 - x1, sx = x1 < x2 ? 1 : -1;
	int8_t dy = y2 - y1, sy = y1 < y2 ? 1 : -1;

	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;

	int8_t ert, err = (dx > dy ? dx : -dy) / 2;

	for ( ; ; ) {
		PCD8544_SetPixel(handle, x1, y1, color);
		if (x1 == x2 && y1 == y2) break;
		ert = err;
		if (ert >-dx) {
			err -= dy;
			x1 += sx;
		}
		if (ert < dy) {
			err += dx;
			y1 += sy;
		}
	}
}

void PCD8544_WriteString(pcd8544_config_t *handle, uint8_t x, uint8_t y, char* str, uint8_t color)
{
	while (*str)
	{
		PCD8544_WriteChar(handle, x, y, *str++, color);
		x += (PCD8544_FONT_WIDTH + 1);
		if (x + PCD8544_FONT_WIDTH > PCD8544_LCD_WIDTH - 1)
		{
			y += (PCD8544_FONT_HEIGHT + 1);
			x = 0;
		}
	}
}

void PCD8544_WriteChar(pcd8544_config_t *handle, uint8_t x, uint8_t y, char ch, uint8_t color)
{
	uint8_t zz = (uint8_t) ch - 0x20;
	for (uint8_t width = 0; width < PCD8544_FONT_WIDTH; width++)
	{
		for (uint8_t height = 0; height < PCD8544_FONT_HEIGHT; height++)
		{
			uint8_t c = 0;
			if ((FONT_Tiny3x8[zz][width] >> height) & 1)
				c = color;
			else
				c = !color;

			PCD8544_SetPixel(handle, x + width, y + height, c);
		}
	}
}

void PCD8544_ClearBuffer(pcd8544_config_t *handle)
{
	memset(&(handle->buffer), 0, PCD8544_LCD_WIDTH * PCD8544_LCD_HEIGHT / 8);
	handle->update_required = true;
	handle->update_min_x = 0;
	handle->update_min_y = 0;
	handle->update_max_x = PCD8544_LCD_WIDTH - 1;
	handle->update_max_y = PCD8544_LCD_HEIGHT - 1;
}
