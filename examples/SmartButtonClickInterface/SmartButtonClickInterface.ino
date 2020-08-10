#include <Arduino.h>

#include <SmartButton.h>

constexpr int BUTTON_PIN = 2;
constexpr int LED_PIN = 13;

using namespace smartbutton;

class ButtonInterface: public SmartButtonInterface {

public:
    virtual void event(SmartButton *button, SmartButton::Event event, int clickCounter)
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

    virtual bool isPressed(SmartButton *button)
    {
        return (digitalRead(BUTTON_PIN) == LOW) ? true : false;
    }
};

ButtonInterface buttonInterface;

SmartButton button(&buttonInterface);

void setup()
{
    pinMode(LED_PIN, OUTPUT);           // Digital output for led
    pinMode(BUTTON_PIN, INPUT_PULLUP);  // Digital input with pull-up resistors (normal high)

    button.begin();                     // Initialize and register smart button
}

void loop()
{
    SmartButton::service();             // Asynchronous service routine, should be called periodically
}

