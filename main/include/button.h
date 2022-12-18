#ifndef GRAMOFON_BUTTON_H
#define GRAMOFON_BUTTON_H

#include "pico.h"
#include "pico/types.h"

#include "device.h"

#define LONG_PRESS_TIME 1000

enum class ButtonState {
    ShortPress, // 0-1 sec
    LongPress,  // 1-unlimited sec
    Holding,
    None		// Not pressed
};

enum class ButtonType {
    PullUp,
    PullDown
};

class Button : public Device {
private:
    uint8_t pin;
    ButtonType type;

    uint32_t lastTime;
    ButtonState state = ButtonState::None;

public:
    explicit Button(uint8_t pin, ButtonType type = ButtonType::PullDown);

    void init();
    void tick();

    ButtonState getState();
};

#endif //GRAMOFON_BUTTON_H
