#pragma once

#if defined (STM32F411xE)
#include "stm32f4xx_hal.h"
#else
#error HAL include missing
#endif

#include <stdbool.h>

#define PCD8544_LCD_WIDTH 84
#define PCD8544_LCD_HEIGHT 48

#define PCD8544_FONT_WIDTH 3
#define PCD8544_FONT_HEIGHT 6

typedef enum {
	PCD8544_COLOR_WHITE = 0,
	PCD8544_COLOR_BLACK
} pcd8544_color_t;

typedef struct {

	GPIO_TypeDef *mosi_port_handle;
	uint16_t mosi_pin;

	GPIO_TypeDef *sck_port_handle;
	uint16_t sck_pin;

	GPIO_TypeDef *dc_port_handle;
	uint16_t dc_pin;

	GPIO_TypeDef *rst_port_handle;
	uint16_t rst_pin;

	uint8_t buffer[PCD8544_LCD_WIDTH * PCD8544_LCD_HEIGHT / 8];

	bool update_required;

	uint8_t update_min_x;
	uint8_t update_min_y;
	uint8_t update_max_x;
	uint8_t update_max_y;

} pcd8544_config_t;

/**
 * Initialises a PCD8544 device by performing a hardware reset, setting default bias and contrast values and clearing
 * the back buffer of the handle.
 * @param handle Handle to a PCD8544 device.
 * @return True on success, false otherwise.
 */
bool PCD8544_Init(pcd8544_config_t *handle);

/**
 *
 * @param handle Handle to a PCD8544 device.
 * @param contrast
 * @return True on success, false otherwise.
 */
bool PCD8544_SetContrast(pcd8544_config_t *handle, uint8_t contrast);

/**
 * Set the display bias.
 * Asserts that the given bias is between 0 and 7.
 * @param handle Handle to a PCD8544 device.
 * @param bias Bias to set.
 * @return True on success, false otherwise.
 */
bool PCD8544_SetBias(pcd8544_config_t *handle, uint8_t bias);

/**
 * Set the display to (not-)inverted mode.
 * @param handle Handle to a PCD8544 device.
 * @param inverted True for inverted, false for not inverted.
 * @return True on success, false otherwise.
 */
bool PCD8544_SetInverted(pcd8544_config_t *handle, bool inverted);

/**
 * Sets a pixel of the back buffer to black or white.
 * Asserts that x is smaller than PCD8544_LCD_WIDTH.
 * Asserts that y is smaller than PCD8544_LCD_HEIGHT.
 * @param handle Handle to a PCD8544 device.
 * @param x The x coordinate of the pixel.
 * @param y The y coordinate of the pixel.
 * @param color Color to set.
 */
void PCD8544_SetPixel(pcd8544_config_t *handle, uint8_t x, uint8_t y, pcd8544_color_t color);

/**
 * Updates the display by transmitting the back buffer to the device.
 * Must be called after all set pixel operations of the current frame.
 * @param handle Handle to a PCD8544 device.
 * @return True on success, false otherwise.
 */
bool PCD8544_UpdateScreen(pcd8544_config_t *handle);

/**
 * Draws an image from given array (vertical pixels) starting in X,Y
 */
void PCD8544_DrawImage(pcd8544_config_t *handle, uint8_t x, uint8_t y, uint8_t width, uint8_t height, unsigned char* image);

/**
 * Draws line from x1,y1 to x2,y2 with given color
 */
void PCD8544_DrawLine(pcd8544_config_t *handle, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, pcd8544_color_t color);


void PCD8544_WriteString(pcd8544_config_t *handle, uint8_t x, uint8_t y, char* str, uint8_t color);
void PCD8544_WriteChar(pcd8544_config_t *handle, uint8_t x, uint8_t y, char ch, uint8_t color);
void PCD8544_ClearBuffer(pcd8544_config_t *handle);
