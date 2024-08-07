#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <lvgl.h>
#include <ESPAsyncWebServer.h>

// 自定义SPI引脚定义
#define TFT_CS 16
#define TFT_RST 20
#define TFT_DC 19
#define TFT_MOSI 18
#define TFT_SCLK 17
#define TFT_BLK 15

// 屏幕分辨率
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 135

// WiFi 配置
char ssid[] = "wlang";
const char* password = "13750731785";

// 创建显示对象，使用自定义SPI引脚
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// 创建HTTP服务器对象
AsyncWebServer server(80);

// 函数声明
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void handleTextUpload(AsyncWebServerRequest *request);

void setup() {
  // 初始化串口
  Serial.begin(115200);

  // 设置背光引脚为输出模式，并将其设置为低电平
  pinMode(TFT_BLK, OUTPUT);
  digitalWrite(TFT_BLK, LOW);

  // 初始化显示屏
  tft.init(SCREEN_HEIGHT, SCREEN_WIDTH);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);

  // 初始化WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // 初始化LVGL
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

  // 创建一个简单的LVGL对象
  lv_obj_t * label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "Hello, LVGL!");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

  // 初始化HTTP服务器
  server.on("/upload", HTTP_POST, handleTextUpload);

  // 添加一个简单的网页
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", "<html><body><h1>Hello, World!</h1><p>This is a simple web page.</p><form method='POST' action='/upload'><input type='text' name='text'><input type='submit' value='Upload'></form></body></html>");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  lv_timer_handler(); // 处理LVGL任务
  delay(5);
}

// 处理上传的文字数据
void handleTextUpload(AsyncWebServerRequest *request) {
  if (request->hasParam("text", true)) {
    String text = request->getParam("text", true)->value();
    Serial.printf("Received text: %s\n", text.c_str());

    // 在屏幕上显示接收到的文字
    tft.fillScreen(ST77XX_WHITE);
    tft.setTextSize(2); // 设置字体大小
    tft.setTextColor(ST77XX_BLACK);
    tft.setTextWrap(true);

    // 计算文字的宽度和高度
    int16_t x1, y1;
    uint16_t w, h;
    tft.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

    // 居中显示文字
    int16_t x = (SCREEN_WIDTH - w) / 2;
    int16_t y = (SCREEN_HEIGHT - h) / 2;
    tft.setCursor(x, y);
    tft.print(text);

    // 发送响应
    request->send(200, "text/plain", "Upload complete");
  } else {
    request->send(400, "text/plain", "No text received");
  }
}

// LVGL显示刷新回调
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint16_t c;
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);
  for (int y = area->y1; y <= area->y2; y++) {
    for (int x = area->x1; x <= area->x2; x++) {
      c = color_p->full;
      tft.writeColor(c, 1);
      color_p++;
    }
  }
  tft.endWrite();
  lv_disp_flush_ready(disp);
}