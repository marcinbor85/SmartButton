#include <Arduino.h>

#include <SmartButton.h>

#define BUTTON1_PIN     2
#define BUTTON2_PIN     A0
#define LED_PIN         13

using namespace smartbutton;

static bool blinking = false;
static unsigned long blinkingPeriod = 500;

void event1Callback(SmartButton *button, SmartButton::Event event, int clickCounter)
{
    if (event == SmartButton::Event::CLICK) {   // Click event handler
        switch (clickCounter) {
        case 1:
            blinkingPeriod = 100;               // Single click will turn fast led blinking
            break;
        case 2:
            blinkingPeriod = 500;               // Double click will turn slow led blinking
            break;
        default:
            break;
        }
    }
}

void event2Callback(SmartButton *button, SmartButton::Event event, int clickCounter)
{
    if (event == SmartButton::Event::HOLD) {    // Hold press event handler
        blinking = !blinking;                   // Turning on/off blinking service
    }
}

bool isPressedHandler(SmartButton *button)
{
    int value = analogRead(BUTTON2_PIN);
    return (value >= 512) ? true : false;       // If ADC value >= 512 then button pressed
}

SmartButton button1(BUTTON1_PIN, SmartButton::InputType::NORMAL_HIGH);
SmartButton button2(isPressedHandler);

void setup()
{
    pinMode(LED_PIN, OUTPUT);           // Digital output for led
    pinMode(BUTTON1_PIN, INPUT_PULLUP); // Digital input with pull-up resistors (normal high)

    button1.begin(event1Callback);      // Initialize and register smart button
    button2.begin(event2Callback);      // Initialize and register smart button
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

