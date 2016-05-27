# PowerTimer
Arduino project to control power to soldering iron or hot glue gun, with an auto-shutoff timer.

110 V Hardware is based off the Sparkfun Controllable Power Outlet (https://www.sparkfun.com/tutorials/119).
* A 5-V power supply is always on.
* Two relays individually control two power outlets.  A standard 2-channel, active-low, 5 V relay module is used.
* Provides power, ground, Relay 1 input, and Relay 2 input.

Arduino Nano uses 2 buttons for input: one for each relay timer.
* Buttons are INPUT_PULLUP, so no resistor is needed.
* Outputs are LOW to activate relays, HIGH to turn them off.
* Pin 13 LED and TX LED (pin 1) are used for visual feedback.  Pin 13 is active high, while TX is active low.
* Buzzer is connected between two digital pins.
* Some pins are set to output LOW because of my tiny breadboard.
