#include <Arduino.h>

#include <SmartButton.h>

constexpr int BUTTON1_PIN = 2;
constexpr int BUTTON2_PIN = A0;
constexpr int LED_PIN = 13;

constexpr unsigned long BLINKING_FAST = 100UL;
constexpr unsigned long BLINKING_SLOW = 500UL;

using namespace smartbutton;

static bool blinking = true;
static unsigned long blinkingPeriod = BLINKING_SLOW;

void event1Callback(SmartButton *button, SmartButton::Event event, int clickCounter)
{
    if (event == SmartButton::Event::CLICK) {   // Click event handler
        switch (clickCounter) {
        case 1:
            blinkingPeriod = BLINKING_FAST;     // Single click will turn fast led blinking
            break;
        case 2:
            blinkingPeriod = BLINKING_SLOW;     // Double click will turn slow led blinking
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

