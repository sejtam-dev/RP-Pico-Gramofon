//
// Created by Tomáš Novák on 17.12.2022.
//
#include "scenes/change_input_scene.h"

#include "defines.h"

#include "rotary_encoder.h"

#include "menu/menu.h"
#include "scenes/main_scene.h"

#include "fonts/12x16_font.h"
#include "fonts/5x8_font.h"
#include "PT2314.h"

#include <iostream>

enum class Settings_SelectedInput : uint8_t {
    IN1 = 0, IN2 = 1, IN3 = 2
};

Settings_SelectedInput settingsSelectedInput = Settings_SelectedInput::IN1;

bool settingsInputSave = false;
bool settingsInputExit = false;

void ChangeInputScene::onStart() {
    lcd->clear();

    lcd->drawText(font_5x8, "INPUTS", 49, 2);
    lcd->drawLine(39, 10, 89, 10);

    lcd->sendBuffer();
}

void ChangeInputScene::onUpdate() {
    lcd->clear(0, 11, 128, 64);

    lcd->drawText(font_5x8, "GRAMOPHONE", 39, 15);
    lcd->drawText(font_5x8, "IN2", 56, 25);
    lcd->drawText(font_5x8, "IN3", 56, 35);

    switch(settingsSelectedInput) {
        case Settings_SelectedInput::IN1: {
            lcd->drawRect(37, 14, 89, 24);
            break;
        }
        case Settings_SelectedInput::IN2: {
            lcd->drawRect(37, 24, 89, 34);
            break;
        }
        case Settings_SelectedInput::IN3: {
            lcd->drawRect(37, 34, 89, 44);
            break;
        }
    }

    lcd->sendBuffer();

    if(settingsInputSave) {
        settingsInputSave = false;

        PT2314::getInstance()->setChannel(static_cast<uint8_t>(settingsSelectedInput));
        PT2314::getInstance()->update();

        Menu::getInstance()->changeScene(new MainScene());
    }

    if(settingsInputExit) {
        settingsInputExit = false;
        Menu::getInstance()->changeScene(new MainScene());
    }
}

void ChangeInputScene::onInput(uint8_t pin, Device* device) {
    switch(pin) {
        case ENCODER_PIN1:
        case ENCODER_PIN2: {
            auto *encoder = (RotaryEncoder *) device;
            EncoderDirection direction = encoder->getDirection();
            if (direction != EncoderDirection::NOROTATION) {
                auto number = static_cast<uint8_t>(settingsSelectedInput);
                if (direction == EncoderDirection::COUNTERCLOCKWISE) {
                    number++;
                } else if (direction == EncoderDirection::CLOCKWISE) {
                    number--;
                }

                if(number == 255)
                    number = 0;
                else if(number > 2)
                    number = 2;

                settingsSelectedInput = static_cast<Settings_SelectedInput>(number);
            }

            break;
        }
        case ENCODER_BUTTON: {
            auto *encoder = (RotaryEncoder *) device;
            ButtonState state = encoder->getButtonState();
            if(state == ButtonState::ShortPress) {
                settingsInputSave = true;
            } else if(state == ButtonState::LongPress) {
                settingsInputExit = true;
            }

            break;
        }
    }
}