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

  // 물/하트 이펙트는 식물 꼭대기 위에 띄운다. (꽃봉오리·꽃이 위로 자라도 겹치지 않게)
  // plantTopRow: 단계별 식물의 가장 윗줄(스프라이트 안의 y 위치)
  static const int plantTopRow[5] = { 44, 32, 15, 9, 7 };
  int effectBaseY = y + plantTopRow[growthStage] - 26;
  if (effectBaseY < 75) effectBaseY = 75;  // 상단 Water/Sun/Growth 영역(0~70)을 침범하지 않게

  if (currentAction == EATING || currentAction == PLAYING) {
    const uint16_t* effect = (currentAction == EATING) ? water_frame : heart_frame;
    display.drawTransparentImage(x + 15, effectBaseY + 6, 20, 20, effect, TFT_BLACK);
    display.drawTransparentImage(x + 50, effectBaseY, 20, 20, effect, TFT_BLACK);
    display.drawTransparentImage(x + 80, effectBaseY + 8, 20, 20, effect, TFT_BLACK);

    if (currentTime - actionStartTime > 2000) {
      // 스프라이트 위쪽 영역에 그린 이펙트는 스프라이트가 지워주지 않으므로 직접 지운다.
      display.clearTrail(x, x + spriteWidth, y - 40, spriteWidth, 40);
      setAction(IDLE);
    }
  }
}
