#include <Arduino.h>

#include <SmartButton.h>

#define BUTTON_PIN      2
#define LED_PIN         13

using namespace smartbutton;

static bool blinking = true;
static unsigned long blinkingPeriod = 500;

void eventCallback(SmartButton *button, SmartButton::Event event, int clickCounter)
{
    if (event == SmartButton::Event::CLICK) {           // Click event handler
        switch (clickCounter) {
        case 1:
            blinkingPeriod = 100;                       // Single click will turn fast led blinking
            break;
        case 2:
            blinkingPeriod = 500;                       // Double click will turn slow led blinking
            break;
        default:
            break;
        }
    } else if (event == SmartButton::Event::HOLD) {     // Hold press event handler
        blinking = !blinking;                           // Turning on/off blinking service
    }
}

SmartButton button(BUTTON_PIN, SmartButton::InputType::NORMAL_HIGH);

void setup()
{
    pinMode(LED_PIN, OUTPUT);           // Digital output for led
    pinMode(BUTTON_PIN, INPUT_PULLUP);  // Digital input with pull-up resistors (normal high)

    button.begin(eventCallback);        // Initialize and register smart button
}

static void blinkService()
{
    static unsigned long lastMillis = 0;
    static bool blinkingState = false;

    if (!blinking)
        return;
    
    if (millis() - lastMillis < blinkingPeriod)
        return;

    lastMillis = millis();
    digitalWrite(LED_PIN, blinkingState);
    blinkingState = !blinkingState;
}

void loop()
{
    SmartButton::service();             // Asynchronous service routine, should be called periodically
    blinkService();                     // Asynchronous led blinking service
}

