#pragma once

#include <Arduino.h>
#include "./modules/gpioexp/gpioexp.h"

enum ValueType { NONE, INT, FLOAT, BOOL };

struct MenuItem;

typedef void (*DrawCallback)(MenuItem &item);
typedef void (*ActionCallback)(MenuItem &item, ButtonEvent e);

struct MenuItem
{
    const char* name;
    MenuItem* parent = nullptr;
    MenuItem* children = nullptr;
    uint8_t childCount = 0;

    ValueType type = NONE;
    int intVal = 0;
    float floatVal = 0.0f;
    bool boolVal = false;

    DrawCallback onDraw = nullptr;
    ActionCallback onEvent = nullptr;
};

extern MenuItem mainMenu[];
extern uint8_t mainMenuCount;
