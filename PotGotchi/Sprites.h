#ifndef SPRITES_H
#define SPRITES_H

#include <Arduino.h>

// 건강한 상태 (성장 단계별, 120x120)
extern const uint16_t healthy_stage_0[] PROGMEM;
extern const uint16_t healthy_stage_1[] PROGMEM;
extern const uint16_t healthy_stage_2[] PROGMEM;
extern const uint16_t healthy_stage_3[] PROGMEM;
extern const uint16_t healthy_stage_4[] PROGMEM;

// 위험한 상태 (성장 단계별, 120x120)
extern const uint16_t danger_stage_0[] PROGMEM;
extern const uint16_t danger_stage_1[] PROGMEM;
extern const uint16_t danger_stage_2[] PROGMEM;
extern const uint16_t danger_stage_3[] PROGMEM;
extern const uint16_t danger_stage_4[] PROGMEM;

// 시든 상태 (성장 단계별, 120x120)
extern const uint16_t dead_stage_0[] PROGMEM;
extern const uint16_t dead_stage_1[] PROGMEM;
extern const uint16_t dead_stage_2[] PROGMEM;
extern const uint16_t dead_stage_3[] PROGMEM;
extern const uint16_t dead_stage_4[] PROGMEM;

// 이펙트/아이템 (20x20)
extern const uint16_t heart_frame[] PROGMEM;
extern const uint16_t water_frame[] PROGMEM;
extern const uint16_t weed_frame[] PROGMEM;
extern const uint16_t sunlight_frame[] PROGMEM;

#endif
