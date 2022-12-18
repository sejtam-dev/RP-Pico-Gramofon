//
// Created by Tomáš Novák on 17.12.2022.
//

#ifndef GRAMOFON_SETTINGS_SCENE_H
#define GRAMOFON_SETTINGS_SCENE_H

#include "menu/scene.h"

class SettingsScene : public Scene
{
public:
    SettingsScene() : Scene() {}
    ~SettingsScene() = default;

private:
    void onStart() override;
    void onUpdate() override;
    void onInput(uint8_t pin, Device* device) override;
};

#endif //GRAMOFON_SETTINGS_SCENE_H
