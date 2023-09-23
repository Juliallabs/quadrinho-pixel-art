 #include "imagens.h"


#define PIN 2 // pino do microcontrolador conectado a matriz pixel

#define BRIGHTNESS 255  // Valor máximo de 255, 30 foi escolhido para manter boa visualização no display e gastar menos energia (500mA)

#define mw 8 //define largura da matriz
#define mh 8 //define altura da matiz

//cria matriz de led 
Adafruit_NeoMatrix *matrix = new Adafruit_NeoMatrix(mw, mh, PIN,NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG, NEO_GRB + NEO_KHZ800);


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

void STARTDisplay() {

  matrix->begin();
  matrix->setTextWrap(false);
  matrix->setBrightness(BRIGHTNESS);
  matrix->clear();
}