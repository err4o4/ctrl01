#include "display.h"
#include "logger.h"
#include "fonts/AniMeMatrix_MB_EN30.h"
#include "fonts/AniMeMatrix_MB_EN20.h"

#define TFT_SUPERGREY 0x4208 /* 64, 64, 64 */

DisplayManager::DisplayManager() : tft(TFT_eSPI(135, 240)), sprite(TFT_eSprite(&tft)), previousMillis(0), lastElapsed(0), totalFreeHeap(0), sampleCount(0), averageFreeHeap(esp_get_free_heap_size() / 1024.0), isEnabled(false) {}

void DisplayManager::begin() {
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);

    ledcSetup(0, 5000, 8); 
    ledcAttachPin(TFT_BL, 0); 
    ledcWrite(0, 127);

    sprite.setColorDepth(16);
    sprite.createSprite(tft.width(), tft.height()); 
    isEnabled = true;
} 

void DisplayManager::disable() {
    tft.fillScreen(TFT_BLACK);
    tft.writecommand(ST7789_DISPOFF);    
    tft.writecommand(ST7789_SLPIN);
    ledcWrite(0, 0);
    isEnabled = false;
}

void DisplayManager::drawText(String line, int yPos, int xOffset, int fontSize, int textColor, int xPosSelect) {
    sprite.setTextSize(fontSize);
    sprite.setTextColor(textColor);

    sprite.setTextFont(0);
    int textWidth = tft.textWidth(line) * fontSize + 20;

    static int xPosSongName = xOffset;
    static int xPosArtist = xOffset;
    static int xPosAlbum = xOffset;

    int& xPos = xPosSelect == 0 ? xPosSongName :
                xPosSelect == 1 ? xPosArtist :
                xPosSelect == 2 ? xPosAlbum : xPosSongName; 

    if (textWidth <= tft.width()) {
        sprite.drawString(line, xOffset, yPos);
    } else {
        sprite.drawString(line, xPos, yPos);
        xPos--;
        if (xPos < -(textWidth)) {
            xPos = tft.width();
        }
    }
}

void DisplayManager::updateDisplay(AppleMediaService::MediaInformation media_info, float battery_voltage, bool isConnected, unsigned long initialFreeHeap) {
    if(!isEnabled) {
        begin();
    }

    sprite.fillSprite(TFT_BLACK);

    addSecond(media_info.mPlaybackState, isConnected);
    checkRAM(100);

    int volPos = map(media_info.mVolume * 1000, 0, 1000, 0, 240);

    if(!isConnected) {
        drawLogo();
        //drawText("Connecting", 60, 4, TFT_WHITE, 0);
    } else if (media_info.mPlaybackState == AppleMediaService::MediaInformation::PlaybackState::Paused) {
        drawText("Paused", 60, 10, 4, TFT_WHITE, 0);
        sprite.fillRect(0, 0, volPos, 14, TFT_WHITE);
    } else {
        int playPos = 0;

        if(media_info.mElapsedTime != 0) {
            playPos = map(calcElapsed(media_info.mElapsedTime) * 1000, 0, media_info.mDuration * 1000, 0, 240);
        }
        
        sprite.fillRect(0, 0, playPos, 135, TFT_SUPERGREY);
        sprite.fillRect(0, 0, volPos, 14, TFT_WHITE);

        drawText(media_info.mTitle.c_str(), 50, 10, 4, TFT_WHITE, 0);
        drawText(media_info.mArtist.c_str(),   90, 10, 2, TFT_LIGHTGREY, 1);
        drawText(media_info.mAlbum.c_str(),    110, 10, 2, TFT_LIGHTGREY, 2);
    }
    
    if (battery_voltage < 3.8) {
        sprite.fillCircle(225, 7, 3, TFT_RED);
    } else if (battery_voltage > 3.8 && battery_voltage < 4.0) {
        sprite.fillCircle(225, 7, 3, TFT_YELLOW);
    } else if (battery_voltage > 4.0 && battery_voltage < 4.3) {
        sprite.fillCircle(225, 7, 3, TFT_DARKGREEN);
    } else {
        sprite.fillCircle(225, 7, 3, TFT_PURPLE);
    }

    drawText(String(initialFreeHeap - averageFreeHeap) + "KB/" + String(initialFreeHeap) + "KB", 4, 110, 1, TFT_DARKGREY, 0);
    drawText(String(battery_voltage) + "v", 4, 185, 1, TFT_DARKGREY, 0);

    sprite.pushSprite(0, 0);
}

void DisplayManager::addSecond(const AppleMediaService::MediaInformation::PlaybackState playbackStatus, bool isConnected) {
    if (millis() - previousMillis >= 1000 && playbackStatus == AppleMediaService::MediaInformation::PlaybackState::Playing && isConnected) {
      previousMillis = millis();
      elapsedTime += 1.0;
    }
}

void DisplayManager::checkRAM(int numSamples) {
    totalFreeHeap += esp_get_free_heap_size();
    sampleCount++;
    if (sampleCount >= numSamples) {
        averageFreeHeap = totalFreeHeap / sampleCount / 1024.0;
    }
}

float DisplayManager::calcElapsed(float mElapsedTime) {
    if(lastElapsed != mElapsedTime) {
        elapsedTime = mElapsedTime;
        lastElapsed = elapsedTime;
    } 
    return elapsedTime;
}

void DisplayManager::drawLogo() {
    digitalWrite(TFT_BL, HIGH);
    sprite.fillSprite(TFT_BLACK);

    sprite.setTextColor(TFT_GREEN);
    sprite.loadFont(AniMeMatrix_MB_EN30);
    sprite.drawString("CTRL 01", 42, 50);

    sprite.setTextColor(TFT_DARKGREY);
    sprite.loadFont(AniMeMatrix_MB_EN20);
    sprite.drawString("CONNECTING", 35, 80);

    sprite.unloadFont();
}