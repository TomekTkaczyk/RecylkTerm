#include "menuitems.h"

void drawCallback(MenuItem &item)
{
    Serial.print("> "); 
    Serial.println(item.name);
    if(item.type == INT) Serial.println(item.intVal);
    if(item.type == FLOAT) Serial.println(item.floatVal, 1);
    if(item.type == BOOL) Serial.println(item.boolVal ? "ON" : "OFF");
}

void eventCallback(MenuItem &item, ButtonEvent e)
{
    Serial.print("Event on ");
    Serial.print(item.name);
    Serial.print(": ");
    switch(e.type)
    {
        case BUTTON_PRESS: Serial.println("PRESS"); break;
        case BUTTON_RELEASE: Serial.println("RELEASE"); break;
        case BUTTON_CLICK: Serial.println("CLICK"); break;
        case BUTTON_DOUBLE_CLICK: Serial.println("DOUBLE"); break;
        case BUTTON_LONG_PRESS: Serial.println("LONG"); break;
        case BUTTON_REPEAT: Serial.println("REPEAT"); break;
    }
}

MenuItem submenu[] = {
    {"Set Temp", nullptr, nullptr, 0, INT, 25, 0.0f, false, drawCallback, nullptr},
    {"Set Mode", nullptr, nullptr, 0, BOOL, 0, 0.0f, true, drawCallback, nullptr}
};

MenuItem mainMenu[] = {
    {"Temperature1", nullptr, nullptr, 0, NONE, 0, 0.0f, false, drawCallback, eventCallback},
    {"Temperature2", nullptr, nullptr, 0, NONE, 0, 0.0f, false, drawCallback, eventCallback},
    {"Settings", nullptr, submenu, 2, NONE, 0, 0.0f, false, drawCallback, nullptr}
};
