//
// Created by Tomáš Novák on 17.12.2022.
//

#ifndef GRAMOFON_CHANGE_INPUT_SCENE_H
#define GRAMOFON_CHANGE_INPUT_SCENE_H

#include "menu/scene.h"

class ChangeInputScene : public Scene
{
public:
    ChangeInputScene() : Scene() {}
    ~ChangeInputScene() = default;

private:
    void onStart() override;
    void onUpdate() override;
    void onInput(uint8_t pin, Device* device) override;
};

#endif //GRAMOFON_CHANGE_INPUT_SCENE_H
