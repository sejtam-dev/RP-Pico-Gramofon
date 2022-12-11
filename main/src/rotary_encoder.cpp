//
// Created by Tomáš Novák on 10.12.2022.
//

#include "rotary_encoder.h"

#include <pico/time.h>
#include <hardware/gpio.h>

#include <string>

RotaryEncoder::RotaryEncoder(const uint8_t pin1, const uint8_t pin2, const uint8_t button_pin, const LatchMode mode)
    : pin1(pin1), pin2(pin2), mode(mode), button(button_pin), oldState(0), positionExt(0), positionExtTime(0), positionExtTimePrev(0) { }

void RotaryEncoder::init()
{
    gpio_init(pin1);
    gpio_set_dir(pin1, GPIO_IN);
    gpio_put(pin1, false);
    gpio_pull_up(pin1);

    gpio_init(pin2);
    gpio_set_dir(pin2, GPIO_IN);
    gpio_put(pin2, false);
    gpio_pull_up(pin2);

    const uint8_t sig1 = gpio_get(pin1);
    const uint8_t sig2 = gpio_get(pin2);
    oldState = sig1 | (sig2 << 1);

    button.init();
}

EncoderDirection RotaryEncoder::getDirection()
{
    EncoderDirection ret = EncoderDirection::NOROTATION;

    if (positionExtPrev > positionExt)
    {
        ret = EncoderDirection::COUNTERCLOCKWISE;
    }
    else if (positionExtPrev < positionExt)
    {
        ret = EncoderDirection::CLOCKWISE;
    }

    positionExtPrev = positionExt;
    return ret;
}

ButtonState RotaryEncoder::getButtonState()
{
    return button.getState();
}

uint32_t RotaryEncoder::getPosition() const
{
    return positionExt;
}
void RotaryEncoder::setPosition(const long newPosition)
{
    switch (mode)
    {
        case LatchMode::FOUR3:
        case LatchMode::FOUR0:
            position = ((newPosition << 2) | (position & 0x03L));
            positionExt = newPosition;
            positionExtPrev = newPosition;

            break;

        case LatchMode::TWO03:
            position = ((newPosition << 1) | (position & 0x01L));
            positionExt = newPosition;
            positionExtPrev = newPosition;

            break;
    }
}

void RotaryEncoder::tick()
{
    button.tick();

    const int sig1 = gpio_get(pin1);
    const int sig2 = gpio_get(pin2);
    const int8_t thisState = sig1 | (sig2 << 1);

    if (oldState != thisState) {
        position += KNOBDIR[thisState | (oldState << 2)];
        oldState = thisState;

        switch (mode) {
            case LatchMode::FOUR3:
                if (thisState == LATCH3) {
                    // The hardware has 4 steps with a latch on the input state 3
                    positionExt = position >> 2;
                    positionExtTimePrev = positionExtTime;
                    positionExtTime = to_ms_since_boot(get_absolute_time());
                }
                break;

            case LatchMode::FOUR0:
                if (thisState == LATCH0) {
                    // The hardware has 4 steps with a latch on the input state 0
                    positionExt = position >> 2;
                    positionExtTimePrev = positionExtTime;
                    positionExtTime = to_ms_since_boot(get_absolute_time());
                }
                break;

            case LatchMode::TWO03:
                if ((thisState == LATCH0) || (thisState == LATCH3)) {
                    // The hardware has 2 steps with a latch on the input state 0 and 3
                    positionExt = position >> 1;
                    positionExtTimePrev = positionExtTime;
                    positionExtTime = to_ms_since_boot(get_absolute_time());
                }
                break;
        }
    }
}


uint32_t RotaryEncoder::getMillisBetweenRotations() const
{
    return (positionExtTime - positionExtTimePrev);
}

uint32_t RotaryEncoder::getRPM() const
{
    const uint32_t timeBetweenLastPositions = positionExtTime - positionExtTimePrev;
    const uint32_t timeToLastPosition = to_ms_since_boot(get_absolute_time()) - positionExtTime;
    const uint32_t t = std::max(timeBetweenLastPositions, timeToLastPosition);
    return 60000 / ((float)(t * 20));
}