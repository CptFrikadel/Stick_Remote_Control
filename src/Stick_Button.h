#ifndef _STICK_BUTTON_H
#define _STICK_BUTTON_H

#include <Bounce.h>
#include <stdint.h>

enum class Mode {
    Normal,
    Mouse,
    Shift,
};


class StickButton {

public:
    StickButton(uint8_t pinNumber)
        : mBouncer(pinNumber, DebounceInterval)
    {}

    virtual Mode Update(Mode currentMode){ return currentMode; }

    static unsigned long DebounceInterval;
protected:
    Bounce mBouncer;
};


class MenuButton : public StickButton {
public:
    MenuButton(uint8_t pinNumber)
        : StickButton(pinNumber)
    {}

    Mode Update(Mode currentMode) override;
};


class EscapeButton : public StickButton {
public:
    EscapeButton(uint8_t pinNumber)
        : StickButton(pinNumber)
    {}

    Mode Update(Mode currentMode) override;
};


class FunctionButton : public StickButton {
public:
    FunctionButton(uint8_t pinNumber)
        : StickButton(pinNumber)
    {}

    Mode Update(Mode currentMode) override;
};


class JoystickDepressButton : public StickButton {
public:
    JoystickDepressButton(uint8_t pinNumber)
        : StickButton(pinNumber)
    {}

    Mode Update(Mode currentMode) override;
};


class STFButton : public StickButton {
public:
    STFButton(uint8_t pinNumber)
        : StickButton(pinNumber)
    {}

    Mode Update(Mode currentMode) override;
};


class JoystickDirectionButton : public StickButton {
public:
    enum class Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT,
    };

    JoystickDirectionButton(uint8_t pinNumber, Direction direction)
        : StickButton(pinNumber)
        , mDirection(direction)
    {}

    Mode Update(Mode currentMode) override;

private:
    Direction mDirection;

};


#endif
