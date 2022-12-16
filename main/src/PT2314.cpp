#include "PT2314.h"

#include "math.hpp"
#include "memory.hpp"

#include <hardware/gpio.h>
#include <cstring>

PT2314::PT2314(i2c_inst_t *inst, uint8_t scl, uint8_t sda, uint8_t address, uint32_t baudRate)
    : inst(inst), scl(scl), sda(sda), address(address), baudRate(baudRate), data(new PT2314_Data())
{
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

    if(Memory::read<bool>(PT2314_MEMORY_OFFSET)) {
        load();
    } else {
        save();
    }

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
    }
    sendCommand(PT2314_BASS_TONE_CONTROL + bass);

    uint8_t treble = map(data->treble, (uint8_t) 0, (uint8_t) 10, (uint8_t) 0, (uint8_t) 15);
    if(treble > 7) {
        treble--;
    }
    sendCommand(PT2314_TREBLE_TONE_CONTROL + treble);
}

void PT2314::sendCommand(uint8_t command) {
    i2c_write_blocking(inst, address, &command, 1,false);
}

void PT2314::save() {
    Memory::write(PT2314_MEMORY_OFFSET, data, sizeof(PT2314_Data));
}

void PT2314::load() {
    auto* readedData = Memory::read<PT2314_Data>(0);

    data = static_cast<PT2314_Data*>(malloc(sizeof(PT2314_Data)));
    memcpy(data, readedData, sizeof(PT2314_Data));
}
