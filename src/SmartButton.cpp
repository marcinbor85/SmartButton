#include "SmartButton.h"

namespace smartbutton {

SmartButton *_smartButtons = NULL;

SmartButton::SmartButton(
    int pin,
    SmartButton::InputType inputType,
    SmartButton::IsPressedHandler isPressedHandler,
    bool *isPressedFlag,
    SmartButtonInterface *interface,
    unsigned long debounceTimeout,
    unsigned long clickTimeout,
    unsigned long holdTimeout,
    unsigned long longHoldTimeout):
        pin(pin),
        inputType(inputType),
        isPressedHandler(isPressedHandler),
        isPressedFlag(isPressedFlag),
        interface(interface),
        debounceTimeout(debounceTimeout),
        clickTimeout(clickTimeout),
        holdTimeout(holdTimeout),
        longHoldTimeout(longHoldTimeout),
        eventCallback(NULL)
{

}

SmartButton::SmartButton(SmartButtonInterface *interface): SmartButton(-1, SmartButton::InputType::NORMAL_HIGH, NULL, NULL, interface)
{
    
}

SmartButton::SmartButton(bool *isPressedFlag): SmartButton(-1, SmartButton::InputType::NORMAL_HIGH, NULL, isPressedFlag, NULL)
{
    
}

SmartButton::SmartButton(SmartButton::IsPressedHandler isPressed): SmartButton(-1, SmartButton::InputType::NORMAL_HIGH, isPressed, NULL, NULL)
{
    
}

SmartButton::SmartButton(int pin, SmartButton::InputType inputType): SmartButton(pin, inputType, NULL, NULL, NULL)
{
    
}

void SmartButton::begin(void *context)
{
    this->context = context;

    this->debounceTick = 0;
    this->pressTick = 0;
    this->clickCounter = 0;

    this->pressedFlag = false;
    this->state = SmartButton::State::RELEASED;

    this->next = _smartButtons;
    _smartButtons = this;
}

void SmartButton::begin(SmartButton::EventCallback eventCallback, void *context)
{
    this->eventCallback = eventCallback;
    this->begin(context);
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

void* SmartButton::getContext()
{
    return this->context;
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
    } else if (this->isPressedHandler != NULL) {
        return this->isPressedHandler(this);
    } else if (this->isPressedFlag != NULL) {
        return *this->isPressedFlag;
    } else if (this->interface != NULL) {
        return this->interface->isPressed(this);
    } else {
        return false;
    }
}

void SmartButton::callEvent(SmartButton::Event event)
{
    if (this->eventCallback != NULL) {
        this->eventCallback(this, event, this->clickCounter);
    } else if (this->interface != NULL) {
        this->interface->event(this, event, this->clickCounter);
    }
}

void SmartButton::debounce()
{
    if (this->getInputState() == this->pressedFlag) {
        this->debounceTick = getTickValue();
    } else if (getTickValue() - this->debounceTick >= this->debounceTimeout) {
        this->debounceTick = getTickValue();
        this->pressedFlag = !this->pressedFlag;
        this->callEvent((this->pressedFlag != false) ? SmartButton::Event::PRESSED : SmartButton::Event::RELEASED);
    }
}

void SmartButton::process()
{
    this->debounce();

    switch (this->state) {
    case SmartButton::State::RELEASED:
        if (this->pressedFlag == false) {
            if ((this->clickCounter != 0) && (getTickValue() - this->pressTick > this->clickTimeout)) {
                this->callEvent(SmartButton::Event::CLICK);
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
        this->callEvent(SmartButton::Event::HOLD);
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
        this->callEvent(SmartButton::Event::LONG_HOLD);
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
