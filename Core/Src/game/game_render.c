#include "game_render.h"

void RENDER_DrawStat(pcd8544_config_t* lcd, uint8_t idx, unsigned char* image, float val)
{
	uint8_t width = 11;
	uint8_t height = 10;

	PCD8544_DrawLine(lcd, (PCD8544_LCD_WIDTH - 1) - width - 1, (height+1) * (idx), (PCD8544_LCD_WIDTH - 1), (height+1) * (idx), 1);
	PCD8544_DrawLine(lcd, (PCD8544_LCD_WIDTH - 1) - width - 1, (height+1) * (idx+1), (PCD8544_LCD_WIDTH - 1), (height+1) * (idx+1), 1);

	PCD8544_DrawLine(lcd, (PCD8544_LCD_WIDTH - 1) - width - 1, (height + 1) * idx, (PCD8544_LCD_WIDTH - 1) - width - 1, (height + 1) * (idx + 1), 1);
	PCD8544_DrawLine(lcd, (PCD8544_LCD_WIDTH - 1), (height + 1) * idx, (PCD8544_LCD_WIDTH - 1), (height + 1) * (idx + 1), 1);

	PCD8544_UpdateScreen(lcd);
	for (uint8_t x_idx = 0; x_idx < width; x_idx++)
	{
		for (uint8_t y_idx = 0; y_idx < height; y_idx++)
		{
			uint8_t bit = (image[x_idx + (y_idx / 8) * width] >> y_idx % 8) & 1;

			uint8_t v = 0;
			float war = round((float) (1+y_idx) / (float) (1+height) * 100.0) / 100.0;
			PCD8544_SetPixel(lcd, 83-width + x_idx, 1 * (idx+1) + height*idx + y_idx, 1 - war > val ? bit : !bit);
		}
	}
}
