//
// Created by Tomáš Novák on 18.12.2022.
//
#include "scenes/save_scene.h"

#include "menu/menu.h"
#include "scenes/main_scene.h"

#include "PT2314.h"

#include "fonts/12x16_font.h"

void SaveScene::onStart() {
    lcd->drawText(font_12x16, "Saving...", 10, 26);
    lcd->sendBuffer();

    sleep_ms(100);
    PT2314::getInstance()->save();

    lcd->clear();
    lcd->drawText(font_12x16, "Saved", 34, 26);
    lcd->sendBuffer();

    sleep_ms(1000);
    PT2314::getInstance()->load();

    Menu::getInstance()->changeScene(new MainScene());
}

void SaveScene::onUpdate() {}
void SaveScene::onInput(uint8_t pin, Device *device) {}

