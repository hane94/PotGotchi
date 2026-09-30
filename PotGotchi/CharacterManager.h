#ifndef CHARACTER_MANAGER_H
#define CHARACTER_MANAGER_H

#include "DisplayManager.h"

enum PetAction { IDLE,
                 EATING,
                 PLAYING,
                 DEAD,
                 MINIGAME };

class CharacterManager {
private:
  int x, y;
  int oldX;
  int spriteWidth;
  int spriteHeight;
  PetAction currentAction;

  int growthStage;
  bool isDanger;
  bool facingRight;

  uint32_t actionStartTime;

public:
  CharacterManager();
  void begin();
  void update(DisplayManager& display);
  void setAction(PetAction newAction);
  void setPlantState(int stage, bool danger);

  PetAction getCurrentAction() const {
    return currentAction;
  }

  int getX() const {
    return x;
  }
  int getOldX() const {
    return oldX;
  }
  int getY() const {
    return y;
  }
  void setX(int newX) {
    if (newX < 0) newX = 0;
    if (newX > 280 - spriteWidth) newX = 280 - spriteWidth;
    x = newX;
  }
  void setFacingRight(bool right) {
    facingRight = right;
  }
};

#endif
