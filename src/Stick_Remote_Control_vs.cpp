#include "Stick_Button.h"
#include <Mouse.h>
#include <Keyboard.h>
#include <Bounce.h>

// Remote control for XCSoar, emulates a keyboard and mouse.
// Hardware is just push buttons connected between pins of an Arduino Leonardo and Gnd
// For each button press a keystroke or mouse action is sent.
//
// Button layout Stefly Remote as shown on http://www.openvario.org/doku.php?id=projects:remote_00:top
// Additional Speed to Fly switch between Arduino pin 7 and GND

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
