#ifndef DISPLAY_SETUP_H
#define DISPLAY_SETUP_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <lvgl.h>

extern Adafruit_ST7789 tft;

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 135

void initDisplay();
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

#endif // DISPLAY_SETUP_H