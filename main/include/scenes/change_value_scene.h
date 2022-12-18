//
// Created by Tomáš Novák on 18.12.2022.
//

#ifndef GRAMOFON_CHANGE_VALUE_SCENE_H
#define GRAMOFON_CHANGE_VALUE_SCENE_H

#include "menu/scene.h"

enum class ChangeValueType {
    TREBLE, BASS
};

class ChangeValueScene : public Scene {
public:
    ChangeValueScene(ChangeValueType type);
    ~ChangeValueScene() = default;

private:
    ChangeValueType type;

    void onStart() override;
    void onUpdate() override;
    void onInput(uint8_t pin, Device* device) override;
};

#endif //GRAMOFON_CHANGE_VALUE_SCENE_H
