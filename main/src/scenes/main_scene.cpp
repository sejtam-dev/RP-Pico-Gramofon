//
// Created by Tomáš Novák on 17.12.2022.
//
#include "scenes/main_scene.h"

#include "defines.h"
#include "rotary_encoder.h"
#include "PT2314.h"
#include "bt_emitter.h"

#include "fonts/5x8_font.h"
#include "fonts/12x16_font.h"

#include "menu/menu.h"
#include "scenes/change_input_scene.h"
#include "scenes/settings_scene.h"

#include <string>
#include <iostream>

enum class ChangeScene {
    NONE, CHANGE_INPUT, SETTINGS
};

ChangeScene changeScene = ChangeScene::NONE;

void MainScene::onStart()
{
    lcd->clear();
    lcd->drawLine(0, 10, 128, 10);
    lcd->drawLine(64, 0, 64, 10);

    lcd->drawText(font_12x16, "Volume: ", 5, 32);
    lcd->sendBuffer();
}

void MainScene::onUpdate() {
    switch(PT2314::getInstance()->getChannel()) {
        case 0: {
            lcd->drawText(font_5x8, "GRAMOPHONE", 0, 0);
            break;
        }
        case 1: {
            lcd->drawText(font_5x8, "IN2", 0, 0);
            break;
        }
        case 2: {
            lcd->drawText(font_5x8, "IN3", 0, 0);
            break;
        }
    }

    if(BT_Emitter::getInstance()->isConnected())
        lcd->drawText(font_5x8, BT_Emitter::getInstance()->connectedDevice().name.c_str(), 66, 0);

    std::string text = std::to_string(PT2314::getInstance()->getVolume());

    lcd->clear(101, 32, 128, 48);
    lcd->drawText(font_12x16, text.c_str(), 101, 32);

    lcd->sendBuffer();

    switch(changeScene) {
        case ChangeScene::CHANGE_INPUT:
            changeScene = ChangeScene::NONE;
            Menu::getInstance()->changeScene(new ChangeInputScene());
            break;
        case ChangeScene::SETTINGS:
            changeScene = ChangeScene::NONE;
            Menu::getInstance()->changeScene(new SettingsScene());
            break;
        default:
            break;
    }
}

void MainScene::onInput(uint8_t pin, Device* device) {
    switch(pin) {
        case ENCODER_PIN1:
        case ENCODER_PIN2: {
            auto *encoder = (RotaryEncoder *) device;
            EncoderDirection direction = encoder->getDirection();
            if (direction != EncoderDirection::NOROTATION) {
                PT2314 *amplifier = PT2314::getInstance();

                uint8_t volume = amplifier->getVolume();
                if (direction == EncoderDirection::COUNTERCLOCKWISE && volume < PT2314_MAX_VOLUME) {
                    volume++;
                } else if (direction == EncoderDirection::CLOCKWISE && volume > PT2314_MIN_VOLUME) {
                    volume--;
                }

                amplifier->setVolume(volume);
                amplifier->update();
            }
            break;
        }
        case ENCODER_BUTTON: {
            auto *encoder = (RotaryEncoder *) device;
            ButtonState state = encoder->getButtonState();
            if(state == ButtonState::ShortPress) {
                std::cout << "Test1" << std::endl;
                changeScene = ChangeScene::SETTINGS;
            } else if(state == ButtonState::LongPress) {
                changeScene = ChangeScene::CHANGE_INPUT;
            }

            break;
        }
    }
}