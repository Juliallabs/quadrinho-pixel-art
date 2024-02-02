#include <IRCClient.h> 
#include <ESP8266HTTPClient.h>
#include "senhas.h"
#include "twitch.h"
#include "iot_settings.h"
#include "display_pixel.h"
//The name of the channel that you want the bot to join

//tive  que colocar aqui pois não sei como está a estrutura do arquivo senhas.h
//#define TWITCH_BOT_NAME "justinfan112312312345" //usuario justinfan112312312345 é visitante, apenas leitura.
//#define TWITCH_OAUTH_TOKEN ""


uint16_t desenho[1][64];//resolver depois

String ircChannel = "";
String mensagem = "";
WiFiClient wiFiClient;
WiFiClientSecure wifiClient;  // Use WiFiClientSecure para permitir requisições HTTPS


int controle = 0;
IRCClient client(IRC_SERVER, IRC_PORT, wiFiClient);

void sendTwitchMessage(String message) {
  client.sendMessage(ircChannel, message);
}


void pixelrequest(String url){ //Função que faz o request da url encurtada pra receber a resposta
    HTTPClient http;
    wifiClient.setInsecure();
    http.begin(wifiClient, url);

    int httpCode = http.GET();
    if (httpCode > 0) {
      //Serial.printf("[HTTP] Código de resposta: %d\n", httpCode);     
      String payload = http.getString();
      //Serial.println("Resposta:");
      //Serial.println(payload);
      msg="desenho";
      desenhar(payload);//chama a função de enviar o desenho pro quadro
    if(httpCode != 200){
      controle=0; //RESETA SE O CODIGO NAO FOR 200
    }
    } else {
      Serial.printf("[HTTP] Falha na requisição. Código de erro: %d\n", httpCode);
      controle=0; //RESETA SE FALHAR

    }
   
    http.end();
}
 
void callback(IRCMessage ircMessage) {
  //sendTwitchMessage(ircMessage.nick);
  if (ircMessage.nick == "streamlabs") {
      if (ircMessage.text.indexOf("seja bem vindo ao Lab!") > -1){msg="follow";}
      if (ircMessage.text.indexOf("bits!") > -1 ) {msg="bits";}
      if (ircMessage.text.indexOf("mandou um Prime!") > -1){msg="prime";}
      if (ircMessage.text.indexOf("SUB") > -1){msg="sub";}
      if (ircMessage.text.indexOf("gifted") > -1){msg="presente";}
      if (ircMessage.text.indexOf("chegou") > -1){msg="raid";}
      if (ircMessage.text.indexOf("Um usuário anônimo presenteou") > -1){msg="anonimo";}
 
    //sendTwitchMessage("STREAMLABS FALOU");
  }
  if (ircMessage.text == "!mouser") {
    msg="mouser";
  }
  // if pra verificar se a url é encurtada assim https://rerre.net/PixelEditor/beta2/mini.php?url=ABOBAAAA
  
   if (ircMessage.text.indexOf("!editor") > -1){
    sendTwitchMessage("https://rerre.net/PixelEditor/");
  }
  
  if (ircMessage.text.indexOf("!bianca") > -1){
    sendTwitchMessage("https://rerre.net/contato/");
    msg="bianca";
  }

  if (ircMessage.text.indexOf("!reset") > -1){
    msg="reset";
  }
   if (strstr(ircMessage.text.c_str(), "PixelEditor") != nullptr && //verificar se é sobre o pixeleditor
        ircMessage.text.startsWith("http") && //verificar se é uma url
        ircMessage.text.indexOf("url=") != -1 && //verificar se tem o parametro url 
        ircMessage.text.indexOf(' ') == -1) 
    { //verificar se não contem espaços
        Serial.println("Desenho recebido!"); //informa que recebeu e detectou um link valido
        pixelrequest(ircMessage.text);  //chama a função que faz o request na url recebida

    }
  
} 

void setupTWconnection(){
  
  ircChannel = "#" + twitchChannelName;
  client.setCallback(callback);
}

void loopTW(){
if (!client.connected()) {
    Serial.println("Attempting to connect to " + ircChannel );
    // Attempt to connect
    // Second param is not needed by Twtich 
//  if (client.connect(TWITCH_BOT_NAME, "", TWITCH_OAUTH_TOKEN)) {
   if (client.connect(TWITCH_BOT_NAME, "", TWITCH_OAUTH_TOKEN)) {
      client.sendRaw("JOIN " + ircChannel);
      Serial.println("connected and ready to rock");
      sendTwitchMessage(twitchInitializationMessage);
    } else {
      Serial.println("failed... try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
    return;
  }
  client.loop();

}


