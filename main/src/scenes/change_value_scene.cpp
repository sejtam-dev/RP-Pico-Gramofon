//
// Created by Tomáš Novák on 18.12.2022.
//
#include "scenes/change_value_scene.h"

#include "defines.h"
#include "rotary_encoder.h"
#include "PT2314.h"

#include "fonts/12x16_font.h"

#include "menu/menu.h"
#include "scenes/settings_scene.h"
#include "math.hpp"

#include <string>
#include <iostream>

bool changeValue_Exit = false;

ChangeValueScene::ChangeValueScene(ChangeValueType type) : Scene() {
    this->type = type;
}

void ChangeValueScene::onStart()
{
    if(type == ChangeValueType::TREBLE) {
        lcd->drawText(font_12x16, "Treble: ", 5, 26);
    } else if (type == ChangeValueType::BASS) {
        lcd->drawText(font_12x16, "Bass: ", 5, 26);
    }

    lcd->sendBuffer();
}

void ChangeValueScene::onUpdate() {

    std::string text;
    if(type == ChangeValueType::TREBLE) {
        text += std::to_string(map((int)PT2314::getInstance()->getTreble(), 0, 10, -5, 5));
    } else if(type == ChangeValueType::BASS) {
        text += std::to_string(map((int)PT2314::getInstance()->getBass(), 0, 10, -5, 5));
    }

    lcd->clear(101, 26, 128, 48);
    lcd->drawText(font_12x16, text.c_str(), 101, 26);

    lcd->sendBuffer();

    if(changeValue_Exit) {
        changeValue_Exit = false;

        Menu::getInstance()->changeScene(new SettingsScene());
    }
}

void ChangeValueScene::onInput(uint8_t pin, Device* device) {
    switch(pin) {
        case ENCODER_PIN1:
        case ENCODER_PIN2: {
            auto *encoder = (RotaryEncoder *) device;
            EncoderDirection direction = encoder->getDirection();
            if (direction != EncoderDirection::NOROTATION) {
                PT2314 *amplifier = PT2314::getInstance();

                if(type == ChangeValueType::TREBLE) {
                    uint8_t treble = amplifier->getTreble();
                    if (direction == EncoderDirection::COUNTERCLOCKWISE && treble < PT2314_MAX_TREBLE) {
                        treble++;
                    } else if (direction == EncoderDirection::CLOCKWISE && treble > PT2314_MIN_TREBLE) {
                        treble--;
                    }
                    amplifier->setTreble(treble);
                } else if(type == ChangeValueType::BASS) {
                    uint8_t bass = amplifier->getBass();
                    if (direction == EncoderDirection::COUNTERCLOCKWISE && bass < PT2314_MAX_BASS) {
                        bass++;
                    } else if (direction == EncoderDirection::CLOCKWISE && bass > PT2314_MIN_BASS) {
                        bass--;
                    }
                    amplifier->setBass(bass);
                }

                amplifier->update();
            }
            break;
        }
        case ENCODER_BUTTON: {
            auto *encoder = (RotaryEncoder *) device;
            ButtonState state = encoder->getButtonState();
            if(state == ButtonState::ShortPress || state == ButtonState::LongPress) {
                changeValue_Exit = true;
            }

            break;
        }
    }
}
