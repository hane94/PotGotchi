#include "HardwareConfig.h"
#include "PetState.h"
#include "DisplayManager.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "CharacterManager.h"
enum GameState { STATE_MAIN,
                 STATE_MENU,
                 STATE_DEAD,
                 STATE_MINIGAME };

GameState currentState = STATE_MAIN;
PetState pet;
DisplayManager display;
InputManager input;
SoundManager sound;
CharacterManager cat;

int lastMoisture = -1;
int lastSunlight = -1;
int lastGrowth = -1;
int lastCatX = 100;
int lastWeedCount = -1;
int mgTreatX = 140;
int mgTreatY = 0;
int mgOldTreatX = 140;
int mgOldTreatY = 0;
int mgScore = 0;
int menuSelection = 0;
int mgLastScore = -1;
int mgLastTime = -1;

const int NUM_MENU_ITEMS = 4;

uint32_t mgStartTime = 0;

void setup() {
  Serial.begin(115200);

  input.begin();
  sound.begin();
  pet.begin();
  display.begin();
  cat.begin();

  display.forceFullRedraw(pet.getMoisture(), pet.getSunlight(), pet.getGrowth(), pet.getGrowthStage(), pet.getWeedCount());

  sound.playHappyTone();
}

void loop() {
  if (currentState == STATE_MAIN) {
    pet.update();

    if (pet.isDead()) {
      cat.setPlantState(pet.getGrowthStage(), false);
      cat.setAction(DEAD);
      currentState = STATE_DEAD;
      menuSelection = 0;
      sound.playSadTone();
      display.forceFullRedraw(pet.getMoisture(), pet.getSunlight(), pet.getGrowth(), pet.getGrowthStage(), pet.getWeedCount());
      display.drawGameOver(menuSelection);
      return;
    }

    bool isCritical = (pet.getMoisture() <= 20 || pet.getSunlight() <= 20);
    cat.setPlantState(pet.getGrowthStage(), isCritical);

    cat.update(display);

    if (cat.getX() != lastCatX) {
      display.clearTrail(lastCatX, cat.getX(), cat.getY(), 120, 120);
      lastCatX = cat.getX();
      display.drawWeeds(pet.getWeedCount());
    }

    if (pet.getWeedCount() != lastWeedCount) {
      lastWeedCount = pet.getWeedCount();
      display.drawWeeds(pet.getWeedCount());
    }

    if (pet.getMoisture() != lastMoisture || pet.getSunlight() != lastSunlight || pet.getGrowth() != lastGrowth) {
      lastMoisture = pet.getMoisture();
      lastSunlight = pet.getSunlight();
      lastGrowth = pet.getGrowth();
      display.drawMainScreen(pet.getMoisture(), pet.getSunlight(), pet.getGrowth(), pet.getGrowthStage());
    }

    if (input.isOkPressed()) {
      sound.playClick();
      currentState = STATE_MENU;
      menuSelection = 0;
      display.drawMenu(menuSelection);
    }

    if (input.isLovePressed()) {
      bool grew = pet.canLove();
      pet.love();
      cat.setAction(PLAYING);
      if (grew) {
        sound.playHappyTone();
      }
    }
  } else if (currentState == STATE_MENU) {
    if (input.isLeftPressed()) {
      sound.playClick();
      menuSelection--;
      if (menuSelection < 0) menuSelection = NUM_MENU_ITEMS - 1;
      display.drawMenu(menuSelection);
    }

    if (input.isRightPressed()) {
      sound.playClick();
      menuSelection++;
      if (menuSelection >= NUM_MENU_ITEMS) menuSelection = 0;
      display.drawMenu(menuSelection);
    }

    if (input.isOkPressed()) {
      sound.playClick();
      if (menuSelection == 0) {
          pet.feed();
          cat.setAction(EATING);
          currentState = STATE_MAIN;
        } else if (menuSelection == 1) {
          currentState = STATE_MINIGAME;
          cat.setAction(MINIGAME);
          mgScore = 0;
          mgStartTime = millis();
          mgTreatY = 20;
          mgTreatX = random(20, 260);
          display.clearScreen();
        } else if (menuSelection == 2) {
          pet.clean();
          sound.playHappyTone();
          currentState = STATE_MAIN;
        } else if (menuSelection == 3) {
          currentState = STATE_MAIN;
        }
      if (currentState == STATE_MAIN) {
        display.forceFullRedraw(pet.getMoisture(), pet.getSunlight(), pet.getGrowth(), pet.getGrowthStage(), pet.getWeedCount());
        lastMoisture = -1;
        lastWeedCount = -1;
        lastCatX = cat.getX();
      }
    }
  }

  else if (currentState == STATE_MINIGAME) {
    int timeLeft = 15 - ((millis() - mgStartTime) / 1000);

    if (mgScore != mgLastScore || timeLeft != mgLastTime) {
      mgLastScore = mgScore;
      mgLastTime = timeLeft;
      display.drawMinigameTopBar(mgScore, timeLeft);
    }
    if (timeLeft <= 0) {
      sound.playHappyTone();
      if (mgScore >= 4) {
        pet.play(20);
      } else {
        pet.play(5);
      }
      cat.setX(80);
      cat.setAction(PLAYING);
      currentState = STATE_MAIN;
      display.forceFullRedraw(pet.getMoisture(), pet.getSunlight(), pet.getGrowth(), pet.getGrowthStage(), pet.getWeedCount());

      lastMoisture = -1;
      lastCatX = cat.getX();
      mgLastScore = -1;
      mgLastTime = -1;
      return;
    }

    int oldCatX = cat.getX();
    int mgStepSpeed = 14;

    if (input.isLeftPressed()) {
      cat.setX(cat.getX() - mgStepSpeed);
      cat.setFacingRight(false);
    } else if (input.isRightPressed()) {
      cat.setX(cat.getX() + mgStepSpeed);
      cat.setFacingRight(true);
    }

    if (oldCatX != cat.getX()) {
      display.clearTrail(oldCatX, cat.getX(), cat.getY(), 120, 120);
    }

    mgOldTreatX = mgTreatX;
    mgOldTreatY = mgTreatY;

    int dropSpeed = 2;
    mgTreatY += dropSpeed;

    if (mgTreatY > 145 && mgTreatY < 200) {
      if (abs((mgTreatX + 8) - (cat.getX() + 60)) < 40) {
        mgScore++;
        sound.playClick();
        mgTreatY = 20;
        mgTreatX = random(20, 260);
      }
    } else if (mgTreatY > 240) {
      mgTreatY = 20;
      mgTreatX = random(20, 260);
    }

    display.updateMinigameTreat(mgTreatX, mgTreatY, mgOldTreatX, mgOldTreatY);
    cat.update(display);
  }

  else if (currentState == STATE_DEAD) {
    
    if (input.isLeftPressed() || input.isRightPressed()) {
      sound.playClick();
      menuSelection = (menuSelection == 0) ? 1 : 0;
      display.drawGameOver(menuSelection);
    }
    if (input.isOkPressed()) {
      sound.playClick();
      if (menuSelection == 0) {
        pet.reset();
        cat.setAction(IDLE);
        currentState = STATE_MAIN;
        display.forceFullRedraw(pet.getMoisture(), pet.getSunlight(), pet.getGrowth(), pet.getGrowthStage(), pet.getWeedCount());
        lastMoisture = -1;
        lastWeedCount = -1;
        lastCatX = cat.getX();
      } else if (menuSelection == 1) {
        display.drawGameOver(menuSelection);
      }
    }
  }
  delay(10);
}