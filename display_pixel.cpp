#include "imagens.h"
#include <Adafruit_NeoMatrix.h>
#include "display_pixel.h"



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


void display_rgbBitmap(uint8_t bmp_num) {
  fixdrawRGBBitmap(0, 0, RGB_bmp[bmp_num], mw, mh);
 // matrix->drawRGBBitmap(0, 0, RGB_bmp[bmp_num], mw, mh);
  matrix->show();

}

void limpa(){
  display_rgbBitmap(10);
}

void STARTDisplay() {

  matrix->begin();
  matrix->setTextWrap(false);
  matrix->setBrightness(BRIGHTNESS);
  matrix->clear();
}