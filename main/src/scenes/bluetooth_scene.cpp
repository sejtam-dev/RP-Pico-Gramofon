//
// Created by Tomáš Novák on 18.12.2022.
//
#include "scenes/bluetooth_scene.h"

#include "defines.h"

#include "menu/menu.h"
#include "scenes/main_scene.h"

#include "rotary_encoder.h"
#include "bt_emitter.h"

#include "fonts/5x8_font.h"
#include "fonts/12x16_font.h"

#include <iostream>

std::vector<BT_Device> bt_SearchedDevices;
uint8_t bt_Selected = 0;

bool bt_Exit = false;
bool bt_Connect = false;

void BluetoothScene::onStart() {
    if(BT_Emitter::getInstance()->isConnected()) {
        lcd->drawText(font_12x16, "Disconnecting...", 0, 26);
        lcd->sendBuffer();

        BT_Emitter::getInstance()->disconnect();
    }


    lcd->clear();
    lcd->drawText(font_12x16, "Searching...", 0, 26);
    lcd->sendBuffer();

    bt_SearchedDevices.clear();
    bt_SearchedDevices = BT_Emitter::getInstance()->search();

    if(bt_SearchedDevices.empty()) {
        lcd->clear();
        lcd->drawText(font_12x16, "No found", 0, 26);
        lcd->sendBuffer();

        sleep_ms(1000);

        Menu::getInstance()->changeScene(new MainScene());
    }
}

void BluetoothScene::onUpdate() {
    lcd->clear();

    std::string selectedText = "SELECT " + std::to_string(bt_Selected + 1) + "/" + std::to_string(bt_SearchedDevices.size());
    lcd->drawText(font_5x8, selectedText.c_str(), 40, 2);
    lcd->drawLine(39, 10, 89, 10);

    std::string deviceText = "Device " + std::to_string(bt_Selected) + ": ";
    lcd->drawText(font_5x8, deviceText.c_str(), 10, 20);

    BT_Device device = bt_SearchedDevices[bt_Selected];

    std::string deviceName = "Name: " + device.name;
    lcd->drawText(font_5x8, deviceName.c_str(), 10, 40);

    std::string deviceMac = "Mac: " + device.macAddress;
    lcd->drawText(font_5x8, deviceMac.c_str(), 10, 46);

    lcd->sendBuffer();

    if(bt_Exit) {
        bt_Exit = false;

        Menu::getInstance()->changeScene(new MainScene());
        return;
    }

    if(bt_Connect) {
        bt_Connect = false;

        lcd->clear();
        lcd->drawText(font_12x16, "Connecting...", 0, 26);
        lcd->sendBuffer();

        bool isConnected = BT_Emitter::getInstance()->connect(device);

        lcd->clear();
        if(isConnected) {
            lcd->drawText(font_12x16, "Connected", 0, 26);
        } else {
            lcd->drawText(font_12x16, "Failed", 0, 26);
        }
        lcd->sendBuffer();

        bt_Selected = 0;
        sleep_ms(1000);

        Menu::getInstance()->changeScene(new MainScene());
        return;
    }
}

void BluetoothScene::onInput(uint8_t pin, Device *device) {
    switch(pin) {
        case ENCODER_PIN1:
        case ENCODER_PIN2: {
            auto *encoder = (RotaryEncoder *) device;
            EncoderDirection direction = encoder->getDirection();
            if (direction != EncoderDirection::NOROTATION) {
                if (direction == EncoderDirection::COUNTERCLOCKWISE) {
                    bt_Selected++;
                } else if (direction == EncoderDirection::CLOCKWISE) {
                    bt_Selected--;
                }

                if(bt_Selected == 255)
                    bt_Selected = 0;

                if(bt_Selected > bt_SearchedDevices.size() - 1)
                    bt_Selected = bt_SearchedDevices.size() - 1;
            }
            break;
        }
        case ENCODER_BUTTON: {
            auto *encoder = (RotaryEncoder *) device;
            ButtonState state = encoder->getButtonState();
            if(state == ButtonState::ShortPress) {
                bt_Connect = true;
            } else if(state == ButtonState::LongPress) {
                bt_Exit = true;
            }

            break;
        }
    }

}
