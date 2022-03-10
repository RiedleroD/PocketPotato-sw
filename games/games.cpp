//defining functions for the games
extern void drawTexture(const uint8_t _x,const uint8_t _y,const uint8_t texture[],const uint8_t width,const uint8_t height);
extern void drawZoomedPixel(const uint8_t x,const uint8_t y,uint8_t zoom);
extern void printFromFlash(char* text);

#include "Snake.cpp"
#include "buttontest.cpp"
#include "testgame.cpp"
#include "eepromtest.cpp"
