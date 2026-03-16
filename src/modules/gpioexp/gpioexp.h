#pragma once

#include <Arduino.h>
#include <PCF8574.h>

#include "./infrastructure/eventbus/eventbus.h"

#define MAX_EVENTS 10
#define BUTTON_COUNT 5
#define OUTPUT_COUNT 3

#define DEBOUNCE_MS 50
#define CLICK_MS 350
#define LONG_PRESS_MS 1200
#define REPEAT_MS 400

#define FLIP_OUTPUT_INTERVAL 300

// piny expandera portów
const uint8_t inputPins[] = {P0, P1, P2, P3, P4};
const uint8_t outputPins[] = {P5, P6, P7};

enum ButtonEventType
{
    BUTTON_PRESS,
    BUTTON_RELEASE,
    BUTTON_CLICK,
    BUTTON_DOUBLE_CLICK,
    BUTTON_LONG_PRESS,
    BUTTON_REPEAT
};

struct ButtonEvent
{
    uint8_t pin;
    ButtonEventType type;
};

class GPIOExpander
{
public:
    GPIOExpander(uint8_t address, uint8_t interruptPin, EventBus &eventBus);
    typedef void (*Callback)(ButtonEvent event);
    void begin(const uint8_t *inputs, uint8_t inputCount, const uint8_t *outputs, uint8_t outputCount);
    void update();

private:
    static GPIOExpander *expander;

    PCF8574 pcf;
    uint8_t interruptPin;
    EventBus &eventBus;


    const uint8_t *inputs;
    uint8_t inputCount;
    const uint8_t *outputs;
    uint8_t outputCount;

    volatile bool interruptFlag = false;

    uint8_t lastState;

    unsigned long lastDebounce[BUTTON_COUNT];
    unsigned long pressTime[BUTTON_COUNT];
    unsigned long lastRepeat[BUTTON_COUNT];
    unsigned long lastClick[BUTTON_COUNT];

    uint8_t clickCount[BUTTON_COUNT];

    Callback callback;

    ButtonEvent queue[MAX_EVENTS];
    uint8_t qHead = 0;
    uint8_t qTail = 0;

    static void IRAM_ATTR isrStatic();

    uint8_t readInputs();

    void process(uint8_t state);
    void publishButtonEvent(EventType type, uint8_t pin);

    const unsigned long outputInterval = FLIP_OUTPUT_INTERVAL; // 300 ms
    unsigned long lastUpdate = 0;
    uint8_t currentOutput = 0;
};









