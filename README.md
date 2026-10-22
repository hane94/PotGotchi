# 🌱 Plant Care Tamagotchi (화분 키우기 다마고치)

ESP32-C3 기반의 화분 키우기 다마고치입니다.
derdacavga(DSN Industries)님의 [Esp32-Tamagotchi](https://github.com/derdacavga/Esp32-Tamagotchi)를
기반으로, 다마고치(고양이) 컨셉을 화분 키우기로 전면 재설계했습니다.

A plant-growing tamagotchi built on ESP32-C3, based on
[Esp32-Tamagotchi](https://github.com/derdacavga/Esp32-Tamagotchi) by derdacavga (DSN Industries).
The original cat-tamagotchi concept was redesigned into a plant-care simulator.

---

## 📋 License

This project is licensed under **GPL-3.0**, inheriting the license of the original project.
이 프로젝트는 원본 프로젝트의 라이선스를 이어받아 **GPL-3.0**으로 배포됩니다.

- Original software: [Esp32-Tamagotchi](https://github.com/derdacavga/Esp32-Tamagotchi) (GPL-3.0)
- Display library: [DSN-Esp32C3-St7789](https://github.com/derdacavga/DSN-Esp32C3-St7789) (GPL-3.0)

3D 프린팅 케이스(STL)는 별도의 독립 창작물로, 소프트웨어 라이선스와 무관합니다.
The 3D-printed case (STL files) is an independent original design, separate from the software license.
- case:https://makerworld.com/en/models/3476394-potgotchi-diy-mini-game-plant-tamagotchi#profileId-3968646
---

## ✨ 주요 변경사항 / Key Changes

| 항목 / Item | 원본 / Original | 변경 후 / Changed |
|---|---|---|
| 스탯 체계 / Stats | Hunger, Happiness, Energy | Moisture, Sunlight, Growth |
| 생명 시스템 / Life system | Energy 기반 사망 | 신규 Vitality(생명력) 시스템 |
| 성장 표현 / Growth | 없음 | 5단계(씨앗→새싹→줄기→봉오리→꽃) |
| 그래픽 / Sprites | 고양이 애니메이션 | 화분 정지 이미지 + 이펙트(물방울/하트) |
| 상호작용 / Interactions | Feed, Play, Sleep, Clean | Water, Sunlight(미니게임), Clean, Love |
| 메뉴 / Menu | 6개 항목 | 4개 항목 (Sleep/Settings 제거) |

## 🛠 하드웨어 / Hardware

- ESP32-C3 개발보드 (Type-C) / ESP32-C3 development board (Type-C)
- 1.69인치 TFT LCD 디스플레이 (ST7789V3, 240×280, SPI) / 1.69" TFT LCD display (ST7789V3, 240×280, SPI)
- 1x4 버튼 모듈 / 1x4 button module

자세한 조립 가이드는 `docs/` 폴더의 제작 가이드를 참고하세요.
See the build guide in `docs/` for full assembly instructions.

## 📦 라이브러리 / Library

디스플레이 라이브러리: [DSN-Esp32C3-St7789](https://github.com/derdacavga/DSN-Esp32C3-St7789)

## 🚀 시작하기 / Getting Started

1. Arduino IDE에 ESP32 보드 패키지 설치
2. 위 디스플레이 라이브러리 다운로드 및 설치
3. 이 저장소를 다운로드하고 `Tamagotchi.ino`를 Arduino IDE로 열기
4. 보드를 `ESP32C3 Dev Module`로 설정 후 업로드

Install the ESP32 board package in Arduino IDE, install the display library above,
download this repository, open `Tamagotchi.ino`, select `ESP32C3 Dev Module` as the board, and upload.

---

Made with 🌱 based on an open-source foundation.
