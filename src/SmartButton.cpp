#include "SmartButton.h"

namespace smartbutton {

SmartButton *_smartButtons = NULL;

SmartButton::SmartButton(
    int pin,
    SmartButton::InputType inputType,
    SmartButton::IsPressedHandler isPressedHandler, 
    unsigned long debounceTimeout = DEFAULT_DEBOUNCE_TIMEOUT,
    unsigned long clickTimeout = DEFAULT_CLICK_TIMEOUT,
    unsigned long holdTimeout = DEFAULT_HOLD_TIMEOUT,
    unsigned long longHoldTimeout = DEFAULT_LONG_HOLD_TIMEOUT):
        pin(pin),
        inputType(inputType),
        isPressedHandler(isPressedHandler),
        debounceTimeout(debounceTimeout),
        clickTimeout(clickTimeout),
        holdTimeout(holdTimeout),
        longHoldTimeout(longHoldTimeout)
{

}

SmartButton::SmartButton(SmartButton::IsPressedHandler isPressed): SmartButton(-1, SmartButton::InputType::NORMAL_HIGH, isPressed)
{
    
}

SmartButton::SmartButton(int pin, SmartButton::InputType inputType): SmartButton(pin, inputType, NULL)
{
    
}

void SmartButton::begin(SmartButton::EventCallback eventCallback)
{
    this->eventCallback = eventCallback;

    this->debounceTick = 0;
    this->pressTick = 0;
    this->clickCounter = 0;

    this->pressedFlag = false;
    this->state = SmartButton::State::RELEASED;

    this->next = _smartButtons;
    _smartButtons = this;
}

void SmartButton::end()
{
    SmartButton *button = _smartButtons;
    SmartButton *prev = NULL;

    while (button != NULL) {
        if (button == this) {
            if (prev == NULL) {
                _smartButtons = button->next;
            } else {
                prev->next = button->next;
            }
            break;
        }
        prev = button;
        button = button->next;
    }
}

SmartButton::State SmartButton::getState()
{
    return this->state;
}

bool SmartButton::isPressedDebounced()
{
    return this->pressedFlag;
}

bool SmartButton::getInputState()
{
    bool s;
    if (this->pin >= 0) {
        s = getGpioState(this->pin);
        if (this->inputType == SmartButton::InputType::NORMAL_HIGH) {
            return !s;
        } else {
            return s;
        }
    } else {
        return this->isPressedHandler(this);
    }
}

void SmartButton::debounce()
{
    if (this->getInputState() == this->pressedFlag) {
        this->debounceTick = getTickValue();
    } else if (getTickValue() - this->debounceTick >= this->debounceTimeout) {
        this->debounceTick = getTickValue();
        this->pressedFlag = !this->pressedFlag;
        this->eventCallback(this, (this->pressedFlag != false) ? SmartButton::Event::PRESSED : SmartButton::Event::RELEASED, this->clickCounter);
    }
}

void SmartButton::process()
{
    this->debounce();

    switch (this->state) {
    case SmartButton::State::RELEASED:
        if (this->pressedFlag == false) {
            if ((this->clickCounter != 0) && (getTickValue() - this->pressTick > this->clickTimeout)) {
                this->eventCallback(this, SmartButton::Event::CLICK, this->clickCounter);
                this->clickCounter = 0;
            }
            break;
        }
        this->state = SmartButton::State::PRESSED;
        this->pressTick = getTickValue();
        break;
    case SmartButton::State::PRESSED:
        if (this->pressedFlag == false) {
            this->clickCounter++;
            this->state = SmartButton::State::RELEASED;
            break;
        }
        if (getTickValue() - this->pressTick < this->holdTimeout)
            break;
        this->eventCallback(this, SmartButton::Event::HOLD, this->clickCounter);
        this->pressTick = getTickValue();
        this->state = SmartButton::State::HOLD;
        break;
    case SmartButton::State::HOLD:
        if (this->pressedFlag == false) {
            this->state = SmartButton::State::RELEASED;
            this->clickCounter = 0;
            break;
        }
        if (getTickValue() - this->pressTick < this->longHoldTimeout)
            break;
        this->eventCallback(this, SmartButton::Event::LONG_HOLD, this->clickCounter);
        this->pressTick = getTickValue();
        this->state = SmartButton::State::LONG_HOLD;
        break;
    case SmartButton::State::LONG_HOLD:
        if (this->pressedFlag == false) {
            this->state = SmartButton::State::RELEASED;
            this->clickCounter = 0;
        }
        break;
    }
}

void SmartButton::service()
{
    SmartButton *button = _smartButtons;

    while (button != NULL) {
        button->process();
        button = button->next;
    }
}

};
