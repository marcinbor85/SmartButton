#include <Arduino.h>

#include <SmartButton.h>

constexpr int BUTTON_PIN = A0;
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

bool isPressedHandler(SmartButton *button)
{
    int value = analogRead(BUTTON_PIN);
    return (value >= 512) ? true : false;       // If ADC value >= 512 then button pressed
}

SmartButton button(isPressedHandler);

void setup()
{
    pinMode(LED_PIN, OUTPUT);           // Digital output for led

    button.begin(eventCallback);        // Initialize and register smart button
}

void loop()
{
    SmartButton::service();             // Asynchronous service routine, should be called periodically
}

