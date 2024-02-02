// display_functions.h

#ifndef DISPLAY_FUNCTIONS_H
#define DISPLAY_FUNCTIONS_H

#include "imagens.h"
#include <Adafruit_NeoMatrix.h>
extern const int PIN;
extern const int BRIGHTNESS;
extern const int mw;
extern const int mh;
extern const int area;


extern Adafruit_NeoMatrix *matrix;


void fixdrawRGBBitmap(int16_t x, int16_t y, const uint16_t *bitmap, int16_t w, int16_t h);
void display_rgbBitmap(uint8_t bmp_num);
void limpa();
void desenhar(String desenho);
void STARTDisplay();

#endif // DISPLAY_FUNCTIONS_H
