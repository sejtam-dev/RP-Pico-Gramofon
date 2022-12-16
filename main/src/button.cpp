#include <pico/time.h>
#include <hardware/gpio.h>

#include "button.h"

Button::Button(uint8_t pin, ButtonType type)
    : pin(pin), type(type), lastTime(0) { }

void Button::init()
{
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_put(pin, false);

    if(type == ButtonType::PullDown)
    {
        gpio_pull_down(pin);
    }
    else if (type == ButtonType::PullUp)
    {
        gpio_pull_up(pin);
    }

}

void Button::tick()
{
    switch(state)
    {
        case ButtonState::None:
            if (gpio_get(pin)) {
                lastTime = to_ms_since_boot(get_absolute_time());

                state = ButtonState::Holding;
            }

            break;
        case ButtonState::ShortPress:
        case ButtonState::LongPress:
            break;
        case ButtonState::Holding:
            if(!gpio_get(pin))
            {
                const uint32_t time = to_ms_since_boot(get_absolute_time());
                if (lastTime + LONG_PRESS_TIME >= time)
                {
                    state = ButtonState::ShortPress;
                }
                else
                {
                    state = ButtonState::LongPress;
                }
            }

            break;
    }
}

ButtonState Button::getState()
{
    const ButtonState cache = state;

    if (state == ButtonState::LongPress || state == ButtonState::ShortPress)
        state = ButtonState::None;

    return cache;
}