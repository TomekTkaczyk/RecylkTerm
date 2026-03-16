#pragma once
#include "events.h"

constexpr uint8_t MAX_EVENT_QUEUE = 16;
constexpr uint8_t MAX_EVENT_LISTENERS = 16;

typedef void (*EventCallback)(void* ctx, const Event& event);

struct ListenerEntry
{
    EventType type;
    EventCallback callback;
    void* context;
};

class EventBus
{
public:
    // publishing an event (adding to the queue)
    void publish(const Event &event);

    // event subscription
    void subscribe(EventType type, EventCallback cb, void* ctx);

    // invoking callbacks for all events in the queue
    void update();

private:
    Event queue[MAX_EVENT_QUEUE];
    uint8_t head = 0;
    uint8_t tail = 0;

    ListenerEntry listeners[MAX_EVENT_LISTENERS];
    uint8_t listenerCount = 0;

    bool isEmpty() const { return head == tail; }
};
