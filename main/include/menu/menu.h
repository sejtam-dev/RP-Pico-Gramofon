//
// Created by Tomáš Novák on 17.12.2022.
//

#ifndef GRAMOFON_MENU_H
#define GRAMOFON_MENU_H

#include "menu/scene.h"

#include "device.h"
#include "SH1106.h"


class Menu
{
private:
    static Menu* instance;

    SH1106* lcd;
    Scene* scene;

public:
    Menu(SH1106* lcd);
    ~Menu() = default;
    static Menu* getInstance() { return instance; }

    void init();
    void update();

    void changeScene(Scene* scene);
    void input(uint8_t pin, Device* device);
};

#endif //GRAMOFON_MENU_H
