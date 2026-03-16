#include "gpioexp.h"

GPIOExpander *GPIOExpander::expander = nullptr;

GPIOExpander::GPIOExpander(uint8_t address, uint8_t interruptPin, EventBus &eventBus)
    : pcf(address), interruptPin(interruptPin), eventBus(eventBus)
{
    expander = this;
    Wire.beginTransmission(address);
    Wire.write(0xFF);
    Wire.endTransmission();
}

void GPIOExpander::begin(const uint8_t *inputs, uint8_t inputCount, const uint8_t *outputs, uint8_t outputCount)
{
    this->inputs = inputs;
    this->inputCount = inputCount;
    this->outputs = outputs;
    this->outputCount = outputCount;

    for (int i = 0; i < inputCount; i++)
        pcf.pinMode(inputs[i], INPUT);

    for (uint8_t i = 0; i < outputCount; i++)
        pcf.pinMode(outputs[i], OUTPUT, HIGH);

    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), isrStatic, FALLING);

    pcf.begin();
    lastState = readInputs();
    memset(lastDebounce, 0, sizeof(lastDebounce));
    memset(pressTime, 0, sizeof(pressTime));
    memset(lastRepeat, 0, sizeof(lastRepeat));
    memset(lastClick, 0, sizeof(lastClick));
    memset(clickCount, 0, sizeof(clickCount));
}

void GPIOExpander::update()
{
    // animacja diod na wyjściach (do usunięcia lub zastąpienia docelową funkcjonalnością)
    unsigned long now = millis();
    if (now - lastUpdate >= FLIP_OUTPUT_INTERVAL)
    {
        pcf.digitalWrite(outputs[currentOutput], HIGH);
        currentOutput = (currentOutput + 1) % outputCount;
        pcf.digitalWrite(outputs[currentOutput], LOW);
        lastUpdate = now;
    }

    // button operation
    uint8_t state = lastState;
    if (interruptFlag)
    {
        interruptFlag = false;
        state = readInputs();
    }
    process(state);
}

void IRAM_ATTR GPIOExpander::isrStatic()
{
    if (expander)
    {
        expander->interruptFlag = true;
    }
}

uint8_t GPIOExpander::readInputs()
{
    auto d = pcf.digitalReadAll();
    uint8_t state = 0;
    for (int i = 0; i < inputCount; i++)
    {
        uint8_t pin = inputs[i];
        bool bit = 1;
        switch (pin)
        {
        case P0:
            bit = d.p0;
            break;
        case P1:
            bit = d.p1;
            break;
        case P2:
            bit = d.p2;
            break;
        case P3:
            bit = d.p3;
            break;
        case P4:
            bit = d.p4;
            break;
        case P5:
            bit = d.p5;
            break;
        case P6:
            bit = d.p6;
            break;
        case P7:
            bit = d.p7;
            break;
        }

        if (!bit)
        {
            state |= (1 << i);
        }
    }

    return state;
}

void GPIOExpander::process(uint8_t state)
{
    unsigned long now = millis();
    for (int i = 0; i < inputCount; i++)
    {
        bool pressed = state & (1 << i);
        bool last = lastState & (1 << i);
        if (pressed != last)
        {
            if (now - lastDebounce[i] < DEBOUNCE_MS)
            {
                continue;
            }
            lastDebounce[i] = now;
            if (pressed)
            {
                pressTime[i] = now;
                lastRepeat[i] = 0;
                publishButtonEvent(EventType::EVT_BUTTON_PRESS, i);
            }
            else
            {
                publishButtonEvent(EventType::EVT_BUTTON_RELEASE, i);
                lastRepeat[i] = 0;
                if (now - pressTime[i] < LONG_PRESS_MS)
                {
                    clickCount[i]++;
                    if (clickCount[i] == 1)
                    {
                        lastClick[i] = now;
                    }
                    else
                    {
                        publishButtonEvent(EventType::EVT_BUTTON_DOUBLE_CLICK, i);
                        clickCount[i] = 0;
                    }
                }
            }
        }

        if (pressed)
        {
            if (now - pressTime[i] > LONG_PRESS_MS)
            {
                if (lastRepeat[i] == 0)
                {
                    publishButtonEvent(EventType::EVT_BUTTON_LONG_PRESS, i);
                    lastRepeat[i] = now;
                }
                if (now - lastRepeat[i] > REPEAT_MS)
                {
                    publishButtonEvent(EventType::EVT_BUTTON_REPEAT, i);
                    lastRepeat[i] = now;
                }
            }
        }

        if (clickCount[i] == 1)
        {
            if (now - lastClick[i] > CLICK_MS)
            {
                publishButtonEvent(EventType::EVT_BUTTON_CLICK, i);
                clickCount[i] = 0;
            }
        }
    }
    lastState = state;
}

void GPIOExpander::publishButtonEvent(EventType type, uint8_t pin)
{
    Event e;
    e.type = type;
    e.data.button.pin = pin;
    eventBus.publish(e);
}
