#include <IRCClient.h> 
#include "senhas.h"
#include "twitch.h"
#include "iot_settings.h"
#include "display_pixel.h"
//The name of the channel that you want the bot to join


uint16_t desenho[1][64];//resolver depois

String ircChannel = "";
String mensagem = "";
WiFiClient wiFiClient;
ESP8266WebServer server(80);

int controle = 0;
IRCClient client(IRC_SERVER, IRC_PORT, wiFiClient);

void sendTwitchMessage(String message) {
  client.sendMessage(ircChannel, message);
}
 
void callback(IRCMessage ircMessage) {
  if (ircMessage.nick == "Streamlabs") {
    sendTwitchMessage("STREAMLABS FALOU");
  }


  if (ircMessage.text == "!mouser") {
    sendTwitchMessage("FUNCIONAAAAAAAAAAA!");
    controle=1;

  }
  
  if (ircMessage.text.indexOf("has cheered") > -1 && ircMessage.nick == "Streamlabs"){ //donate
    controle=2;

  }
  if (ircMessage.text.indexOf("seja bem vindo ao Lab!") > -1 && ircMessage.nick == "Streamlabs"){ // segue
    controle=3;

  }

    if (ircMessage.text.indexOf("mandou um Prime!") > -1 && ircMessage.nick == "Streamlabs"){ //inscreveu com prime
    controle=4;

  }

  
    if (ircMessage.text.indexOf("!fofao") > -1){
    controle=5;

  }

    if (ircMessage.text.indexOf("Um usuário anônimo presenteou") > -1 && ircMessage.nick == "Streamlabs") { // anonimo
    controle=6;

  }

  
   if (ircMessage.text.indexOf("!aboba") > -1){
    controle=7;

  }

  
  if (ircMessage.text.indexOf("!bianca") > -1){
    sendTwitchMessage("https://rerre.net/contato/");
    controle=8;
  }

  if (ircMessage.text.indexOf("!reset") > -1){
    controle=0;
  }

  if (ircMessage.text.indexOf("OI") > -1 && ircMessage.nick == "JULIALABS")
  {

  }
  if (ircMessage.text.startsWith("https://rerre.net")){
    controle=9;
    matrix->clear();
    // https://rerre.net/PixelEditor/beta/?h=8&w=8&s=25&array=6e3_6e3_ab3_e93_6e3_6e3_6e3_6e3_6e3_d93_f83_f83_f83_6e3_6e3_6e3_6e3_5cf_fda_fda_5cf_fda_6e3_9b3_6e3_fda_fda_fda_fda_6e3_a83_8c3_6e3_6e3_f83_f83_f83_b73_d53_8b3_6e3_fda_f83_fd9_f83_fda_e43_6e3_6e3_6e3_f83_fc9_f83_e53_8b3_6e3_6e3_6e3_fda_6e3_fd9_6e3_6e3_6e3
    mensagem=ircMessage.text;
    limpa();
    mensagem.remove(0,54);
    mensagem.replace("_","0x");
    mensagem.replace("=","0x");
    String a = "";
    char *end;
    //"0x6E3"
    //Serial.println(ircMessage.text);
    //chat[0].c_str(ircMessage.text);
    
    int x=0;
    for (int i = 0 ;i<mensagem.length() ; i=i+5){
      a = mensagem.substring(i+2,i+5);
      desenho[0][x]=(uint16_t)strtoul(a.c_str(), NULL, 16);
      x++;
      Serial.println((uint16_t)strtoul(a.c_str(), NULL, 16));
    }
    fixdrawRGBBitmap(0, 0, desenho[0], 8, 8);
    
  }
 
    return;
  
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
