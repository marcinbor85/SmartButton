#include <Arduino.h>

#include <SmartButton.h>

#define BUTTON_PIN      2
#define LED_PIN         13

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

