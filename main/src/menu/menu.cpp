//
// Created by Tomáš Novák on 17.12.2022.
//
#include "menu/menu.h"

Menu::Menu(SH1106* lcd)
    : lcd(lcd)
{
    instance = this;
}

void Menu::init()
{
    if (scene != nullptr)
        scene->onStart();
}

void Menu::update()
{
    if (scene != nullptr)
        scene->onUpdate();
}

void Menu::input(uint8_t pin, Device* device)
{
    if (scene != nullptr)
        scene->onInput(pin, device);
}

void Menu::changeScene(Scene* scene)
{
    if (this->scene != nullptr) {
        delete this->scene;

        this->scene = nullptr;
    }

    lcd->clear();

    if (scene != nullptr) {
        scene->setLCD(lcd);
        this->scene = scene;
        scene->onStart();
    }
}

Menu* Menu::instance = nullptr;