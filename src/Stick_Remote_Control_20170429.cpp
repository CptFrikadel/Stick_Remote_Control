#include <Mouse.h>
#include <Keyboard.h>
#include <Bounce.h>


// remote control for XCSoar, emulates a keyboard and mouse
// hardware is just pushbuttons connected between pins of an Arduino Leonardo and Gnd
// for each button press a keystroke or mouse action is sent
// code is a wild mix of various snippets found on the net mixed up by an software illiterate 
// I started from http://forum.arduino.cc/index.php?topic=80913.msg1077713#msg1077713 and 
// modified by copy & paste trial and error. Kudos to Paul for the great starting point! 
//
// button layout Stefly Remote as shown on http://www.openvario.org/doku.php?id=projects:remote_00:top 
// additional Speed to Fly switch between Arduino pin 4 and GND



byte buttons[]={9,1,2,14,5,7,3,15,4,6};//seperate array from definitions to set up the pins
#define NUMBUTTONS sizeof(buttons)//gives size of array *helps for adding buttons

int debounce_delay = 20;       //Debounce delay in milliseconds
int mouse_rebounce_interval = 4;
int keyboard_rebounce_interval = 200;
int button_pressed = 99;
long unsigned time_pressed;
long unsigned time_released;
boolean Mouse_Active = 0;
const int Mouse_Move_Distance = 1;


// I really dont see getting around doing this manually
Bounce bouncer[] = { //would guess thats what the fuss is about
        Bounce(9,debounce_delay),   // Button 0 = top button (Mode)
        Bounce(1,debounce_delay),  // Button 1 = upper RH button (ALT)
        Bounce(2,debounce_delay),  // Button 2 = joystick up
        Bounce(14,debounce_delay),   // Button 3 = upper LH button (QM)
        Bounce(5,debounce_delay),   // Button 4 = joystick left
        Bounce(7,debounce_delay),   // Button 5 = STF switch
        Bounce(3,debounce_delay),  // Button 6 = joystick right
        Bounce(15,debounce_delay),  // Button 7 = lower RH button (ESC)
        Bounce(4,debounce_delay),   // Button 8 = joystick down
        Bounce(6,debounce_delay),  // Button 9 = joystick press
};

void setup() {
    for (byte set=0;set<=NUMBUTTONS;set++){//sets the button pins
        pinMode(buttons[set],INPUT);
        digitalWrite(buttons[set],HIGH);//<-comment out this line if not using internal pull ups
    }//-----------------------------------and change read()==to high if your set up requires
    // pinMode(LED,OUTPUT);//------------------otherwise event will occure on release

// Wait five seconds since the HID drivers need a bit of time to re-mount after upload.
    delay(1000);
    Keyboard.begin();
    Mouse.begin();
//  Serial.begin(57600);
//  Serial.println("Pushbutton Bounce library test:");
}

void loop() {

    while (button_pressed == 99) {
        for(int num=0;num<NUMBUTTONS;num++){
            if ( bouncer[num].update()) {
                if ( bouncer[num].fallingEdge()) {
                    button_pressed = num;    //button_pressed=0-10 when button is pressed
//       Serial.println("Button Pressed");
//       Serial.println(button_pressed);
                    break;}
                if ( bouncer[num].risingEdge()) {
                    button_pressed = num+50; //button_pressed=50-60 when button is released
//       Serial.println("Button Pressed");
//       Serial.println(button_pressed);
                    break;}
            }
        }
    }

    if (Mouse_Active) {
        if (button_pressed == 0) Mouse_Active = 0;
        if (button_pressed == 1) Keyboard.press(KEY_F5);
        if (button_pressed == 2){
            while (bouncer[2].read() == 0) {
                Mouse.move(0, -Mouse_Move_Distance);
                delay(mouse_rebounce_interval);
                bouncer[2].update();}}
        //Serial.println("Bouncer Read");
        //Serial.println(bouncer[2].read());
        if (button_pressed == 3) time_pressed = millis();
        if (button_pressed == 53) {
            time_released = millis();
            if (((time_released - time_pressed) < 500)) Keyboard.press(KEY_F1);
            else Keyboard.press('M');
        }
        if (button_pressed == 4) {
            while (bouncer[4].read() == 0) {
                Mouse.move(-Mouse_Move_Distance, 0);
                delay(mouse_rebounce_interval);
                bouncer[4].update();}}
        if (button_pressed == 5) {
            time_pressed = millis();
        }
        if (button_pressed == 55) {
            time_released = millis();
            if (((time_released - time_pressed) < 300)) Keyboard.press('V');
            else Keyboard.press('S');
        }
        if (button_pressed == 6) {
            while (bouncer[6].read() == 0) {
                Mouse.move(Mouse_Move_Distance, 0);
                delay(mouse_rebounce_interval);
                bouncer[6].update();}}
        if (button_pressed == 7) {
            Keyboard.press(KEY_ESC);
            Keyboard.releaseAll();
            Keyboard.press(KEY_ESC);}
        if (button_pressed == 8) {
            while (bouncer[8].read() == 0) {
                Mouse.move(0, Mouse_Move_Distance);
                delay(mouse_rebounce_interval);
                bouncer[8].update();}}
        if (button_pressed == 9) Mouse.click(MOUSE_LEFT);
    }
    else {
        if (button_pressed == 0) Mouse_Active = 1;
        if (button_pressed == 1) Keyboard.press(KEY_F5);
        if (button_pressed == 2) {
            while (bouncer[2].read() == 0) {
                Keyboard.press(KEY_UP_ARROW);
                Keyboard.releaseAll();
                delay(keyboard_rebounce_interval);
                bouncer[2].update();}}
        if (button_pressed == 3) {
            time_pressed = millis();
        }
        if (button_pressed == 53) {
            time_released = millis();
            if (((time_released - time_pressed) < 500)) Keyboard.press(KEY_F1);
            else Keyboard.press('M');
        }
        if (button_pressed == 4) {
            while (bouncer[4].read() == 0) {
                Keyboard.press(KEY_LEFT_ARROW);
                Keyboard.releaseAll();
                delay(keyboard_rebounce_interval);
                bouncer[4].update();}}
        if (button_pressed == 5) {
            time_pressed = millis();
        }
        if (button_pressed == 55) {
            time_released = millis();
            if (((time_released - time_pressed) < 300)) Keyboard.press('V');
            else Keyboard.press('S');
        }
        if (button_pressed == 6) {
            while (bouncer[6].read() == 0) {
                Keyboard.press(KEY_RIGHT_ARROW);
                Keyboard.releaseAll();
                delay(keyboard_rebounce_interval);
                bouncer[6].update();}}
        if (button_pressed == 7) {
            Keyboard.press(KEY_ESC);
            Keyboard.releaseAll();
            Keyboard.press(KEY_ESC);}
        if (button_pressed == 8) {
            while (bouncer[8].read() == 0) {
                Keyboard.press(KEY_DOWN_ARROW);
                Keyboard.releaseAll();
                delay(keyboard_rebounce_interval);
                bouncer[8].update();}}
        if (button_pressed == 9) Keyboard.press(KEY_RETURN);
    }

    Keyboard.releaseAll();
    button_pressed = 99;
}