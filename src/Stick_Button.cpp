#include "Stick_Button.h"
#include "Arduino.h"
#include "Mouse.h"
#include <Keyboard.h>


#define RETURN_IF_NO_UPDATE if (mBouncer.update() != 1) return currentMode; \
    if (!mBouncer.fallingEdge()) return currentMode; \


enum class VarioMode {
    Vario,
    SpeedCommand,
};

static VarioMode varioMode = VarioMode::Vario;

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
        switch (currentMode){
            case Mode::Normal:
            case Mode::Mouse:
                Keyboard.press(meny_key_long_press);
                break;
            case Mode::Shift:
                if (varioMode == VarioMode::Vario) {
                    varioMode = VarioMode::SpeedCommand;
                    Keyboard.press(key_stf_mode);
                } else {
                    varioMode = VarioMode::Vario;
                    Keyboard.press(key_vario_mode);
                }
                break;
        }
    } else {
        switch (currentMode) {
        
            case Mode::Normal:
            case Mode::Mouse:
                Keyboard.press(menu_key_press);
                break;
            case Mode::Shift:
                if (varioMode == VarioMode::Vario) {
                    varioMode = VarioMode::SpeedCommand;
                    Keyboard.press(key_stf_mode);
                } else {
                    varioMode = VarioMode::Vario;
                    Keyboard.press(key_vario_mode);
                }
                break;
        }
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
        case Mode::Shift:
            Keyboard.press(key_volume_mute);
            break;
    }

    return currentMode;
}


Mode STFButton::Update(Mode currentMode)
{
    if (mBouncer.update() != 1) return currentMode;

    if (mBouncer.fallingEdge()) return Mode::Shift;

    if (mBouncer.risingEdge()) return Mode::Normal;

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
        case Mode::Shift:
            return currentMode;
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
        case Mode::Shift:
            PressKeyWithRebounce(key_macready_incr);
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
        case Mode::Shift:
            PressKeyWithRebounce(key_macready_decr);
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
        case Mode::Shift:
            PressKeyWithRebounce(key_volume_incr);
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
        case Mode::Shift:
            PressKeyWithRebounce(key_volume_decr);
            break;
    }
    return currentMode;
}
