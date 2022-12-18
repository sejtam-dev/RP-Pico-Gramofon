//
// Created by Tomáš Novák on 18.12.2022.
//

#ifndef GRAMOFON_SAVE_SCENE_H
#define GRAMOFON_SAVE_SCENE_H

#include "menu/scene.h"

class SaveScene : public Scene
{
public:
    SaveScene() : Scene() {}
    ~SaveScene() = default;

private:
    void onStart() override;
    void onUpdate() override;
    void onInput(uint8_t pin, Device* device) override;
};

#endif //GRAMOFON_SAVE_SCENE_H
