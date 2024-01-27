// ota_manager.h

#ifndef OTA_MANAGER_H
#define OTA_MANAGER_H

#include <ArduinoOTA.h>
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

extern const char* HostName;


void wmConfig();
void setupOTA();

#endif // OTA_MANAGER_H
