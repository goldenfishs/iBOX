#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

void initWiFi(const char* ssid, const char* password, const char* ssid_self, const char* password_self);
String getExternalIP();

#endif // WIFI_SETUP_H