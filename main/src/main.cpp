//
// Created by Tomáš Novák on 10.12.2022.
//

#include "pico.h"
#include "pico/types.h"
#include "pico/stdio.h"

#include <hardware/i2c.h>
#include <hardware/uart.h>
#include <iostream>

#include "rotary_encoder.h"
#include "PT2314.h"
#include "bt_emitter.h"

#define DISPLAY_I2C i2c1
#define DISPLAY_SDA 2
#define DISPLAY_SCL 3
#define DISPLAY_ADDRESS 0x3C

#define AMPLIFIER_I2C i2c0
#define AMPLIFIER_SDA 0
#define AMPLIFIER_SCL 1
#define AMPLIFIER_ADDRESS 0x44

#define ENCODER_PIN1 8
#define ENCODER_PIN2 9
#define ENCODER_BUTTON 7

#define BLUETOOTH_UART uart0
#define BLUETOOTH_TX 12
#define BLUETOOTH_RX 13

int main() {
    stdio_init_all();

    PT2314 amplifier(AMPLIFIER_I2C, AMPLIFIER_SCL, AMPLIFIER_SDA, AMPLIFIER_ADDRESS);
    RotaryEncoder encoder(ENCODER_PIN1, ENCODER_PIN2, ENCODER_BUTTON, RotaryEncoder::LatchMode::FOUR3);
    BT_Emitter btEmitter(BLUETOOTH_UART, BLUETOOTH_TX, BLUETOOTH_RX);

    sleep_ms(2000);
    std::cout << "Init" << std::endl;

    amplifier.init();
    encoder.init();
    btEmitter.init();

    std::cout << "Bluetooth: " << btEmitter.isReady() << std::endl;

    std::vector<BT_Device> search = btEmitter.search();
    if(!search.empty()) {
        for (BT_Device &device: search) {
            std::cout << "Device: " << device.name << ", Mac: " << device.macAddress << std::endl;
        }

        //btEmitter.connect(search[0]);
    }

    while(true) {
        encoder.tick();

        EncoderDirection direction = encoder.getDirection();
        if(direction != EncoderDirection::NOROTATION) {
            uint8_t volume = amplifier.getVolume();
            if(direction == EncoderDirection::COUNTERCLOCKWISE && volume < PT2314_MAX_VOLUME) {
                volume++;
            } else if (direction == EncoderDirection::CLOCKWISE && volume > PT2314_MIN_VOLUME) {
                volume--;
            }

            amplifier.setVolume(volume);
            amplifier.update();

            std::cout << "Volume: " << unsigned(volume) << std::endl;
        }
    }
}