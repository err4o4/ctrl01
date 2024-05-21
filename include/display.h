#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <TFT_eSPI.h>
#include <apple_media_service.h>


class DisplayManager {
public:
    DisplayManager();
    void begin();
    void updateDisplay(
        AppleMediaService::MediaInformation media_info,
        float battery_voltage, 
        bool isConnected, 
        unsigned long initialFreeHeap
    );
    void disable();    
private:
    TFT_eSPI tft;
    TFT_eSprite sprite;
    bool isEnabled;
    // Playback position
    unsigned long previousMillis;
    float lastElapsed;
    float elapsedTime;
    // RAM
    unsigned long totalFreeHeap;
    int sampleCount;
    unsigned long averageFreeHeap;
    void drawText(String line, int yPos, int xOffset, int fontSize, int textColor, int xPosSelect);
    float calcElapsed(float elapsedTime);
    void addSecond(const AppleMediaService::MediaInformation::PlaybackState playbackStatus, bool isConnected);
    void drawLogo();
    void checkRAM(int numSamples);
};

#endif // DISPLAYMANAGER_H
