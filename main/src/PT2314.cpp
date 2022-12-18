#include "PT2314.h"

#include "math.hpp"
#include "memory.hpp"

#include <hardware/gpio.h>

#include <cstring>
#include <iostream>

PT2314::PT2314(i2c_inst_t *inst, uint8_t scl, uint8_t sda, uint8_t address, uint32_t baudRate)
    : inst(inst), scl(scl), sda(sda), address(address), baudRate(baudRate)
{
    instance = this;

    data = new PT2314_Data();
    data->volume = 25;
    data->bass = 5;
    data->treble = 5;
}

void PT2314::init() {
    i2c_init(inst, baudRate);

    // Initialize I2C pins
    gpio_set_function(scl, GPIO_FUNC_I2C);
    gpio_set_function(sda, GPIO_FUNC_I2C);

    gpio_pull_up(scl);
    gpio_pull_up(sda);

    sleep_ms(1000);
    if(Memory::read<bool>(PT2314_MEMORY_OFFSET)) {
        std::cout << "Loading data..." << std::endl;
        load();
    } else {
        std::cout << "Saving data..." << std::endl;
        save();
    }
    sleep_ms(1000);

    update();
}

void PT2314::update() {
    sendCommand(PT2314_SPEAKER_L);
    sendCommand(PT2314_SPEAKER_R);

    sendCommand(0x40 + (data->loudness ? 4 : 0) + data->channel);

    uint8_t volume = 63 - map(data->volume, (uint8_t) 0, (uint8_t) 50, (uint8_t) 0, (uint8_t) 63);
    sendCommand(volume);

    uint8_t bass = map(data->bass, (uint8_t) 0, (uint8_t) 10, (uint8_t) 0, (uint8_t) 15);
    if(bass > 7) {
        bass--;

        bass = 15 - (bass - 7);
    }
    sendCommand(PT2314_BASS_TONE_CONTROL + bass);

    uint8_t treble = map(data->treble, (uint8_t) 0, (uint8_t) 10, (uint8_t) 0, (uint8_t) 15);
    if(treble > 7) {
        treble--;

        treble = 15 - (treble - 7);
    }
    sendCommand(PT2314_TREBLE_TONE_CONTROL + treble);
}

void PT2314::sendCommand(uint8_t command) {
    i2c_write_blocking(inst, address, &command, 1,false);
}

void PT2314::save() {
    Memory::write(PT2314_MEMORY_OFFSET, *data, 0);
}

void PT2314::load() {
    auto* readedData = Memory::read<PT2314_Data>(PT2314_MEMORY_OFFSET);

    std::cout << "Data: " << std::endl;
    uint8_t* data1 = (uint8_t*) readedData;
    for (size_t i = 0; i < 32; ++i) {
        printf("%02x", data1[i]);
        if (i % 16 == 15)
            printf("\n");
        else
            printf(" ");
    }
    std::cout << std::endl;

    data = static_cast<PT2314_Data*>(malloc(sizeof(PT2314_Data)));
    memcpy(data, readedData, sizeof(PT2314_Data));
}

PT2314* PT2314::instance = nullptr;
