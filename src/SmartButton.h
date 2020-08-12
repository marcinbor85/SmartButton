#ifndef SMART_BUTTON_H
#define SMART_BUTTON_H

#include "SmartButtonDefs.h"

namespace smartbutton {

class SmartButtonInterface;

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
        HOLD_REPEAT,
        LONG_HOLD,
        LONG_HOLD_REPEAT
    };

    enum class InputType {
        NORMAL_HIGH,
        NORMAL_LOW
    };

    using IsPressedHandler = bool (*)(SmartButton *button);
    using EventCallback = void (*)(SmartButton *button, SmartButton::Event event, int clickCounter);

    explicit SmartButton(int pin, SmartButton::InputType inputType = SmartButton::InputType::NORMAL_HIGH);
    explicit SmartButton(SmartButton::IsPressedHandler isPressedHandler);
    explicit SmartButton(bool *isPressedFlag);
    explicit SmartButton(SmartButtonInterface *interface);

    SmartButton::State getState(void);
    bool isPressedDebounced(void);

    void begin(SmartButton::EventCallback eventCallback, void *context = NULL);
    void begin(void *context = NULL);
    void end();

    void* getContext();

    void process();
    static void service();

    SmartButton(
        int pin,
        SmartButton::InputType inputType,
        SmartButton::IsPressedHandler isPressedHandler,
        bool *isPressedFlag,
        SmartButtonInterface *interface,
        unsigned long debounceTimeout = DEFAULT_DEBOUNCE_TIMEOUT,
        unsigned long clickTimeout = DEFAULT_CLICK_TIMEOUT,
        unsigned long holdTimeout = DEFAULT_HOLD_TIMEOUT,
        unsigned long longHoldTimeout = DEFAULT_LONG_HOLD_TIMEOUT,
        unsigned long holdRepeatPeriod = DEFAULT_HOLD_REPEAT_PERIOD,
        unsigned long longHoldRepeatPeriod = DEFAULT_LONG_HOLD_REPEAT_PERIOD
    );

private:
    SmartButton() = delete;
    SmartButton(const SmartButton&) = delete;
    SmartButton& operator=(const SmartButton&) = delete;

    void debounce();
    void callEvent(SmartButton::Event event);

    bool getInputState();

    bool *isPressedFlag;
    SmartButtonInterface *interface;
    SmartButton::IsPressedHandler isPressedHandler;
    SmartButton::EventCallback eventCallback;
    void *context;

    const unsigned long debounceTimeout;
    const unsigned long clickTimeout;
    const unsigned long holdTimeout;
    const unsigned long longHoldTimeout;
    const unsigned long holdRepeatPeriod;
    const unsigned long longHoldRepeatPeriod;

    const int pin;
    const SmartButton::InputType inputType;

    unsigned long pressTick;
    unsigned long debounceTick;
    unsigned long repeatTick;

    bool pressedFlag;

    int clickCounter;
    SmartButton::State state;

    SmartButton *next;
};

class SmartButtonInterface {

public:
    virtual bool isPressed(SmartButton *button) = 0;
    virtual void event(SmartButton *button, SmartButton::Event event, int clickCounter) = 0;

};

extern SmartButton *_smartButtons;

};

#endif /* SMART_BUTTON_H */
