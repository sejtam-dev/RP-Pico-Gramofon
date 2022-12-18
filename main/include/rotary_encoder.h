#ifndef GRAMOFON_ROTARY_ENCODER_H
#define GRAMOFON_ROTARY_ENCODER_H

#include "pico.h"
#include "pico/types.h"

#include "device.h"
#include "button.h"

#define LATCH0 0 // input state at position 0
#define LATCH3 3 // input state at position 3

enum class EncoderDirection {
    NOROTATION = 0,
    CLOCKWISE = 1,
    COUNTERCLOCKWISE = -1
};

const int8_t KNOBDIR[] = {
        0, -1, 1, 0,
        1, 0, 0, -1,
        -1, 0, 0, 1,
        0, 1, -1, 0
};

class RotaryEncoder : public Device {
public:
    enum class LatchMode
    {
        FOUR3 = 1, // 4 steps, Latch at position 3 only (compatible to older versions)
        FOUR0 = 2, // 4 steps, Latch at position 0 (reverse wirings)
        TWO03 = 3  // 2 steps, Latch at position 0 and 3
    };

private:
    uint8_t pin1, pin2;
    LatchMode mode;
    uint8_t oldState;

    Button button;

    uint32_t position = 0;        // Internal position (4 times _positionExt)
    uint32_t positionExt = 0;     // External position
    uint32_t positionExtPrev = 0; // External position (used only for direction checking)

    uint32_t positionExtTime;     // The time the last position change was detected.
    uint32_t positionExtTimePrev; // The time the previous position change was detected.

public:
    RotaryEncoder(uint8_t pin1, uint8_t pin2, uint8_t button_pin, LatchMode mode = LatchMode::TWO03);
    void init();

    EncoderDirection getDirection();
    ButtonState getButtonState();

    uint32_t getPosition() const;
    void setPosition(long newPosition);

    void tick();

    uint32_t getMillisBetweenRotations() const;
    uint32_t getRPM() const;
};

#endif //GRAMOFON_ROTARY_ENCODER_H
