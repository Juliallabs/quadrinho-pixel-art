// webserver.h

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "twitch.h"
#include <ESP8266WebServer.h>
#include "iot_settings.h"
#include "display_pixel.h"

extern const char* HostName;


void loopserver();
void setupWebServer();

#endif // 
