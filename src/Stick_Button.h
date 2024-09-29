#ifndef _STICK_BUTTON_H
#define _STICK_BUTTON_H

#include "Keyboard.h"
#include <Bounce.h>
#include <stdint.h>

enum class Mode {
    Normal,
    Mouse,
    Shift,
};

static constexpr int mouse_move_distance = 1;

static constexpr uint8_t menu_key_press = KEY_F1;
static constexpr uint8_t meny_key_long_press = 'M';
static constexpr uint8_t esc_key_press = KEY_ESC;
static constexpr uint8_t key_vario_mode = 'V';
static constexpr uint8_t key_stf_mode = 'S';

static constexpr uint8_t key_macready_incr = '+';
static constexpr uint8_t key_macready_decr = '-';
static constexpr uint8_t key_volume_incr = '0';
static constexpr uint8_t key_volume_decr = '9';
static constexpr uint8_t key_volume_mute = '8';


class StickButton {

public:
    StickButton(uint8_t pinNumber)
        : mBouncer(pinNumber, DebounceInterval)
    {}

    virtual Mode Update(Mode currentMode){ return currentMode; }

    static constexpr unsigned long DebounceInterval = 10;
    static constexpr unsigned long keyboard_initial_rebounce_interval = 600;
    static constexpr unsigned long keyboard_rebounce_interval = 80;

protected:
    Bounce mBouncer;

    void PressKeyWithRebounce(unsigned key);
};


class MenuButton : public StickButton {
public:
    MenuButton(uint8_t pinNumber)
        : StickButton(pinNumber)
    {}

    Mode Update(Mode currentMode) override;

    static constexpr unsigned long menu_long_press_time = 600;

private:
    unsigned long mPressTime = 0;
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


class JoyStickDirectionButton : public StickButton {
public:
    JoyStickDirectionButton(uint8_t pinNumber)
        : StickButton(pinNumber)
    {}

    static constexpr int mouse_move_distance = 1;
    static constexpr int mouse_rebounce_interval = 4;

protected:
    
    void MoveMouse(int8_t x, int8_t y);
};

class JoystickLeftButton : public JoyStickDirectionButton {
public:

    JoystickLeftButton(uint8_t pinNumber)
        : JoyStickDirectionButton(pinNumber)
    {}

    Mode Update(Mode currentMode) override;
};


class JoystickRightButton : public JoyStickDirectionButton {
public:

    JoystickRightButton(uint8_t pinNumber)
        : JoyStickDirectionButton(pinNumber)
    {}

    Mode Update(Mode currentMode) override;
};


class JoystickUpButton : public JoyStickDirectionButton {
public:

    JoystickUpButton(uint8_t pinNumber)
        : JoyStickDirectionButton(pinNumber)
    {}

    Mode Update(Mode currentMode) override;
};


class JoystickDownButton : public JoyStickDirectionButton {
public:

    JoystickDownButton(uint8_t pinNumber)
        : JoyStickDirectionButton(pinNumber)
    {}

    Mode Update(Mode currentMode) override;
};

#endif
