#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
//#include "settings.h"
#include "ota.h"
#include "twitch.h"
//#include "imagens.h"



void desenha(int pos) {
  matrix->clear();
  display_rgbBitmap(pos);
  delay(100);
}

void setup() {
  setupOTA();
  setupTWconnection();
  Serial.begin(115200);
  STARTDisplay();
  display_rgbBitmap(0);
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
