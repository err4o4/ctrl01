#include "leds.h"
#include <apple_media_service.h>

LEDManager::LEDManager(): animation(0) {}

void LEDManager::begin() {
  FastLED.addLeds < NEOPIXEL, DATA_PIN > (leds, LED_AMOUNT);
  FastLED.setBrightness(GLOBAL_BRIGHTNESS);
}

void LEDManager::updateLEDs(const AppleMediaService::MediaInformation::PlaybackState playbackStatus, bool isConnected) {

  if (!isConnected) {
    static uint8_t brightness = 0;
    static int8_t delta = 1;

    for (int i = 0; i < LED_AMOUNT; i++) {
      leds[i] = CRGB(0, brightness, 0);
    }

    FastLED.show();

    brightness += delta;
    if (brightness == 0 || brightness == 255) {
      delta = -delta;
    }

  } else {
    if (playbackStatus == AppleMediaService::MediaInformation::PlaybackState::Playing) {
      for (int i = 0; i < LED_AMOUNT; i++) {
        leds[i] = CHSV(animation + (i * (255 / LED_AMOUNT)), 255, 255);
      }
      animation += 0.3;
    } else {
      for (int i = 0; i < LED_AMOUNT; i++) {
        leds[i] = CRGB(0, 255, 0);
      }
    }

    FastLED.show();
  }
}

void LEDManager::disable() {
  FastLED.clear(true);
}
