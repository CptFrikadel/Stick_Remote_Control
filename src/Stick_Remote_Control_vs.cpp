#include "Stick_Button.h"
#include <Mouse.h>
#include <Keyboard.h>
#include <Bounce.h>
#include <stdint.h>

// remote control for XCSoar, emulates a keyboard and mouse
// hardware is just push buttons connected between pins of an Arduino Leonardo and Gnd
// for each button press a keystroke or mouse action is sent
// code is a wild mix of various snippets found on the net mixed up by a software illiterate
// I started from http://forum.arduino.cc/index.php?topic=80913.msg1077713#msg1077713 and
// modified by copy & paste trial and error. Kudos to Paul for the great starting point!
//
// button layout Stefly Remote as shown on http://www.openvario.org/doku.php?id=projects:remote_00:top
// additional Speed to Fly switch between Arduino pin 4 and GND

uint8_t buttons[] = {9, 1, 2, 14, 5, 7, 3, 15, 4, 6}; //separate array from definitions to set up the pins

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
    for (auto button : buttons) { //sets the button pins
        pinMode(button, INPUT);
        digitalWrite(button, HIGH);  //<-comment out this line if not using internal pull-ups
    }//-----------------------------------and change read()==to high if your set up requires
    // pinMode(LED,OUTPUT);//------------------otherwise event will occur on release

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
