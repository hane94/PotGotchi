#include "PetState.h"
#include <Arduino.h>

PetState::PetState()
  : moisture(50), sunlight(50), growth(0), vitality(100), lastStatUpdateTime(0), lastLoveTime(0) {}

void PetState::begin() {
  preferences.begin("pet_data", false);
  moisture = preferences.getInt("moisture", 50);
  sunlight = preferences.getInt("sunlight", 50);
  growth = preferences.getInt("growth", 0);
  vitality = preferences.getInt("vitality", 100);
  dead = preferences.getBool("dead", false);
  weedCount = preferences.getInt("weeds", 0);
  starveTicks = preferences.getInt("starving", 0);
  age = preferences.getInt("age", 0);

  uptimeMinutes = preferences.getInt("uptime", 0);

  lastStatUpdateTime = millis();
  lastWeedTime = millis();
  lastMinuteTime = millis();
  lastLoveTime = preferences.getULong("lastLove", 0);
}

void PetState::update() {
  if (dead) return;

  if (millis() - lastWeedTime > 600000) {
    lastWeedTime = millis();
    if (weedCount < 4) {
      weedCount++;
      saveState();
    }
  }

  if (millis() - lastMinuteTime >= 60000) {
    lastMinuteTime = millis();
    uptimeMinutes++;

    if (uptimeMinutes >= 60) {
      uptimeMinutes = 0;
      addDay();
    } else {
      saveState();
    }
  }

  if (millis() - lastStatUpdateTime > 600000) {
    lastStatUpdateTime = millis();

    if (moisture > 0) {
      moisture -= 2;
      if (moisture < 0) moisture = 0;
    }

    sunlight -= (2 + weedCount);
    if (sunlight < 0) sunlight = 0;

    if (moisture >= 70 && sunlight >= 70) {
      growth++;
      if (growth > 100) growth = 100;
    }

    if (moisture < 10 || sunlight < 10) {
      vitality--;
      if (vitality <= 0) {
        vitality = 0;
        dead = true;
      }
    }

    saveState();
  }
}

int PetState::getGrowthStage() const {
  if (growth < 20) {
    return 0;   // 씨앗
  } else if (growth < 40) {
    return 1;   // 새싹
  } else if (growth < 60) {
    return 2;   // 줄기
  } else if (growth < 80) {
    return 3;   // 봉오리
  } else {
    return 4;   // 꽃
  }
}

void PetState::feed() {
  moisture += 10;
  if (moisture > 100) moisture = 100;
  saveState();
}

void PetState::play(int amount) {
  sunlight += amount;
  if (sunlight > 100) sunlight = 100;
  saveState();
}

void PetState::clean() {
  if (weedCount > 0) {
    weedCount--;
    sunlight += 5;
    if (sunlight > 100) sunlight = 100;
  }
  saveState();
}

void PetState::love() {
  if (canLove()) {
    growth += 5;
    if (growth > 100) growth = 100;
    lastLoveTime = millis();
    preferences.putULong("lastLove", lastLoveTime);
  }
  saveState();
}

void PetState::saveState() {
  preferences.putInt("moisture", moisture);
  preferences.putInt("sunlight", sunlight);
  preferences.putInt("growth", growth);
  preferences.putInt("vitality", vitality);
  preferences.putBool("dead", dead);
  preferences.putInt("weeds", weedCount);
  preferences.putInt("starving", starveTicks);
  preferences.putInt("age", age);
  preferences.putInt("uptime", uptimeMinutes);
}

void PetState::reset() {
  moisture = 50;
  sunlight = 50;
  growth = 0;
  vitality = 100;
  dead = false;
  weedCount = 0;
  starveTicks = 0;
  age = 0;
  uptimeMinutes = 0;
  saveState();
}

void PetState::addDay() {
  age++;
  saveState();
}