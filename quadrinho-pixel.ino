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

void desenha(int pos) {
  matrix->clear();
  display_rgbBitmap(pos);
  delay(100);
}

void setup() {
  Serial.begin(115200);
  wmConfig(); // funcao descrita no arquivo iot_settings.cpp
  setupOTA(); // funcao descrita no arquivo iot_settings.cpp
  setupTWconnection();// funcao descrita no arquivo twitch.cpp
  STARTDisplay();// funcao descrita no arquivo display_pixel.cpp
  display_rgbBitmap(2); // funcao descrita no arquivo display_pixel.cpp
  delay(500);
}



void loop() {
  ArduinoOTA.handle();
  loopTW();

  switch (controle) {
    case 0: 
    for (int i = 0 ;i<= 5; i++){
      desenha(i);
      delay(100);
    }
    break;
    case 1://mouser

      break;
    //----------------------
    case 2: //donate

      break;
    //----------------------
    case 3: //seguiu o canal

      break;
    //----------------------
    case 4: // inscreveu com prime

      break;
    //----------------------
    case 5: //foifao

      break;
      //----------------------

    case 6: // presente anonimo

      break;
    //----------------------
    case 7: // PIKACHU
    desenha(7);
    delay(5000);
    controle=0;

      break;
    //----------------------
    case 8: //bianca
    desenha(8);
    delay(5000);
    controle=0;
      break;
      //----------------------
    case 9:
      break;
      //----------------------
    case 10:

      break;
    //----------------------
    default:
    for (int i = 0 ;i<= 5; i++){
      desenha(i);
      delay(100);
    }
      break;

  }
}
