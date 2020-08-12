#include <Arduino.h>
#include <SmartButton.h>

constexpr int BUTTON_PIN = 2;
constexpr int LED_PIN = 13;

using namespace smartbutton;

SmartButton button(BUTTON_PIN, SmartButton::InputType::NORMAL_HIGH);

void setup()
{
    pinMode(LED_PIN, OUTPUT);           // Digital output for led
    pinMode(BUTTON_PIN, INPUT_PULLUP);  // Digital input with pull-up resistors (normal high)

    // Initialize and register smart button
    button.begin([] (SmartButton *button, SmartButton::Event event, int clickCounter)
    {
        if (event == SmartButton::Event::CLICK) {   // Click event handler
            if (clickCounter == 1) {
                digitalWrite(LED_PIN, LOW);         // Single click will turn led off
            } else if (clickCounter == 2) {
                digitalWrite(LED_PIN, HIGH);        // Double click will turn led on
            }
        }
    });
}

void loop()
{
    SmartButton::service();   // Asynchronous service routine, should be called periodically
}
