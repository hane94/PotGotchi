#include <ST7789_ESP32C3.h>
#include <User_Setup.h>
#include <gfxfont.h>
#include <glcdfont.h>
#include <transaction.h>

#include "DisplayManager.h"

DisplayManager::DisplayManager()
  : catBuffer(&tft) {
}

void DisplayManager::begin() {
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_CREAM);

  catBuffer.createSprite(120, 120);
}

void DisplayManager::forceFullRedraw(int moisture, int sunlight, int growth, int growthStage, int weedCount) {
  tft.fillScreen(TFT_CREAM);
  drawMainScreen(moisture, sunlight, growth, growthStage);
  drawWeeds(weedCount);
}

void DisplayManager::clearScreen() {
  tft.fillScreen(TFT_CREAM);
}

void DisplayManager::drawMainScreen(int moisture, int sunlight, int growth, int growthStage) {
  tft.fillRect(0, 0, 280, 70, TFT_CREAM);
  tft.setTextSize(2);

  if (moisture <= 20 || sunlight <= 20) {
    tft.setTextColor(TFT_RED);
  } else {
    tft.setTextColor(TFT_WARMGRAY);
  }

  tft.setCursor(40, 5);
  tft.printf("Water:%d", moisture);

  tft.setCursor(155, 5);
  tft.printf("Sun:%d", sunlight);
  //%d는 숫자를 끼워넣으라는 자리표시자. 값은 그 뒤에 따라오는 moisture와 sunlight

  tft.setTextColor(TFT_WARMGRAY);
  tft.setCursor(5, 30);
  tft.print("Growth:");

  tft.drawRect(95, 30, 175, 16, TFT_WARMGRAY);

  int stageProgress = growth % 20;
  int fillWidth;

  if (growth >= 100) {
    fillWidth = 171;
  } else {
    fillWidth = (171 * stageProgress) / 20;
  }

  if (fillWidth > 0) {
    tft.fillRect(97, 32, fillWidth, 12, 0xE14F);
  }

  tft.setCursor(5, 52);
  if (growthStage == 0) {
    tft.print("Stage: Seed");
  } else if (growthStage == 1) {
    tft.print("Stage: Sprout");
  } else if (growthStage == 2) {
    tft.print("Stage: Stem");
  } else if (growthStage == 3) {
    tft.print("Stage: Bud");
  } else {
    tft.print("Stage: Flower");
  }
}

void DisplayManager::clearTrail(int oldX, int newX, int y, int width, int height) {
  if (newX > oldX) {
    tft.fillRect(oldX, y, newX - oldX, height, TFT_CREAM);
  } else if (newX < oldX) {
    tft.fillRect(newX + width, y, oldX - newX, height, TFT_CREAM);
  }
}

void DisplayManager::drawTransparentImage(int x, int y, int width, int height, const uint16_t* frame, uint16_t transparentColor) {
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      uint16_t color = pgm_read_word(&frame[row * width + col]);
      if (color != transparentColor) {
        tft.drawPixel(x + col, y + row, color);
      }
    }
  }
}

void DisplayManager::drawSprite(int x, int y, int width, int height, const uint16_t* frame) {
  catBuffer.fillSprite(TFT_CREAM);

  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      uint16_t color = pgm_read_word(&frame[row * width + col]);
      if (color != TFT_BLACK) {
        catBuffer.drawPixel(col, row, color);
      }
    }
  }
  catBuffer.pushSprite(x, y);
}

void DisplayManager::drawSpriteFlipped(int x, int y, int width, int height, const uint16_t* frame) {
  catBuffer.fillSprite(TFT_CREAM);

  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      uint16_t color = pgm_read_word(&frame[row * width + col]);
      if (color != TFT_BLACK) {
        catBuffer.drawPixel(width - 1 - col, row, color);
      }
    }
  }
  catBuffer.pushSprite(x, y);
}

void DisplayManager::drawWeeds(int count) {
  int weedX[] = { 10, 45, 210, 245 };
  int weedY = 195;
  for (int i = 0; i < count; i++) {
    drawTransparentImage(weedX[i], weedY, 20, 20, weed_frame, TFT_BLACK);
  }
}

void DisplayManager::drawMenu(int selectedIndex) {
  const char* menuItems[] = { "Water", "Sunlight", "Clean", "Reset", "Exit" };

  tft.fillRoundRect(40, 40, 200, 165, 10, TFT_BLACK);
  tft.drawRoundRect(40, 40, 200, 165, 10, TFT_WHITE);
  tft.setTextSize(2);

  for (int i = 0; i < 5; i++) {
    if (i == selectedIndex) {
      tft.setTextColor(TFT_CREAM);
      tft.setCursor(60, 55 + (i * 25));
      tft.print("> ");
    } else {
      tft.setTextColor(TFT_WHITE);
      tft.setCursor(60, 55 + (i * 25));
      tft.print("  ");
    }
    tft.println(menuItems[i]);
  }
}

void DisplayManager::drawGameOver(int selectedIndex) {
  tft.fillRoundRect(30, 70, 220, 100, 10, TFT_BLACK);
  tft.drawRoundRect(30, 70, 220, 100, 10, TFT_WHITE);
  tft.setTextSize(2);

  tft.setTextColor(TFT_RED);
  tft.setCursor(80, 85);
  tft.print("R.I.P.");

  const char* options[] = { "Restart", "Leave" };

  for (int i = 0; i < 2; i++) {
    if (i == selectedIndex) {
      tft.setTextColor(TFT_CREAM);
      tft.setCursor(60, 115 + (i * 25));
      tft.print("> ");
    } else {
      tft.setTextColor(TFT_WHITE);
      tft.setCursor(60, 115 + (i * 25));
      tft.print("  ");
    }
    tft.println(options[i]);
  }
}

void DisplayManager::drawResetConfirm(int selectedIndex) {
  tft.fillRoundRect(30, 70, 220, 100, 10, TFT_BLACK);
  tft.drawRoundRect(30, 70, 220, 100, 10, TFT_WHITE);
  tft.setTextSize(2);

  tft.setTextColor(TFT_RED);
  tft.setCursor(50, 85);
  tft.print("Reset plant?");

  const char* options[] = { "No", "Yes" };

  for (int i = 0; i < 2; i++) {
    if (i == selectedIndex) {
      tft.setTextColor(TFT_CREAM);
      tft.setCursor(60, 115 + (i * 25));
      tft.print("> ");
    } else {
      tft.setTextColor(TFT_WHITE);
      tft.setCursor(60, 115 + (i * 25));
      tft.print("  ");
    }
    tft.println(options[i]);
  }
}

void DisplayManager::drawMinigameUI(int score, int timeLeft, int treatX, int treatY, int oldTreatX, int oldTreatY) {
  if (oldTreatY > 0) {
    tft.fillRect(oldTreatX, oldTreatY, 20, 20, TFT_CREAM);
  }

  tft.fillRect(0, 0, 280, 30, TFT_CREAM);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WARMGRAY);
  tft.setCursor(10, 5);
  tft.printf("Score:%d  Time:%ds", score, timeLeft);

  if (treatY > 0) {
    drawTransparentImage(treatX, treatY, 20, 20, sunlight_frame, TFT_BLACK);
  }
}

void DisplayManager::drawMinigameTopBar(int score, int timeLeft) {
  tft.fillRect(0, 0, 280, 30, TFT_CREAM);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WARMGRAY);
  tft.setCursor(10, 5);
  tft.printf("Score:%d  Time:%ds", score, timeLeft);
}

void DisplayManager::updateMinigameTreat(int treatX, int treatY, int oldTreatX, int oldTreatY) {
  if (oldTreatY > 0) {
    tft.fillRect(oldTreatX, oldTreatY, 20, 20, TFT_CREAM);
  }
  if (treatY > 0) {
    drawTransparentImage(treatX, treatY, 20, 20, sunlight_frame, TFT_BLACK);
  }
}