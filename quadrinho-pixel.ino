#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoOTA.h>
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include "display_pixel.h"
#include "iot_settings.h"
#include "twitch.h"
#include "imagens.h"
#include "webserver.h"
ESP8266WebServer server(80);
String msg = "";
const int PIN = 2; // pino do microcontrolador conectado a matriz pixel
const int BRIGHTNESS = 120;  // Valor máximo de 255, 30 foi escolhido para manter boa visualização no display e gastar menos energia (500mA)

const int mw = 8;  //define largura da matriz
const int mh = 8; //define altura da matiz
const int area = mw*mh; // esta variável nao esta funcionando no twitch.cpp
//cria matriz de led 
Adafruit_NeoMatrix *matrix = new Adafruit_NeoMatrix(mw, mh, PIN,NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG, NEO_GRB + NEO_KHZ800);
//The name that you want the bot to have
const String TWITCH_BOT_NAME = "julialabs";
const String twitchChannelName = "julialabs";
const String twitchInitializationMessage = "QUADRINHO PIXEL ON";
const char* HostName = "pixel";



void setup() {
  Serial.begin(115200);
  wmConfig(); // funcao descrita no arquivo iot_settings.cpp
  setupOTA(); // funcao descrita no arquivo iot_settings.cpp
  setupWebServer();
  setupTWconnection();// funcao descrita no arquivo twitch.cpp
  STARTDisplay();// funcao descrita no arquivo display_pixel.cpp
  desenhar(imagens[0]); // funcao descrita no arquivo display_pixel.cpp
  delay(500);
  msg="init";
}



void loop() {
  ArduinoOTA.handle();
  loopTW();
  loopserver();
  if(msg=="follow"){
    for(int i = 0; i<3 ; i++){
      Serial.print("FOLLOW");
      desenhar(follow[0]);
      delay(100);
      desenhar(follow[1]);
      delay(100);
      desenhar(follow[2]);
      delay(100);
    }
    msg="reset";

    return;}
 // if(msg=="bits"){ desenhar(..) return;}
  //if(msg=="prime"){ desenhar(..) return;}
  //if(msg=="sub"){ desenhar(..) return;}
  //if(msg=="presente"){ desenhar(..) return;}
  //if(msg=="raid"){ desenhar(..) return;}
  //if(msg=="anonimo"){ desenhar(..) return;}
    if(msg=="") {desenhar(imagens[0]); return;}
    if(msg=="pituxo") {       
      desenhar(imagens[1]);
      delay(100);
      desenhar(imagens[2]);
      delay(100);
      desenhar(imagens[3]);
      delay(100);
      }

    
    

  
}
