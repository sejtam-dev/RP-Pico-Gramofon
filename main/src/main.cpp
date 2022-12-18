#include "pico.h"
#include "pico/types.h"
#include "pico/stdio.h"

#include <hardware/i2c.h>
#include <hardware/uart.h>
#include <hardware/irq.h>
#include <hardware/gpio.h>

#include <iostream>

#include "defines.h"
#include "rotary_encoder.h"
#include "PT2314.h"
#include "bt_emitter.h"
#include "SH1106.h"

#include "menu/menu.h"
#include "scenes/main_scene.h"

#include "fonts/5x8_font.h"
#include "fonts/12x16_font.h"
#include "memory.hpp"

PT2314 amplifier(AMPLIFIER_I2C, AMPLIFIER_SCL, AMPLIFIER_SDA, AMPLIFIER_ADDRESS);
RotaryEncoder encoder(ENCODER_PIN1, ENCODER_PIN2, ENCODER_BUTTON, RotaryEncoder::LatchMode::FOUR3);
BT_Emitter btEmitter(BLUETOOTH_UART, BLUETOOTH_TX, BLUETOOTH_RX);
SH1106 display(DISPLAY_I2C, DISPLAY_SCL, DISPLAY_SDA, DISPLAY_ADDRESS);
Menu menu(&display);

void gpio_callback(uint gpio, uint32_t events);

int main() {
    stdio_init_all();

    sleep_ms(2000);
    std::cout << "Init" << std::endl;

    amplifier.init();
    encoder.init();

    btEmitter.init();
    std::cout << "Bluetooth: " << (btEmitter.isReady() ? "Ready" : "Not Ready") << std::endl;

    display.init();
    display.setOrientation(true);

    display.clear();
    display.drawText(font_12x16, "Loading...", 0, 26);
    display.sendBuffer();

    sleep_ms(2000);
    btEmitter.disconnect();

    gpio_set_irq_enabled_with_callback(ENCODER_BUTTON, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, gpio_callback);
    gpio_set_irq_enabled(ENCODER_PIN1, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(ENCODER_PIN2, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);

    menu.changeScene(new MainScene());

    while(true) {
        menu.update();
    }
}

void gpio_callback(uint gpio, uint32_t events) {
    Device* device;

    switch(gpio) {
        case ENCODER_PIN1:
        case ENCODER_PIN2:
        case ENCODER_BUTTON:
            encoder.tick();
            device = &encoder;
            break;
    }

    menu.input(gpio, device);
}