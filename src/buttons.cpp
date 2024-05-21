#include "buttons.h"
#include "apple_media_service.h"
#include "logger.h"

void send_cmd(const uint8_t cmd) {
    if (cmd != 0) {
        AppleMediaService::setRemoteCommandValue(cmd);
        delay(100);
    } else {
        LOG_TRACE("No command found, skipping");
    }
}

ButtonManager::Button::Button(uint8_t pin, const uint8_t singleCommand, const uint8_t secondCommand, const uint8_t thirdCommand, const uint8_t longCommand)
    : button(pin), singleCommand(singleCommand), secondCommand(secondCommand), thirdCommand(thirdCommand), longCommand(longCommand) {
    if (singleCommand != 0) {
        button.attachClick([](void *scope) { ((Button *)scope)->Clicked(scope); }, this);
    }
    if (secondCommand != 0) {
        button.attachDoubleClick([](void *scope) { ((Button *)scope)->DoubleClicked(scope); }, this);
    }
    if (longCommand != 0) {
        button.attachDuringLongPress([](void *scope) { ((Button *)scope)->DuringLongPress(scope); }, &button);
    }
    if (thirdCommand != 0) {
        button.attachMultiClick([](void *scope) { ((Button *)scope)->MultiClicked(scope); }, this);
    }

    button.setDebounceMs(25);
    button.setClickMs(200);
    button.setPressMs(500);
}

void ButtonManager::Button::Clicked(void *state) {
    send_cmd(singleCommand);
}

void ButtonManager::Button::DoubleClicked(void *state) {
    send_cmd(secondCommand);
}

void ButtonManager::Button::MultiClicked(void *state) {
    int n = ((OneButton *)state)->getNumberClicks();
    if (n == 3) {
        send_cmd(thirdCommand);
        //displayManager.showBattery(batteryManager.getBatteryVoltage());
    }
}

void ButtonManager::Button::DuringLongPress(void *state) {
    send_cmd(longCommand);
}

void ButtonManager::Button::handle() {
    button.tick();
}

ButtonManager::ButtonManager(): 
    //btn_left(12, RemoteCommandIDNextTrack, 0, 0, 0),
    //btn_center(13, RemoteCommandIDTogglePlayPause, RemoteCommandIDNextTrack, RemoteCommandIDPreviousTrack, RemoteCommandIDLikeTrack),
    //btn_right(15, RemoteCommandIDPreviousTrack, 0, 0, 0),

    btn_up(35, RemoteCommandIDTogglePlayPause, RemoteCommandIDNextTrack, RemoteCommandIDPreviousTrack, RemoteCommandIDVolumeUp),
    btn_down(0, RemoteCommandIDVolumeDown, 0, 0, RemoteCommandIDVolumeDown) {}

void ButtonManager::begin() {
    // Initialize buttons if needed
}

void ButtonManager::handle() {
    //btn_left.handle();
    //btn_center.handle();
    //btn_right.handle();
    btn_up.handle();
    btn_down.handle();
}

bool ButtonManager::isAnyClicked() {
    if (
        //(digitalRead(12) == LOW) ||
        //(digitalRead(13) == LOW) ||
        //(digitalRead(15) == LOW) ||
        (digitalRead(0) == LOW) ||
        (digitalRead(35) == LOW)) {
        return true;
    } else {
        return false;
    }
}
