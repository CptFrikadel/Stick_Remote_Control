#include <Mouse.h>
#include <Keyboard.h>
#include <Bounce.h>

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
#define NUMBUTTONS sizeof(buttons)  //gives size of array *helps for adding buttons
#define DEBOUNCE_DELAY 20           //Debounce delay in milliseconds
#define MOUSE_REBOUNCE_INTERVAL 4
#define KEYBOARD_REBOUNCE_INTERVAL 400
#define MOUSE_MOVE_DISTANCE  1

// I really don't see getting around doing this manually
Bounce bouncer[] = { //would guess that's what the fuss is about
        Bounce(9, DEBOUNCE_DELAY),    // Button 0 = top button (Mode)
        Bounce(1, DEBOUNCE_DELAY),    // Button 1 = upper RH button (ALT)
        Bounce(2, DEBOUNCE_DELAY),    // Button 2 = joystick up
        Bounce(14, DEBOUNCE_DELAY),   // Button 3 = upper LH button (QM)
        Bounce(5, DEBOUNCE_DELAY),    // Button 4 = joystick left
        Bounce(7, DEBOUNCE_DELAY),    // Button 5 = STF switch
        Bounce(3, DEBOUNCE_DELAY),    // Button 6 = joystick right
        Bounce(15, DEBOUNCE_DELAY),   // Button 7 = lower RH button (ESC)
        Bounce(4, DEBOUNCE_DELAY),    // Button 8 = joystick down
        Bounce(6, DEBOUNCE_DELAY),    // Button 9 = joystick press
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

unsigned button_pressed;
long unsigned time_pressed;
long unsigned time_released;
bool speed = false;
#ifdef SUPPORT_MOUSE_MODE
bool mouse_active = false;
#endif

void loop() {
    unsigned last_button_pressed = button_pressed;
    button_pressed = 99;

    while (button_pressed == 99) {
        switch (last_button_pressed) {
            case 3:
                if (millis() - time_pressed > 600) {
                    Keyboard.press('M');
                    Keyboard.releaseAll();
                    last_button_pressed = button_pressed;
                }
                break;
            case 5:
                if (millis() - time_pressed >= 800) {
                    Keyboard.press('S');
                    speed = true;
                    Keyboard.releaseAll();
                    last_button_pressed = button_pressed;
                }
                break;
            default:
                break;

        }
        for (unsigned num = 0; num < NUMBUTTONS; num++) {
            if (bouncer[num].update()) {
                if (bouncer[num].fallingEdge()) {
                    button_pressed = num;    //button_pressed=0-10 when button is pressed
                    break;
                }
                if (bouncer[num].risingEdge()) {
                    button_pressed = num + 50; //button_pressed=50-60 when button is released
                    break;
                }
            }
        }
    }

#ifdef SUPPORT_MOUSE_MODE
    if (mouse_active) {
        switch (button_pressed) {
            case 0:
                mouse_active = false;
                break;
            case 1:
                Keyboard.press(KEY_F5);
                break;
            case 2:
                while (bouncer[2].read() == 0) {
                    Mouse.move(0, -MOUSE_MOVE_DISTANCE);
                    delay(MOUSE_REBOUNCE_INTERVAL);
                    bouncer[2].update();
                }
                break;
            case 3:
                time_pressed = millis();
                break;
            case 53:
                time_released = millis();
                if (time_released - time_pressed < 500) {
                    Keyboard.press(KEY_F1);
                } else {
                    Keyboard.press('M');
                }
                break;
            case 4:
                while (bouncer[4].read() == 0) {
                    Mouse.move(-MOUSE_MOVE_DISTANCE, 0);
                    delay(MOUSE_REBOUNCE_INTERVAL);
                    bouncer[4].update();
                }
                break;
            case 5:
                time_pressed = millis();
                break;
            case 55:
                time_released = millis();
                if (time_released - time_pressed < 300) {
                    Keyboard.press('V');
                }
                break;
            case 6:
                while (bouncer[6].read() == 0) {
                    Mouse.move(MOUSE_MOVE_DISTANCE, 0);
                    delay(MOUSE_REBOUNCE_INTERVAL);
                    bouncer[6].update();
                }
                break;
            case 7:
                Keyboard.press(KEY_ESC);
                Keyboard.releaseAll();
                Keyboard.press(KEY_ESC);
                break;
            case 8:
                while (bouncer[8].read() == 0) {
                    Mouse.move(0, MOUSE_MOVE_DISTANCE);
                    delay(MOUSE_REBOUNCE_INTERVAL);
                    bouncer[8].update();
                }
                break;
            case 9:
                Mouse.click(MOUSE_LEFT);
                break;
            default:
                break;
        }
    } else {
#endif
    switch (button_pressed) {
        case 0:
#ifdef SUPPORT_MOUSE_MODE
            mouse_active = true;
#else
            //Keyboard.press('1');
            Keyboard.press(speed ? 'V' : 'S');
            speed = !speed;
#endif
            break;
        case 1:
            Keyboard.press(KEY_F5);
            break;
        case 2:
            while (bouncer[2].read() == 0) {
                Keyboard.press(KEY_UP_ARROW);
                Keyboard.releaseAll();
                delay(KEYBOARD_REBOUNCE_INTERVAL);
                bouncer[2].update();
            }
            break;
        case 3:
            time_pressed = millis();
            break;
        case 53:
            time_released = millis();
            if (time_released - time_pressed < 500) {
                Keyboard.press(KEY_F1);
            }
            break;
        case 4:
            while (bouncer[4].read() == 0) {
                Keyboard.press(KEY_LEFT_ARROW);
                Keyboard.releaseAll();
                delay(KEYBOARD_REBOUNCE_INTERVAL);
                bouncer[4].update();
            }
            break;
        case 5:
            time_pressed = millis();
            break;
        case 55:
            time_released = millis();
            if (time_released - time_pressed > 200 && time_released - time_pressed < 800) {
                Keyboard.press('V');
                speed = false;
            }
            break;
        case 6:
            while (bouncer[6].read() == 0) {
                Keyboard.press(KEY_RIGHT_ARROW);
                Keyboard.releaseAll();
                delay(KEYBOARD_REBOUNCE_INTERVAL);
                bouncer[6].update();
            }
            break;
        case 7:
            Keyboard.press(KEY_ESC);
            Keyboard.releaseAll();
            Keyboard.press(KEY_ESC);
            break;
        case 8:
            while (bouncer[8].read() == 0) {
                Keyboard.press(KEY_DOWN_ARROW);
                Keyboard.releaseAll();
                delay(KEYBOARD_REBOUNCE_INTERVAL);
                bouncer[8].update();
            }
            break;
        case 9:
            Keyboard.press(KEY_RETURN);
            break;
        default:
            break;
    }
#ifdef SUPPORT_MOUSE_MODE
    }
#endif
    Keyboard.releaseAll();
}