#ifndef LEDMANAGER_H
#define LEDMANAGER_H

#include <FastLED.h>
#include <apple_media_service.h>

#define LED_AMOUNT 1
#define DATA_PIN 15
#define GLOBAL_BRIGHTNESS 10

class LEDManager {
public:
    LEDManager();
    void begin();
    void updateLEDs(const AppleMediaService::MediaInformation::PlaybackState playbackStatus, bool isConnected);
    void disable();
    CRGB leds[LED_AMOUNT];

private:
    float animation;
};

#endif // LEDMANAGER_H
