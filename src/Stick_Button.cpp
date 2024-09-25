#include "Stick_Button.h"
#include <Keyboard.h>

unsigned long StickButton::DebounceInterval = 20;

Mode MenuButton::Update(Mode currentMode)
{
    if (mBouncer.update() && mBouncer.fallingEdge()){
        Keyboard.press(KEY_F5);
    }
    return currentMode;
}


Mode EscapeButton::Update(Mode currentMode)
{
    if (mBouncer.update() && mBouncer.fallingEdge()){
        Keyboard.press(KEY_ESC);
        Keyboard.releaseAll();
        Keyboard.press(KEY_ESC);
    }

    return currentMode;
}


Mode FunctionButton::Update(Mode currentMode)
{
    if (!mBouncer.update()) return currentMode;
    if (!mBouncer.fallingEdge()) return currentMode;

    switch (currentMode){
        case Mode::Normal:
            return Mode::Mouse;
        case Mode::Mouse:
            return Mode::Normal;
        case Mode::Shift:
            return Mode::Shift;
    }

    return currentMode;
}


Mode STFButton::Update(Mode currentMode)
{
    // TODO
    return currentMode;
}


Mode JoystickDepressButton::Update(Mode currentMode)
{
    // TODO
    return currentMode;
}


Mode JoystickDirectionButton::Update(Mode currentMode)
{
    //TODO
    return currentMode;
}
