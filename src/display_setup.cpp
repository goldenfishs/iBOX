#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <lvgl.h>
#include <Arduino.h>
#include "display_setup.h"

#define TFT_CS 16
#define TFT_RST 20
#define TFT_DC 19
#define TFT_MOSI 18
#define TFT_SCLK 17
#define TFT_BLK 15

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void initDisplay() {
  pinMode(TFT_BLK, OUTPUT);
  digitalWrite(TFT_BLK, LOW);

  tft.init(SCREEN_HEIGHT, SCREEN_WIDTH);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);

  lv_init();
  lv_disp_draw_buf_t draw_buf;
  static lv_color_t buf[SCREEN_WIDTH * 10];
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, SCREEN_WIDTH * 10);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = SCREEN_WIDTH;
  disp_drv.ver_res = SCREEN_HEIGHT;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  lv_obj_t *label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "Initialising...");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);

  // 使用 pushColor 逐个像素传输数据
  for (int y = area->y1; y <= area->y2; y++) {
    for (int x = area->x1; x <= area->x2; x++) {
      tft.pushColor(color_p->full);
      color_p++;
    }
  }

  tft.endWrite();
  lv_disp_flush_ready(disp);
}