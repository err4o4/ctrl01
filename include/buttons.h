#ifndef BUTTONMANAGER_H
#define BUTTONMANAGER_H

#include <OneButton.h>

class ButtonManager {
public:
    ButtonManager();
    void begin();
    void handle();
    bool isAnyClicked();

private:
    class Button {
    public:
        Button(uint8_t pin, const uint8_t singleCommand, const uint8_t secondCommand, const uint8_t thirdCommand, const uint8_t longCommand);
        void handle();
    private:
        OneButton button;
        const uint8_t singleCommand;
        const uint8_t secondCommand;
        const uint8_t thirdCommand;
        const uint8_t longCommand;
        void Clicked(void *state);
        void DoubleClicked(void *state);
        void MultiClicked(void *state);
        void DuringLongPress(void *state);
    };

    //Button btn_left;
    //Button btn_center;
    //Button btn_right;
    Button btn_up;
    Button btn_down;
};

#endif // BUTTONMANAGER_H
