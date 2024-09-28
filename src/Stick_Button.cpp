#include "Stick_Button.h"
#include "Mouse.h"
#include <Keyboard.h>


#define RETURN_IF_NO_UPDATE if (mBouncer.update() != 1) return currentMode; \
    if (!mBouncer.fallingEdge()) return currentMode; \


void JoyStickDirectionButton::MoveMouse(int8_t x, int8_t y)
{
    while (mBouncer.read() == 0) {
        Mouse.move(x, y);
        delay(mouse_rebounce_interval);
        mBouncer.update();
    }
}

Mode MenuButton::Update(Mode currentMode)
{
    RETURN_IF_NO_UPDATE;

    Keyboard.press(KEY_F5);
    return currentMode;
}


Mode EscapeButton::Update(Mode currentMode)
{
    RETURN_IF_NO_UPDATE;

    Keyboard.press(KEY_ESC);
    Keyboard.releaseAll();
    Keyboard.press(KEY_ESC);

    return currentMode;
}


Mode FunctionButton::Update(Mode currentMode)
{
    RETURN_IF_NO_UPDATE;

    switch (currentMode){
        case Mode::Normal:
            return Mode::Mouse;
        case Mode::Mouse:
            return Mode::Normal;
    }

    return currentMode;
}


Mode STFButton::Update(Mode currentMode)
{
    RETURN_IF_NO_UPDATE;
    // TODO
    return currentMode;
}


Mode JoystickDepressButton::Update(Mode currentMode)
{
    RETURN_IF_NO_UPDATE;

    switch (currentMode){
        case Mode::Normal:
            Keyboard.press(KEY_RETURN);
            break;
        case Mode::Mouse:
            Mouse.click(MOUSE_LEFT);
            break;
    }
    return currentMode;
}


Mode JoystickRightButton::Update(Mode currentMode)
{
    RETURN_IF_NO_UPDATE;
    
    switch (currentMode) {
        case Mode::Normal:
            // TODO: Handle rebouncing
            Keyboard.press(KEY_RIGHT_ARROW);
            break;
        case Mode::Mouse:
            MoveMouse(mouse_move_distance, 0);
            break;
    }
    return currentMode;
}


Mode JoystickLeftButton::Update(Mode currentMode)
{
    RETURN_IF_NO_UPDATE;
    
    switch (currentMode) {
        case Mode::Normal:
            // TODO: Handle rebouncing
            Keyboard.press(KEY_LEFT_ARROW);
            break;
        case Mode::Mouse:
            MoveMouse(-mouse_move_distance, 0);
            break;
    }
    return currentMode;
}


Mode JoystickUpButton::Update(Mode currentMode)
{
    RETURN_IF_NO_UPDATE;
    
    switch (currentMode) {
        case Mode::Normal:
            // TODO: Handle rebouncing
            Keyboard.press(KEY_UP_ARROW);
            break;
        case Mode::Mouse:
            MoveMouse(0, -mouse_move_distance);
            break;
    }
    return currentMode;
}


Mode JoystickDownButton::Update(Mode currentMode)
{
    RETURN_IF_NO_UPDATE;
    
    switch (currentMode) {
        case Mode::Normal:
            // TODO: Handle rebouncing
            Keyboard.press(KEY_DOWN_ARROW);
            break;
        case Mode::Mouse:
            MoveMouse(0, mouse_move_distance);
            break;
    }
    return currentMode;
}
