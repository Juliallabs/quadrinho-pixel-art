#include "imagens.h"
#include <Adafruit_NeoMatrix.h>
#include "display_pixel.h"
uint16_t desenhoMatrix[1][64];
void desenhar(String desenho){
    //https://rerre.net/PixelEditor/beta/?h=8&w=8&s=25&array=6e3_6e3_ab3_e93_6e3_6e3_6e3_6e3_6e3_d93_f83_f83_f83_6e3_6e3_6e3_6e3_5cf_fda_fda_5cf_fda_6e3_9b3_6e3_fda_fda_fda_fda_6e3_a83_8c3_6e3_6e3_f83_f83_f83_b73_d53_8b3_6e3_fda_f83_fd9_f83_fda_e43_6e3_6e3_6e3_f83_fc9_f83_e53_8b3_6e3_6e3_6e3_fda_6e3_fd9_6e3_6e3_6e3
    //limpa();
    
    if (desenho.indexOf("array=") > -1){
      desenho=desenho.substring(desenho.indexOf("array=")+5);
    }
    
    desenho.replace("_","0x");
    desenho.replace("=","0x");
    String a = "";
    
    int x=0;
    for (int i = 0 ;i<desenho.length() ; i=i+5){
      a = desenho.substring(i+2,i+5);
      desenhoMatrix[0][x]=(uint16_t)strtoul(a.c_str(), NULL, 16);
      x++;
    }
   fixdrawRGBBitmap(0, 0, desenhoMatrix[0], mw, mh);
}


void fixdrawRGBBitmap(int16_t x, int16_t y, const uint16_t *bitmap, int16_t w, int16_t h) {
  uint16_t RGB_bmp_fixed[w * h];
  for (uint16_t pixel = 0; pixel < w * h; pixel++) {
    uint8_t r, g, b;
    uint16_t color = pgm_read_word(bitmap + pixel);
    
    //Serial.print(color, HEX);
    r = (color & 0xF00) >> 8;
    g = (color & 0x0F0) >> 4;
    b = color & 0x00F;

    //essa parte aqui a IA me explicou que
    //ela mudava o range das cores
    //muda o range de 0 a 15 pra 0 a 31
    //balanceamento das cores
    b = map(b, 0, 15, 0, 22);
    g = map(g, 0, 15, 0, 31);
    r = map(r, 0, 15, 0, 21);

    RGB_bmp_fixed[pixel] = (r << 11) + (g << 5) + b;
  }
  matrix->drawRGBBitmap(x, y, RGB_bmp_fixed, w, h);
  matrix->show();
}


String clear = "https://rerre.net/PixelEditor/?h=8&w=8&s=50&array=000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000_000";
void limpa(){
  desenhar(clear);
}

void STARTDisplay() {

  matrix->begin();
  matrix->setTextWrap(false);
  matrix->setBrightness(BRIGHTNESS);
  matrix->clear();
}