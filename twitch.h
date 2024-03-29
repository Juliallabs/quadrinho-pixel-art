#ifndef TWITCH_H
#define TWITCH_H
#include <IRCClient.h>
#include <ESP8266HTTPClient.h>
#include "senhas.h"
#include "iot_settings.h"
#include "display_pixel.h"
#include <ESP8266WebServer.h>
#define IRC_SERVER   "irc.chat.twitch.tv"
#define IRC_PORT     6667


extern int controle;
extern const String twitchChannelName;
extern const String twitchInitializationMessage;

extern String msg;
extern IRCClient client;
extern ESP8266WebServer server;
extern WiFiClient wiFiClient;
extern WiFiClientSecure wifiClient;
extern const String TWITCH_BOT_NAME;

void sendTwitchMessage(String message);
void callback(IRCMessage ircMessage);
void setupTWconnection();
void loopTW();

#endif // TWITCH_BOT_H
