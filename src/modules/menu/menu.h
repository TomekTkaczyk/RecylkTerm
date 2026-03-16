#pragma once

#include <Arduino.h>
#include "menuitems.h"
#include "./modules/display/display.h"
#include "./modules/gpioexp/gpioexp.h"

class Menu
{

public:
    Menu(EventBus &eventBus)
        : eventBus(eventBus) {};

    void begin(MenuItem *rootMenu);
    void update();
    void handleOnButtonEvent(ButtonEvent e);

private:
    EventBus &eventBus;

    MenuItem *current = nullptr;
    MenuItem *currentParent = nullptr;
    uint8_t currentIndex = 0;
    bool inEdit = false;

    void enter();
    void back();
    void left();
    void right();
    void up();
    void down();
    void changeValue(ButtonEvent e);
    static void handleOnButton(void *ctx, const Event &e);
};
