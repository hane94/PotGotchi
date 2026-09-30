#include "CharacterManager.h"
#include "Sprites.h"

const uint16_t* healthyFrames[5] = { healthy_stage_0, healthy_stage_1, healthy_stage_2, healthy_stage_3, healthy_stage_4 };
const uint16_t* dangerFrames[5] = { danger_stage_0, danger_stage_1, danger_stage_2, danger_stage_3, danger_stage_4 };
const uint16_t* deadFrames[5] = { dead_stage_0, dead_stage_1, dead_stage_2, dead_stage_3, dead_stage_4 };

CharacterManager::CharacterManager() {
  x = 80;
  oldX = 80;
  y = 115;
  spriteWidth = 120;
  spriteHeight = 120;
  currentAction = IDLE;
  growthStage = 0;
  isDanger = false;
  facingRight = true;
  actionStartTime = 0;
}

void CharacterManager::begin() {
}

void CharacterManager::setPlantState(int stage, bool danger) {
  if (stage < 0) stage = 0;
  if (stage > 4) stage = 4;
  growthStage = stage;
  isDanger = danger;
}

void CharacterManager::setAction(PetAction newAction) {
  if (currentAction != newAction) {
    currentAction = newAction;
    actionStartTime = millis();
  }
}

void CharacterManager::update(DisplayManager& display) {
  uint32_t currentTime = millis();

  if (currentAction == DEAD) {
    display.drawSprite(x, y, spriteWidth, spriteHeight, deadFrames[growthStage]);
    return;
  }

  const uint16_t* baseFrame = isDanger ? dangerFrames[growthStage] : healthyFrames[growthStage];

  if (currentAction == MINIGAME && !facingRight) {
    display.drawSpriteFlipped(x, y, spriteWidth, spriteHeight, baseFrame);
  } else {
    display.drawSprite(x, y, spriteWidth, spriteHeight, baseFrame);
  }

  if (currentAction == EATING) {
    display.drawTransparentImage(x + 15, y + 10, 20, 20, water_frame, TFT_BLACK);
    display.drawTransparentImage(x + 50, y + 5, 20, 20, water_frame, TFT_BLACK);
    display.drawTransparentImage(x + 80, y + 15, 20, 20, water_frame, TFT_BLACK);

    if (currentTime - actionStartTime > 2000) {
      setAction(IDLE);
    }
  } else if (currentAction == PLAYING) {
    display.drawTransparentImage(x + 15, y + 10, 20, 20, heart_frame, TFT_BLACK);
    display.drawTransparentImage(x + 50, y + 5, 20, 20, heart_frame, TFT_BLACK);
    display.drawTransparentImage(x + 80, y + 15, 20, 20, heart_frame, TFT_BLACK);

    if (currentTime - actionStartTime > 2000) {
      setAction(IDLE);
    }
  }
}
