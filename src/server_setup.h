#ifndef SERVER_SETUP_H
#define SERVER_SETUP_H

#include <ESPAsyncWebServer.h>

extern AsyncWebServer server;

void initServer();
void handleTextUpload(AsyncWebServerRequest *request);
void handleImageUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final);

#endif // SERVER_SETUP_H