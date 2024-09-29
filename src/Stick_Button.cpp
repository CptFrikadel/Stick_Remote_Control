#include "Stick_Button.h"
#include "Arduino.h"
#include "Mouse.h"
#include <Keyboard.h>


#define RETURN_IF_NO_UPDATE if (mBouncer.update() != 1) return currentMode; \
    if (!mBouncer.fallingEdge()) return currentMode; \

void StickButton::PressKeyWithRebounce(unsigned key)
{
    auto start = millis();

    Keyboard.press(key);
    Keyboard.releaseAll();

    // Hold for initial delay
    while (mBouncer.read() == 0){
        if (millis() - start > keyboard_initial_rebounce_interval){
            Keyboard.press(key);
            Keyboard.releaseAll();
            start = millis();
            break;
        }
        mBouncer.update();
    }

   while (mBouncer.read() == 0) {
       if (millis() - start > keyboard_rebounce_interval){
           Keyboard.press(key);
           Keyboard.releaseAll();
           start = millis();
       }
       mBouncer.update();
   }
}

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
    if (mBouncer.update() != 1) return currentMode;

    if (mBouncer.fallingEdge()) {
        mPressTime = millis(); 
        return currentMode;
    }

    if (!mBouncer.risingEdge()) return currentMode;

    if (millis() - mPressTime > menu_long_press_time) {
        Keyboard.press(meny_key_long_press);
    } else {
        Keyboard.press(menu_key_press);
    }

    return currentMode;
}


Mode EscapeButton::Update(Mode currentMode)
{
    RETURN_IF_NO_UPDATE;

    Keyboard.press(esc_key_press);
    Keyboard.releaseAll();
    Keyboard.press(esc_key_press);

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
    if (mBouncer.update() != 1) return currentMode;

    if (mBouncer.fallingEdge()) {
        mPressTime = millis(); 
        return currentMode;
    }

    if (!mBouncer.risingEdge()) return currentMode;

    if (millis() - mPressTime > stf_long_press_time) {
        switch (mMode) {
            case VarioMode::Vario:
                Keyboard.press(key_stf_mode);
                mMode = VarioMode::SpeedCommand;
                break;
            case VarioMode::SpeedCommand:
                Keyboard.press(key_vario_mode);
                mMode = VarioMode::Vario;
                break;
        }
    } else {
        Keyboard.press(key_vario_mode);
        mMode = VarioMode::Vario;
    }

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
            PressKeyWithRebounce(KEY_RIGHT_ARROW);
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
            PressKeyWithRebounce(KEY_LEFT_ARROW);
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
            PressKeyWithRebounce(KEY_UP_ARROW);
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
            PressKeyWithRebounce(KEY_DOWN_ARROW);
            break;
        case Mode::Mouse:
            MoveMouse(0, mouse_move_distance);
            break;
    }
    return currentMode;
}
