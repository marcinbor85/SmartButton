#ifndef SMART_BUTTON_DEFS_H
#define SMART_BUTTON_DEFS_H

#include <Arduino.h>

namespace smartbutton {

constexpr unsigned long DEFAULT_DEBOUNCE_TIMEOUT = 20UL;
constexpr unsigned long DEFAULT_CLICK_TIMEOUT = 500UL;
constexpr unsigned long DEFAULT_HOLD_TIMEOUT = 1000UL;
constexpr unsigned long DEFAULT_LONG_HOLD_TIMEOUT = 2000UL;

constexpr unsigned long (*getTickValue)() = millis;
constexpr bool (*getGpioState)(int) = digitalRead;

};

#endif /* SMART_BUTTON_DEFS_H */
