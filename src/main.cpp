#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <lvgl.h>
#include <ESPAsyncWebServer.h>
#include <PNGdec.h>
#include <SPIFFS.h> // 添加SPIFFS库

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

// PNG解码器对象
PNG png;

// 函数声明
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void handleUpload(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
void *pngOpen(const char *filename, int32_t *size);
void pngClose(void *handle);
int32_t pngRead(PNGFILE *pngFile, uint8_t *buffer, int32_t length);
int32_t pngSeek(PNGFILE *pngFile, int32_t position);
int32_t pngTell(PNGFILE *pngFile);
void PNGDraw(PNGDRAW *pDraw);

void setup() {
  // 初始化串口
  Serial.begin(115200);

  // 设置背光引脚为输出模式，并将其设置为低电平
  pinMode(TFT_BLK, OUTPUT);
  digitalWrite(TFT_BLK, LOW);

  // 初始化显示屏
  tft.init(SCREEN_WIDTH, SCREEN_HEIGHT);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_RED);

  // 初始化WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

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

  // 初始化SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // 初始化HTTP服务器
  server.on("/upload", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, handleUpload);

  // 添加一个简单的网页
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", "<html><body><h1>Hello, World!</h1><p>This is a simple web page.</p><form method='POST' action='/upload' enctype='multipart/form-data'><input type='file' name='image'><input type='submit' value='Upload'></form></body></html>");
  });

  server.begin();
}

void loop() {
  lv_timer_handler(); // 处理LVGL任务
  delay(5);
}

// 处理上传的图像数据
void handleUpload(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  static uint8_t imageData[SCREEN_WIDTH * SCREEN_HEIGHT * 2]; // 假设图像数据为RGB565格式
  static File uploadFile;

  if (index == 0) {
    Serial.printf("UploadStart: %s\n", request->url().c_str());
    uploadFile = SPIFFS.open("/upload.png", FILE_WRITE);
  }

  // 将接收到的数据写入文件
  if (uploadFile) {
    uploadFile.write(data, len);
  }

  if (index + len == total) {
    Serial.printf("UploadEnd: %s, %u B\n", request->url().c_str(), total);
    if (uploadFile) {
      uploadFile.close();
    }

    // 解码PNG文件并显示
    if (SPIFFS.exists("/upload.png")) {
      int16_t rc = png.open("/upload.png", pngOpen, pngClose, pngRead, pngSeek, PNGDraw);
      if (rc == PNG_SUCCESS) {
        rc = png.decode(NULL, 0);
        if (rc == PNG_SUCCESS) {
          Serial.println("PNG decoded successfully");
        } else {
          Serial.printf("PNG decode error: %d\n", rc);
        }
      } else {
        Serial.printf("PNG open error: %d\n", rc);
      }
    }

    request->send(200, "text/plain", "Upload complete");
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

// PNG解码回调函数
void PNGDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[SCREEN_WIDTH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  tft.startWrite();
  tft.setAddrWindow(pDraw->iXOffset, pDraw->iYOffset, pDraw->iWidth, 1);
  tft.writePixels(lineBuffer, pDraw->iWidth);
  tft.endWrite();
}

// 文件操作回调函数
File pngFile;
void *pngOpen(const char *filename, int32_t *size) {
  pngFile = SPIFFS.open(filename);
  if (pngFile) {
    *size = pngFile.size();
    return &pngFile;
  }
  return NULL;
}

void pngClose(void *handle) {
  File *file = static_cast<File*>(handle);
  if (file) {
    file->close();
  }
}

int32_t pngRead(PNGFILE *pngFile, uint8_t *buffer, int32_t length) {
  return pngFile->read(buffer, length);
}

int32_t pngSeek(PNGFILE *pngFile, int32_t position) {
  return pngFile->seek(position);
}

int32_t pngTell(PNGFILE *pngFile) {
  return pngFile->position();
}