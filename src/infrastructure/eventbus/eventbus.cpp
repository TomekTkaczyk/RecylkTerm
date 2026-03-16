#include "eventbus.h"

static const __FlashStringHelper *eventTypeToString(EventType type);

void EventBus::publish(const Event &event)
{
    uint8_t next = (head + 1) % MAX_EVENT_QUEUE;

    if (next == tail)
        tail = (tail + 1) % MAX_EVENT_QUEUE;
    queue[head] = event;
    head = next;
}

void EventBus::subscribe(EventType type, EventCallback cb, void *ctx)
{
    if (listenerCount >= MAX_EVENT_LISTENERS)
    {
        Serial.println(F("Listener list full"));
        return;
    }

    listeners[listenerCount++] = {type, cb, ctx};

    Serial.print(F("Add listener for event: "));
    Serial.println(eventTypeToString(type));
}

void EventBus::update()
{
    while (!isEmpty())
    {
        Event &event = queue[tail];
        tail = (tail + 1) % MAX_EVENT_QUEUE;

        bool handled = false;

        for (uint8_t i = 0; i < listenerCount; i++)
        {
            if (listeners[i].type == event.type)
            {
                listeners[i].callback(listeners[i].context, event);
                handled = true;
            }
        }

        if (!handled)
        {
            Serial.print(F("Unhandled event: "));
            Serial.println(eventTypeToString(event.type));
        }
    }
}

static const __FlashStringHelper *eventTypeToString(EventType type)
{
    switch (type)
    {
    case EventType::EVT_BUTTON_PRESS:
        return F("EVT_BUTTON_PRESS");
    case EventType::EVT_BUTTON_RELEASE:
        return F("EVT_BUTTON_RELEASE");
    case EventType::EVT_BUTTON_CLICK:
        return F("EVT_BUTTON_CLICK");
    case EventType::EVT_BUTTON_DOUBLE_CLICK:
        return F("EVT_BUTTON_DOUBLE_CLICK");
    case EventType::EVT_BUTTON_LONG_PRESS:
        return F("EVT_BUTTON_LONG_PRESS");
    case EventType::EVT_BUTTON_REPEAT:
        return F("EVT_BUTTON_REPEAT");
    case EventType::EVT_TEMPERATURE_READY:
        return F("EVT_TEMPERATURE_READY");
    case EventType::EVT_FOUND_SENSOR:
        return F("EVT_FOUND_SENSOR");
    case EventType::EVT_DISPLAY_SENSOR_SCREEN:
        return F("EVT_DISPLAY_SENSOR_SCREEN");
    case EventType::EVT_DISPLAY_MENU_SCREEN:
        return F("EVT_DISPLAY_MENU_SCREEN");
    case EventType::EVT_DISPLAY_SETTINGS_SCREEN:
        return F("EVT_DISPLAY_SETTINGS_SCREEN");
    default:
        return F("EVT_UNKNOWN");
    }
}
