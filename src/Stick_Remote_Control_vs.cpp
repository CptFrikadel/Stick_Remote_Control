#include "Stick_Button.h"
#include <Mouse.h>
#include <Keyboard.h>
#include <Bounce.h>

/**
 * Re-implementation of Remote control for XCSoar, emulates a keyboard and
 * mouse. Hardware is just push buttons connected between pins of an Arduino
 * Leonardo and Gnd For each button press a keystroke or mouse action is sent.
 *
 * Button layout Stefly Remote as shown on
 * http://www.openvario.org/doku.php?id=projects:remote_00:top Additional Speed
 * to Fly switch between Arduino pin 7 and GND
 *
 * This implementation uses the STF trigger switch as a 'Shift' key to trigger
 * another layer. To use this layer, a custom event file is to be used. See
 * src/Stick_Button.h for the keybindings.
 *
 * On the 'normal' layer, the remote works as normal (without STF). I.e:
 *  - Pressing Menu opens the menu (F1), longpress opens the vario menu ('M')
 *  - The Joystick operates the arrow keys, or the mouse depending on wether
 * mouse mode is active
 *
 *  When the STF trigger is held, the 'Shift' layer is activated.
 *  - Joystick Right/Left now Increases/Decreases Macready ('+' / '-')
 *  - Joystick Up/Down now Increases/Decreases vario Volume ('0' / '9')
 *  - The Menu key toggles between Vario and STF mode without requiring a
 *    longpress ('V') / ('S')
 *
 */

MenuButton menuButton(1);
EscapeButton escapeButton(15);
FunctionButton functionButton(9);
JoystickDepressButton joystickPressButton(6);
STFButton speedToFlyButton(7);

JoystickUpButton joystickUp(2);
JoystickRightButton joystickRight(3);
JoystickDownButton joystickDown(4);
JoystickLeftButton joystickLeft(5);

StickButton* StickButtons[] = {
    &menuButton,
    &escapeButton,
    &functionButton,
    &joystickPressButton,
    &speedToFlyButton,
    &joystickUp,
    &joystickDown,
    &joystickLeft,
    &joystickRight,
};


void setup() {
    for (auto button : StickButtons) { //sets the button pins
        button->Setup();
    }

// Wait five seconds since the HID drivers need a bit of time to re-mount after upload.
    delay(1000);
    Keyboard.begin();
    Mouse.begin();
}

Mode currentMode = Mode::Normal;

void loop() {

    for (unsigned i = 0; i < sizeof(StickButtons) / sizeof(StickButton*); i++)
    {
        currentMode = StickButtons[i]->Update(currentMode);
    }

    Keyboard.releaseAll();
}
