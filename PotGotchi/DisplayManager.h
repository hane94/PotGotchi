#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H
#define TFT_CREAM 0xF6DA
#define TFT_WARMGRAY 0x9C91

#include <Arduino.h>
#include "ST7789_ESP32C3.h"
#include "Sprites.h"

class DisplayManager {
private:
  ST7789_ESP32C3 tft;
  ST7789_Sprite catBuffer;
public:
  DisplayManager();
  void begin();

  void forceFullRedraw(int moisture, int sunlight, int growth, int growthStage, int weedCount);
  void clearScreen();
  void drawMainScreen(int moisture, int sunlight, int growth, int growthStage);
  void clearTrail(int oldX, int newX, int y, int width, int height);

  void drawTransparentImage(int x, int y, int width, int height, const uint16_t* frame, uint16_t transparentColor);
  void drawSprite(int x, int y, int width, int height, const uint16_t* frame);
  void drawSpriteFlipped(int x, int y, int width, int height, const uint16_t* frame);

  void drawWeeds(int count);
  void drawMenu(int selectedIndex);
  void drawGameOver(int selectedIndex);
  void drawResetConfirm(int selectedIndex);
  void drawMinigameUI(int score, int timeLeft, int treatX, int treatY, int oldTreatX, int oldTreatY);

  void drawMinigameTopBar(int score, int timeLeft);
  void updateMinigameTreat(int treatX, int treatY, int oldTreatX, int oldTreatY);
};

#endif