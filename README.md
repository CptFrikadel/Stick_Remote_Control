# Stick_Remote_Control

Source code for the [SteFly remote stick control](https://www.stefly.aero/product/stefly-leather-remote-stick/)

Original source taken from [here](https://stefly.aero/wp-content/uploads/2017/05/Stick_Remote_Control_vs.zip).

This implementation uses the STF trigger switch as a 'Shift' key to trigger
another layer. To use this layer, new xcsoar events need to be defined in your
.xci file, see `src/Stick_Button.h` for the keybindings.

On the 'normal' layer, the remote works as previous (just without STF switching). I.e:
 - Pressing Menu opens the menu (F1), longpress opens the vario menu ('M')
 - The Joystick operates the arrow keys, or the mouse depending on wether
mouse mode is active

 When the STF trigger is held, the 'Shift' layer is activated.
 - Joystick Right/Left now Increases/Decreases Macready ('+' / '-')
 - Joystick Up/Down now Increases/Decreases vario Volume ('0' / '9')
 - The Menu key toggles between Vario and STF mode without requiring a
   longpress ('V') / ('S')


#### Manual update

The project is built using the [Platform IO](https://platformio.org/) ecosystem.
```sh
platformio run -t upload
```
