#ifndef PET_STATE_H
#define PET_STATE_H

#include <Preferences.h>
#include <Arduino.h>

class PetState {
private:
  Preferences preferences;
  bool dead;

  int moisture;
  int sunlight;
  int growth;
  int vitality;
  int weedCount;
  int starveTicks;
  int age;
  int uptimeMinutes;

  uint32_t lastMinuteTime;
  uint32_t lastStatUpdateTime;
  uint32_t lastWeedTime;
  uint32_t lastLoveTime;

public:
  PetState();
  void begin();
  void update();
  void feed();
  void play(int amount);
  void clean();
  void saveState();
  void reset();
  void addDay();

  int getGrowthStage() const;

  int getMoisture() const {
    return moisture;
  }
  int getSunlight() const {
    return sunlight;
  }
  int getGrowth() const {
    return growth;
  }
  int getVitality() const {
    return vitality;
  }
  int getWeedCount() const {
    return weedCount;
  }
  bool isDead() const {
    return dead;
  }
  int getAge() const {
    return age;
  }
  void love();
  bool canLove() const {
  return (millis() - lastLoveTime) > 3600000;
  }
};

#endif