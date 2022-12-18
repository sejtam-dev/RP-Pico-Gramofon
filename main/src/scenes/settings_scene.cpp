//
// Created by Tomáš Novák on 17.12.2022.
//
#include "scenes/settings_scene.h"

#include "defines.h"

#include "rotary_encoder.h"

#include "menu/menu.h"
#include "scenes/main_scene.h"
#include "scenes/change_value_scene.h"
#include "scenes/save_scene.h"
#include "scenes/bluetooth_scene.h"

#include "fonts/12x16_font.h"
#include "fonts/5x8_font.h"

#include <iostream>

enum class Settings_SelectedAction : uint8_t {
    BT = 0, TREBLE = 1, BASS = 2, SAVE = 3
};

Settings_SelectedAction settingsSelectedAction = Settings_SelectedAction::BT;

bool settingsNext = false;
bool settingsExit = false;

void SettingsScene::onStart() {
    lcd->clear();

    lcd->drawText(font_5x8, "SETTINGS", 45, 2);
    lcd->drawLine(39, 10, 89, 10);

    lcd->sendBuffer();
}

void SettingsScene::onUpdate() {
    lcd->clear(0, 11, 128, 64);

    lcd->drawText(font_5x8, "BT", 59, 15);
    lcd->drawText(font_5x8, "TREBLE", 49, 25);
    lcd->drawText(font_5x8, "BASS", 54, 35);
    lcd->drawText(font_5x8, "SAVE", 54, 45);

    switch(settingsSelectedAction) {
        case Settings_SelectedAction::BT: {
            lcd->drawRect(47, 14, 80, 24);

            if(settingsNext) {
                settingsNext = false;
                Menu::getInstance()->changeScene(new BluetoothScene());
                return;
            }

            break;
        }
        case Settings_SelectedAction::TREBLE: {
            lcd->drawRect(47, 24, 80, 34);

            if(settingsNext) {
                settingsNext = false;
                Menu::getInstance()->changeScene(new ChangeValueScene(ChangeValueType::TREBLE));
                return;
            }

            break;
        }
        case Settings_SelectedAction::BASS: {
            lcd->drawRect(47, 34, 80, 44);

            if(settingsNext) {
                settingsNext = false;
                Menu::getInstance()->changeScene(new ChangeValueScene(ChangeValueType::BASS));
                return;
            }

            break;
        }
        case Settings_SelectedAction::SAVE: {
            lcd->drawRect(47, 44, 80, 54);

            if(settingsNext) {
                settingsNext = false;
                Menu::getInstance()->changeScene(new SaveScene());
                return;
            }
            break;
        }
    }

    lcd->sendBuffer();

    if(settingsExit) {
        settingsExit = false;
        Menu::getInstance()->changeScene(new MainScene());
    }
}

void SettingsScene::onInput(uint8_t pin, Device* device) {
    switch(pin) {
        case ENCODER_PIN1:
        case ENCODER_PIN2: {
            auto *encoder = (RotaryEncoder *) device;
            EncoderDirection direction = encoder->getDirection();
            if (direction != EncoderDirection::NOROTATION) {
                auto number = static_cast<uint8_t>(settingsSelectedAction);
                if (direction == EncoderDirection::COUNTERCLOCKWISE) {
                    number++;
                } else if (direction == EncoderDirection::CLOCKWISE) {
                    number--;
                }

                if(number == 255)
                    number = 0;
                else if(number > 3)
                    number = 3;

                settingsSelectedAction = static_cast<Settings_SelectedAction>(number);
            }
            break;
        }
        case ENCODER_BUTTON: {
            auto *encoder = (RotaryEncoder *) device;
            ButtonState state = encoder->getButtonState();
            if(state == ButtonState::ShortPress) {
                settingsNext = true;
            } else if(state == ButtonState::LongPress) {
                std::cout << "Test2" << std::endl;
                settingsExit = true;
            }

            break;
        }
    }
}