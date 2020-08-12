# SmartButton Library
SmartButton Library is used for handling various button events.
It supports button press, release, multiple click and button hold events.
Library is implemented in C++ with build-in Arduino port, but it is easy to
port to different architecture.

## Features
* asynchronous non-blocking API based on events callbacks
* hardware independent high-level architecture
* works well with digital, analogs and other signal input backends
* easy to use with Arduino boards
* unlimited number of buttons
* support multi-click events
* support press and release events
* support hold and long-hold events
* support auto-repeat events during hold and long-hold
* build-in debounce algorithm
* configurable timeouts

## Changelog

### 2020.08.12 - 0.3.0
- [x] hold auto-repeat periodic events added

### 2020.08.10 - 0.2.0
- [x] optional initialization by object-interface added

## Example

Single and double click example:

```cpp
#include <Arduino.h>

#include <SmartButton.h>

constexpr int BUTTON_PIN = 2;
constexpr int LED_PIN = 13;

using namespace smartbutton;

void eventCallback(SmartButton *button, SmartButton::Event event, int clickCounter)
{
    if (event == SmartButton::Event::CLICK) {   // Click event handler
        switch (clickCounter) {
        case 1:
            digitalWrite(LED_PIN, LOW);         // Single click will turn led off
            break;
        case 2:
            digitalWrite(LED_PIN, HIGH);        // Double click will turn led on
            break;
        default:
            break;
        }
    }
}

SmartButton button(BUTTON_PIN, SmartButton::InputType::NORMAL_HIGH);

void setup()
{
    pinMode(LED_PIN, OUTPUT);           // Digital output for led
    pinMode(BUTTON_PIN, INPUT_PULLUP);  // Digital input with pull-up resistors (normal high)

    button.begin(eventCallback);        // Initialize and register smart button
}

void loop()
{
    SmartButton::service();             // Asynchronous service routine, should be called periodically
}
```
