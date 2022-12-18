//
// Created by Tomáš Novák on 17.12.2022.
//

#ifndef GRAMOFON_SCENE_H
#define GRAMOFON_SCENE_H

#include "pico.h"
#include "pico/types.h"

#include "SH1106.h"

class Scene
{
protected:
    SH1106* lcd;

public:
    Scene() = default;
    ~Scene() = default;

    void setLCD(SH1106* lcd);

    virtual void onStart() = 0;
    virtual void onUpdate() = 0;
    virtual void onInput(uint8_t pin, Device* device) = 0;
};

#endif //GRAMOFON_SCENE_H
