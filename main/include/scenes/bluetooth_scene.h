//
// Created by Tomáš Novák on 18.12.2022.
//

#ifndef GRAMOFON_BLUETOOTH_SCENE_H
#define GRAMOFON_BLUETOOTH_SCENE_H

#include "menu/scene.h"

class BluetoothScene : public Scene
{
public:
    BluetoothScene() : Scene() {}
    ~BluetoothScene() = default;

private:
    void onStart() override;
    void onUpdate() override;
    void onInput(uint8_t pin, Device* device) override;
};

#endif //GRAMOFON_BLUETOOTH_SCENE_H
