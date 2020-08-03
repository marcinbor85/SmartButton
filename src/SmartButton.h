#ifndef SMART_BUTTON_H
#define SMART_BUTTON_H

#include "SmartButtonDefs.h"

namespace smartbutton {

class SmartButton {

public:
    enum class State {
        RELEASED,
        PRESSED,
        HOLD,
        LONG_HOLD
    };

    enum class Event {
        RELEASED,
        PRESSED,
        CLICK,
        HOLD,
        LONG_HOLD,
    };

    enum class InputType {
        NORMAL_HIGH,
        NORMAL_LOW
    };

    using IsPressedHandler = bool (*)(SmartButton *button);
    using EventCallback = void (*)(SmartButton *button, SmartButton::Event event, int clickCounter);

    explicit SmartButton(int pin, SmartButton::InputType inputType);
    explicit SmartButton(SmartButton::IsPressedHandler isPressedHandler);

    SmartButton::State getState(void);
    bool isPressedDebounced(void);

    void begin(SmartButton::EventCallback eventCallback);
    void end();

    static void service();

private:
    SmartButton() = delete;
    SmartButton(const SmartButton&) = delete;
    SmartButton& operator=(const SmartButton&) = delete;

    SmartButton(
        int pin,
        SmartButton::InputType inputType,
        SmartButton::IsPressedHandler isPressedHandler,
        unsigned long debounceTimeout,
        unsigned long clickTimeout,
        unsigned long holdTimeout,
        unsigned long longHoldTimeout
    );

    void process();
    void debounce();

    bool getInputState();

    SmartButton::IsPressedHandler isPressedHandler;
    SmartButton::EventCallback eventCallback;

    const unsigned long debounceTimeout;
    const unsigned long clickTimeout;
    const unsigned long holdTimeout;
    const unsigned long longHoldTimeout;

    const int pin;
    const SmartButton::InputType inputType;

    unsigned long pressTick;
    unsigned long debounceTick;

    bool pressedFlag;

    int clickCounter;
    SmartButton::State state;

    SmartButton *next;
};

extern SmartButton *_smartButtons;

};

#endif /* SMART_BUTTON_H */
