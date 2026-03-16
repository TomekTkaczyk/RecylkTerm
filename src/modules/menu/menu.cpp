#include "menu.h"

void Menu::handleOnButton(void *ctx, const Event &e)
{
    Menu *self = static_cast<Menu *>(ctx);
    if (!self)
        return;

    ButtonEvent be;
    be.pin = e.data.button.pin;
    be.type = BUTTON_CLICK;
    self->handleOnButtonEvent(be);
}

void Menu::begin(MenuItem *rootMenu)
{
    eventBus.subscribe(EventType::EVT_BUTTON_CLICK, Menu::handleOnButton, this);
    eventBus.subscribe(EventType::EVT_BUTTON_REPEAT, Menu::handleOnButton, this);

    current = rootMenu;
    currentParent = rootMenu->parent;
    currentIndex = 0;

    if (current->onDraw)
        current->onDraw(*current);
}

void Menu::update()
{
    if (current->onDraw)
        current->onDraw(*current);
}

void Menu::handleOnButtonEvent(ButtonEvent e)
{
    if (e.type != BUTTON_CLICK && e.type != BUTTON_REPEAT)
        return;

    switch (e.pin)
    {
    case 0: // LEFT
        left();
        break;
    case 1: // RIGHT
        right();
        break;
    case 2: // OK / BACK
        enter();
        break;
    case 3: // UP
        up();
        break;
    case 4: // DOWN
        down();
        break;
    }
}

void Menu::enter()
{
    if (current->children && current->childCount > 0)
    {
        currentParent = current;
        current = &current->children[0];
        currentIndex = 0;
    }
    else if (current->type != NONE)
        inEdit = true;

    if (current->onDraw)
        current->onDraw(*current);
}

void Menu::back()
{
    if (currentParent)
    {
        current = currentParent;
        currentParent = current->parent;
        currentIndex = 0;
        inEdit = false;
    }

    if (current->onDraw)
        current->onDraw(*current);
}

void Menu::right()
{
    if (currentParent && currentParent->childCount > 0)
    {
        currentIndex = (currentIndex + 1) % currentParent->childCount;
        current = &currentParent->children[currentIndex];
    }
    if (current->onDraw)
        current->onDraw(*current);
}

void Menu::left()
{
    if (currentParent && currentParent->childCount > 0)
    {
        if (currentIndex == 0)
            currentIndex = currentParent->childCount - 1;
        else
            currentIndex--;
        current = &currentParent->children[currentIndex];
    }
    if (current->onDraw)
        current->onDraw(*current);
}

void Menu::up()
{
    if (current->type == INT)
        current->intVal++;
    else if (current->type == FLOAT)
        current->floatVal += 0.1f;
    else if (current->type == BOOL)
        current->boolVal = !current->boolVal;

    if (current->onDraw)
        current->onDraw(*current);
}

void Menu::down()
{
    if (current->type == INT)
        current->intVal--;
    else if (current->type == FLOAT)
        current->floatVal -= 0.1f;
    else if (current->type == BOOL)
        current->boolVal = !current->boolVal;

    if (current->onDraw)
        current->onDraw(*current);
}

void Menu::changeValue(ButtonEvent e)
{
    if (current->type == INT)
    {
        if (e.pin == 3)
            current->intVal++; // UP
        if (e.pin == 4)
            current->intVal--; // DOWN
    }
    else if (current->type == FLOAT)
    {
        if (e.pin == 3)
            current->floatVal += 0.1f;
        if (e.pin == 4)
            current->floatVal -= 0.1f;
    }
    else if (current->type == BOOL)
    {
        if (e.pin == 3 || e.pin == 4)
            current->boolVal = !current->boolVal;
    }

    if (current->onDraw)
        current->onDraw(*current);
}