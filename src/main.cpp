#include <Arduino.h>
#include <esp_sleep.h>
#include "bluetooth.h"
#include "apple_media_service.h"
#include "display.h"
#include "battery.h"
#include "buttons.h"
#include "leds.h"
#include "logger.h"
#include "esp_system.h"

const unsigned long sleepTimeout   = 1000 * 30; // 5 min
const unsigned long displayTimeout = 1000 * 10;     // 30 sec

BatteryManager batteryManager(34);
ButtonManager buttonManager;
DisplayManager displayManager;
LEDManager ledManager;

unsigned long startTime = 0;
bool disableDisplay = false;
bool isConnected = false;
unsigned long initialFreeHeap = 0;
AppleMediaService::MediaInformation media_info;

TaskHandle_t handleLedsButtonsTask = NULL;
TaskHandle_t handleBLEDisplayTask = NULL;
TaskHandle_t handleSleepTask = NULL;

void handleLedsButtons(void * parameter) {
  for (;;) {
    buttonManager.handle();
    if(buttonManager.isAnyClicked()) {
      startTime = millis();
      disableDisplay = false;
    }
    if(disableDisplay == false) {
      ledManager.updateLEDs(media_info.mPlaybackState, isConnected);
    } else {
      ledManager.disable();
    }
  }
}

void handleBLEDisplay(void * parameter) {
  for (;;) {
    Bluetooth::Service();
    isConnected = Bluetooth::IsConnected();

    if(disableDisplay == false) {
      displayManager.updateDisplay(
        media_info,
        batteryManager.getBatteryVoltage(),
        isConnected,
        initialFreeHeap
      );
    } 
    batteryManager.handle();
    vTaskDelay(10);
  }
}

void handleSleep(void * parameter) {
  // current 
  // leds + display 150mA
  // off 80mA
  // sleep 1mA
  for (;;) {
    if (millis() - startTime >= displayTimeout && disableDisplay == false) {
      LOG_TRACE("Turn display and LEDs off");
      disableDisplay = true;
      displayManager.disable();
    }
    if (millis() - startTime >= sleepTimeout && (media_info.mPlaybackState != AppleMediaService::MediaInformation::PlaybackState::Playing || !isConnected)) {
      LOG_TRACE("Going to deep sleep");
      vTaskDelete(handleLedsButtonsTask);
      vTaskDelete(handleBLEDisplayTask);
      delay(500);
      //Bluetooth::End();
      Serial.flush();
      btStop();
      esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, LOW);
      esp_sleep_enable_ext1_wakeup(GPIO_SEL_35, ESP_EXT1_WAKEUP_ALL_LOW);
      esp_deep_sleep_start();
    }
    vTaskDelay(1000);
  }
}

void onDataUpdateCallback() {
  media_info = AppleMediaService::GetMediaInformation();
}

void setup() {
  initialFreeHeap = esp_get_free_heap_size() / 1024.0;
  Serial.begin(115200);

  ledManager.begin();
  batteryManager.begin();
  batteryManager.checkBattery();
  buttonManager.begin();
  displayManager.begin();

  Bluetooth::Begin("CTRL 01");
  Bluetooth::Service();
  AppleMediaService::RegisterForNotifications(onDataUpdateCallback, AppleMediaService::NotificationLevel::All);

  xTaskCreatePinnedToCore(handleLedsButtons, "handleLedsButtons", 8192, NULL, 1, &handleLedsButtonsTask, 0);
  xTaskCreatePinnedToCore(handleBLEDisplay, "handleBLEDisplay", 8192, NULL, 1, &handleBLEDisplayTask, 1);
  xTaskCreatePinnedToCore(handleSleep, "handleSleep", 8192, NULL, 1, &handleSleepTask, 1);
}

void loop() {
  // Loop is empty as all logic is handled in tasks
}
