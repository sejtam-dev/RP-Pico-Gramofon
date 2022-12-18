//
// Created by Tomáš Novák on 17.12.2022.
//

#ifndef GRAMOFON_MAIN_SCENE_H
#define GRAMOFON_MAIN_SCENE_H

#include "menu/scene.h"

class MainScene : public Scene
{
public:
    MainScene() : Scene() {}
    ~MainScene() = default;

private:
    void onStart() override;
    void onUpdate() override;
    void onInput(uint8_t pin, Device* device) override;
};

#endif //GRAMOFON_MAIN_SCENE_H
