#ifndef GRAMOFON_PT2314_H
#define GRAMOFON_PT2314_H

#include "pico.h"
#include "pico/types.h"

#include <hardware/i2c.h>

#include "device.h"

#define PT2314_BASS_TONE_CONTROL    0x60 // 0b01100000
#define PT2314_TREBLE_TONE_CONTROL  0x70 // 0b01110000

#define PT2314_SPEAKER_L            0xC0 // 1100000000
#define PT2314_SPEAKER_R            0xE0 // 1110000000

#define PT2314_MEMORY_OFFSET        256

#define PT2314_MIN_VOLUME           0
#define PT2314_MAX_VOLUME           50

#define PT2314_MIN_TREBLE           0
#define PT2314_MAX_TREBLE           10

#define PT2314_MIN_BASS             0
#define PT2314_MAX_BASS             10

#define PT2314_MIN_CHANNEL          0
#define PT2314_MAX_CHANNEL          3

struct PT2314_Data {
    bool initialized = true;

    uint8_t volume = 25;
    uint8_t treble = 5;
    uint8_t bass = 5;

    bool loudness = false;
    bool muted = false;

    uint8_t channel = 0;
};

class PT2314 : public Device {
private:
    static PT2314* instance;

    i2c_inst* inst;
    uint8_t scl;
    uint8_t sda;
    uint8_t address;
    uint32_t baudRate;

    PT2314_Data* data;

    void sendCommand(uint8_t command);

public:
    PT2314(i2c_inst* inst, uint8_t scl, uint8_t sda, uint8_t address, uint32_t baudRate = 400000);
    static PT2314* getInstance() { return instance; }

    void init();
    void update();

    void save();
    void load();

    void setVolume(uint8_t volume) const {
        if(volume > PT2314_MAX_VOLUME)
            volume = PT2314_MAX_VOLUME;

        if(volume < PT2314_MIN_VOLUME)
            volume = PT2314_MIN_VOLUME;

        if(volume <= PT2314_MIN_VOLUME) {
            setMute(true);
        } else {
            setMute(false);
        }

        this->data->volume = volume;
    }
    uint8_t getVolume() const { return data->volume; }

    void setTreble(uint8_t treble) const {
        if(treble > PT2314_MAX_TREBLE)
            treble = PT2314_MAX_TREBLE;

        if(treble < PT2314_MIN_TREBLE)
            treble = PT2314_MIN_TREBLE;

        this->data->treble = treble;
    }
    uint8_t getTreble() const { return data->treble; }

    void setBass(uint8_t bass) const {
        if(bass > PT2314_MAX_BASS)
            bass = PT2314_MAX_BASS;

        if(bass < PT2314_MIN_BASS)
            bass = PT2314_MIN_BASS;

        this->data->bass = bass;
    }
    uint8_t getBass() const { return data->bass; }

    void setLoudness(bool loundess) const { this->data->loudness = loundess; }
    bool getLoudness() const { return data->loudness; }

    void setMute(bool muted) const { this->data->muted = muted; }
    bool getMute() const { return data->muted; }

    void setChannel(uint8_t channel) const {
        if(channel > PT2314_MAX_CHANNEL)
            channel = PT2314_MAX_CHANNEL;

        if(channel < PT2314_MIN_CHANNEL)
            channel = PT2314_MIN_CHANNEL;

        this->data->channel = channel;
    }
    uint8_t getChannel() const { return data->channel; }
};

#endif //GRAMOFON_PT2314_H
